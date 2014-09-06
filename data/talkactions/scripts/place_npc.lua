function onSay(cid, words, param)
	local player = Player(cid)
	if not player:getGroup():getAccess() then
		return true
	end

	if player:getAccountType() < ACCOUNT_TYPE_GOD then
		return false
	end

	local position = player:getPosition()
	local npc = Game.createNpc(param, position)
	if npc ~= false then
		position:sendMagicEffect(CONST_ME_MAGIC_RED)
		npc:setMasterPos(position)
	else
		player:sendCancelMessage("There is not enough room.")
		position:sendMagicEffect(CONST_ME_POFF)
	end
	return false
end
