function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	player:getPosition():sendMagicEffect(tonumber(param))
	return false
end
