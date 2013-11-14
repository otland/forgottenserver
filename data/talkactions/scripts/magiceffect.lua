function onSay(cid, words, param)
	local player = Player(cid)
	if not player:getGroup():getAccess() then
		return false
	end

	player:getPosition():sendMagicEffect(tonumber(param))
	return false
end
