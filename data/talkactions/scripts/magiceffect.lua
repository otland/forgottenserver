function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	local effect = tonumber(param)
	if not Game.hasEffect(effect) then
		player:sendCancelMessage("Invalid effect id.")
		return false
	end

	player:getPosition():sendMagicEffect(effect)
	return false
end
