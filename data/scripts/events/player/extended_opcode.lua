local OPCODE_LANGUAGE = 1

local event = Event()

event.onPlayerExtendedOpcode = function(self, opcode, buffer)
	local client = self:getClient()
	if client.os < CLIENTOS_OTCLIENT_LINUX then
		return
	end

	if opcode == OPCODE_LANGUAGE then
		-- otclient language
		if buffer == 'en' or buffer == 'pt' then
			-- example, setting player language, because otclient is multi-language...
			-- self:setStorageValue(SOME_STORAGE_ID, SOME_VALUE)
		end
	else
		-- other opcodes can be ignored, and the server will just work fine...
	end
end

event:register()
