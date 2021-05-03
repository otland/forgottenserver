function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	if player:getAccountType() < ACCOUNT_TYPE_GOD then
		return false
	end

	local position = player:getPosition()
	local npc = Game.createNpc(param, position)
	if not npc then
		player:sendCancelMessage("There is not enough room.")
		position:sendMagicEffect(CONST_ME_POFF)
		return false
	end

	npc:setMasterPos(position)
	position:sendMagicEffect(CONST_ME_MAGIC_RED)
	return false
end
