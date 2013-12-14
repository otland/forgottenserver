function onSay(cid, words, param)
	local player = Player(cid)
	if not player:getGroup():getAccess() then
		return false
	end

	_G["cid"] = cid
	local res, err = loadstring(param)
	if res then
		local ret, err = pcall(res)
		if not ret then
			player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Lua Script Error: " .. err .. ".")
		end
	else
		player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Lua Script Error: " .. err .. ".")
	end
	return false
end