local fireBug = Action()

function fireBug.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local chance = math.random(10)
	if chance > 4 then -- Success 6% chance
		if target.itemid == 7538 then -- Destroy spider webs/North - South
			toPosition:sendMagicEffect(CONST_ME_HITBYFIRE)
			target:transform(7544)
			target:decay()
		elseif target.itemid == 7539 then -- Destroy spider webs/East - West
			toPosition:sendMagicEffect(CONST_ME_HITBYFIRE)
			target:transform(7545)
			target:decay()
		elseif target.itemid == 5466 then -- Burn Sugar Cane
			toPosition:sendMagicEffect(CONST_ME_FIREAREA)
			target:transform(5465)
			target:decay()
		elseif target.itemid == 1485 then -- Light up empty coal basins
			toPosition:sendMagicEffect(CONST_ME_HITBYFIRE)
			target:transform(1484)
		end
	elseif chance == 2 then -- It removes the firebug 1% chance
		item:remove(1)
		toPosition:sendMagicEffect(CONST_ME_POFF)
	elseif chance == 1 then -- It explodes on the user 1% chance
		doTargetCombatHealth(0, player, COMBAT_FIREDAMAGE, -5, -5, CONST_ME_HITBYFIRE)
		player:say('OUCH!', TALKTYPE_MONSTER_SAY)
		item:remove(1)
	else
		toPosition:sendMagicEffect(CONST_ME_POFF) -- It fails, but don't get removed 3% chance
	end
	return true
end

fireBug:id(5468)
fireBug:register()
