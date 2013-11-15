function onSay(cid, words, param)
	local player = Player(cid)
	if not player:getGroup():getAccess() then
		return true
	end

	local orig = player:getPosition()
	local creatureId = doSummonCreature(param, orig)
	if creatureId ~= false then
		local monster = Monster(creatureId)
		monster:getPosition():sendMagicEffect(CONST_ME_TELEPORT)
		orig:sendMagicEffect(CONST_ME_MAGIC_RED)
	else
		player:sendCancelMessage("There is not enough room.")
		orig:sendMagicEffect(CONST_ME_POFF)
	end
	return false
end
