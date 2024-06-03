--[[
    >> NpcTalkQueue <<

    Description:
        - The NpcTalkQueue module provides a way to manage and process a queue of messages for NPCs to say to players.
        
    Functions:
        - NpcTalkQueue:clear()
        - NpcTalkQueue:addToQueue(player: Player, message: string, delay: number)
        - NpcTalkQueue:processQueue()
        - NpcTalkQueue:clearQueue(player: Player)
]]

---@class NpcTalkQueue
---@field queue table<number, table<number, table<string, number>>>
---@field clear fun()
---@field addToQueue fun(player: Player, message: string, delay: number)
---@field processQueue fun()
---@field clearQueue fun(player: Player)
---@field __call fun(npc: Npc): NpcTalkQueue
---@field __index NpcTalkQueue

-- Make sure we are not overloading on reload
if not NpcTalkQueue then
    -- If NpcTalkQueue doesn't exist, it's created as an empty table
    NpcTalkQueue = {}
    -- The metatable is set up to call the function when NpcTalkQueue is called
    setmetatable(NpcTalkQueue, {
        __call = function(self, npc)
            if not self[npc:getId()] then
                self[npc:getId()] = {
                    queue = {}
                }
                setmetatable(self[npc:getId()], {__index = NpcTalkQueue})
            end
            return self[npc:getId()]
        end
    })

    -- Clears all NpcTalkQueue data for an NPC.
    function NpcTalkQueue:clear()
        self = nil
    end

    -- Adds a message to the talk queue for a specific player with a specified delay.
    ---@param player Player The player object.
    ---@param message string The message to be added to the queue.
    ---@param delay number The delay (in milliseconds) before the message is sent.
    function NpcTalkQueue:addToQueue(player, message, delay)
        if not self.queue[player:getId()] then
            self.queue[player:getId()] = {}
        end
        table.insert(self.queue[player:getId()], {
            delay = os.mtime() + delay,
            message = message
        })
    end

    -- Processes the talk queue for each player
    function NpcTalkQueue:processQueue()
        for playerid, queue in pairs(self.queue) do
            local player = Player(playerid)
            if player then
                for id, data in pairs(queue) do
                    if not player then
                        self.queue[playerid] = nil
                        break
                    end
                    if data.delay <= os.mtime() then
                        self.queue[playerid][id] = nil
                        selfSay(data.message, player)
                    end
                end
            else
                -- Clear the queue if the player is no longer online
                self.queue[playerid] = nil
            end
        end
    end

    -- Clears the talk queue for a specific player
    ---@param player Player The player object.
    function NpcTalkQueue:clearQueue(player)
        self.queue[player:getId()] = nil
    end
end
