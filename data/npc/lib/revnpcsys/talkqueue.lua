--[[
    >> NpcTalkQueue <<

    Description:
        - The NpcTalkQueue module provides a way to manage and process a queue of messages for NPCs to say to creatures.
        
    Functions:
        - NpcTalkQueue:clear()
        - NpcTalkQueue:addToQueue(creature: Creature, message: string, delay: number)
        - NpcTalkQueue:processQueue()
        - NpcTalkQueue:clearQueue(creature: Creature)
]]

---@class NpcTalkQueue
---@field queue table<number, table<number, table<string, number>>>
---@field clear fun()
---@field addToQueue fun(creature: Creature, message: string, delay: number)
---@field processQueue fun()
---@field clearQueue fun(creature: Creature)
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
end

-- Clears all NpcTalkQueue data for an NPC.
function NpcTalkQueue:clear()
    self = nil
end

-- Adds a message to the talk queue for a specific creature with a specified delay.
---@param creature Creature The creature object.
---@param message string The message to be added to the queue.
---@param delay number The delay (in milliseconds) before the message is sent.
---@param talkType? number The talk type of the message.
function NpcTalkQueue:addToQueue(creature, message, delay, talkType)
    if not self.queue[creature:getId()] then
        self.queue[creature:getId()] = {}
    end
    table.insert(self.queue[creature:getId()], {
        delay = os.mtime() + delay,
        message = message,
        talkType = talkType or TALKTYPE_PRIVATE_NP
    })
end

-- Processes the talk queue for each creature
function NpcTalkQueue:processQueue()
    for creatureid, queue in pairs(self.queue) do
        local creature = Creature(creatureid)
        if creature then
            for id, data in pairs(queue) do
                if not creature then
                    self.queue[creatureid] = nil
                    break
                end
                if data.delay <= os.mtime() then
                    self.queue[creatureid][id] = nil
                    if creature:isPlayer() then
                        selfSay(data.message, creature)
                    else
                        selfSay(data.message, 0, data.talkType)
                    end
                end
            end
        else
            -- Clear the queue if the creature is no longer online
            self.queue[creatureid] = nil
        end
    end
end

-- Clears the talk queue for a specific creature
---@param creature Creature The creature object.
function NpcTalkQueue:clearQueue(creature)
    self.queue[creature:getId()] = nil
end
