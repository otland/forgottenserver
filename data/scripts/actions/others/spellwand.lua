local monsters = {"Rat", "Green Frog", "Chicken"}

local spellwand = Action()

function spellwand.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if player:getTile():hasFlag(TILESTATE_PROTECTIONZONE) then
		player:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		return true
	end
	if not target:isPlayer() then
		player:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		return true
	end
	if math.random(100) <= 33 then
		item:remove()
		player:say("The spellwand broke.", TALKTYPE_MONSTER_SAY)
		if math.random(100) <= 75 then
			Game.createMonster("Mad Sheep", fromPosition)
		end
	else
		target:setMonsterOutfit(monsters[math.random(#monsters)], 60 * 1000)
		target:getPosition():sendMagicEffect(CONST_ME_MAGIC_GREEN)
		return true
	end
	return true
end

spellwand:id(7735)
spellwand:allowFarUse(true)
spellwand:register()
