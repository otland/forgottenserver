local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_RED)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, 0)

function onCastSpell(creature, var)
	local item = creature:getSlotItem(CONST_SLOT_LEFT)
	if not item or item:getId() ~= 2401 then
		item = creature:getSlotItem(CONST_SLOT_RIGHT)
	end

	if not item or item:getId() ~= 2401 then
		creature:getPosition():sendMagicEffect(CONST_ME_POFF)
		creature:sendTextMessage(MESSAGE_STATUS_SMALL, "You need a magic item to cast this spell.")
		return false
	end

	item:transform(2433)
	item:decay()
	return combat:execute(creature, var)
end
