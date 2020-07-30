function onCastSpell(creature, var)
	local combat = Combat()
	combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_RED)
	local condition = Condition(CONDITION_PARALYZE)
	condition:setParameter(CONDITION_PARAM_TICKS, 20000)

	if creature:isPlayer() then
		local target = Creature(var.number)
		if not target or not target:isPlayer() then
			condition:setFormula(-0.55, 0, -0.75, 0)
		else
			local speed = target:getBaseSpeed() - 80
			if (speed - 80) <= 0 then
				speed = speed + 80
			end

			condition:setParameter(CONDITION_PARAM_SPEED, -speed)
		end
	else
		condition:setFormula(-0.55, 0, -0.75, 0)
	end

	combat:addCondition(condition)
	if not combat:execute(creature, var) then
		return false
	end

	creature:getPosition():sendMagicEffect(CONST_ME_MAGIC_GREEN)
	return true
end
