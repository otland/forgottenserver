--[[
    >> NpcCallbacks <<

    Description:
        - It is used to store the callbacks for a specific NPC, and to check if a callback exists for a specific NPC.

    Functions:
        - NpcCallbacks:hasCallback(callback)
        - NpcCallbacks:onSay(npc, creature, messageType, message)
        - NpcCallbacks:onThink(npc)
        - NpcCallbacks:onMove(npc, oldPos, newPos)
        - NpcCallbacks:onAppear(npc, creature)
        - NpcCallbacks:onDisappear(npc, creature)
        - NpcCallbacks:onSight(npc, creature)
        - NpcCallbacks:onPlayerCloseChannel(npc, creature)
        - NpcCallbacks:onPlayerEndTrade(npc, creature)
]]

---@class NpcCallbacks
---@field onSay fun(npc: Npc, creature: Creature, messageType: number, message: string)
---@field onThink fun(npc: Npc)
---@field onMove fun(npc: Npc, oldPos: Position, newPos: Position)
---@field onAppear fun(npc: Npc, creature: Creature)
---@field onDisappear fun(npc: Npc, creature: Creature)
---@field onSight fun(npc: Npc, creature: Creature)
---@field onPlayerCloseChannel fun(npc: Npc, creature: Creature)
---@field onPlayerEndTrade fun(npc: Npc, creature: Creature)
---@field hasCallback fun(callback: string): boolean

if not NpcCallbacks then
    NpcCallbacks = {}

    setmetatable(NpcCallbacks, {
        __call = function(self, npc)
            if not self[npc:getName()] then
                self[npc:getName()] = {}
            end
            setmetatable(self[npc:getName()], {__index = NpcCallbacks})
            -- The NpcCallbacks is returned
            return self[npc:getName()]
        end
    })

end

---@param callback string The callback to check for.
---@return boolean True if the callback exists, false otherwise.
function NpcCallbacks:hasCallback(callback)
    return self[callback] ~= nil
end
