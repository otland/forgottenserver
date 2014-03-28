function onSay(cid, words, param)
	local player = Player(cid)
	if not player:getGroup():getAccess() then
		return true
	end

	if player:getAccountType() < ACCOUNT_TYPE_GOD then
		return false
	end

	local orig = player:getPosition()
	local creatureId = doSummonCreature(param, orig)
	if creatureId ~= false then
		local monster = Monster(creatureId)
		monster:setMaster(player)
		orig:sendMagicEffect(CONST_ME_MAGIC_RED)
	else
		player:sendTextMessage(MESSAGE_STATUS_SMALL, "There is not enough room.")
		orig:sendMagicEffect(CONST_ME_POFF)
	end
	return false
end
