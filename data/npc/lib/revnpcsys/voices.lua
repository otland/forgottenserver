--[[
    >> NpcVoices <<

    Description:
        - The NpcVoices module provides a way to manage the voices of NPCs which he randomly says.
        - It keeps track of the last voice used by an NPC, as well as the time it was used.
        - The class provides methods to check if a voice can be used and to get the voice to use.

    Functions:
        - NpcVoices:clear()
        - NpcVoices:canUseVoice(handler)
]]

---@class NpcVoices
---@field lastVoice table<string, any>
---@field lastVoiceTime number
---@field clear fun()
---@field canUseVoice fun(handler: table): boolean, table|nil

if not NpcVoices then
    -- If NpcVoices doesn't exist, it's created as an empty table
    NpcVoices = {}
    -- The metatable is set up to call the function when NpcVoices is called
    setmetatable(NpcVoices, {
        __call = function(self, npc)
            -- If the NPC doesn't have NpcVoices, one is created for it
            if not self[npc:getId()] then
                self[npc:getId()] = {
                    lastVoice = nil,
                    lastVoiceTime = 0
                }
                setmetatable(self[npc:getId()], {__index = NpcVoices})
            end
            -- The NpcVoices is returned
            return self[npc:getId()]
        end
    })
end

-- Clears all NpcVoices data for an NPC.
function NpcVoices:clear()
    self = nil
end

-- Checks if a voice can be used.
---@param handler table The NpcsHandler to get the voices from.
---@return boolean, table|nil if the voice can be used, false otherwise.
function NpcVoices:canUseVoice(handler)
    local voices = {}

    if self.lastVoiceTime >= os.mtime() then
        return false
    end

    for _,voice in pairs(handler.voices) do
        if math.random(1, 100) <= voice.chance then
            table.insert(voices, voice)
        end
    end

    self.lastVoiceTime = os.mtime() + handler.voicesDelay

    if #voices == 0 then
        return false
    end

    local voice = voices[math.random(1, #voices)]
    if voice == self.lastVoice and #voices > 1 then
        -- rolling again until we get another voice, we only do this if there is more than one voice to choose from
        repeat
            voice = voices[math.random(1, #voices)]
        until voice ~= self.lastVoice
    end

    self.lastVoice = voice
    return true, voice
end
