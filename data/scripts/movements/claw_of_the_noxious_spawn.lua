local clawOfTheNoxiousSpawnE = MoveEvent()

function clawOfTheNoxiousSpawnE.onEquip(player, item, slot, isCheck)
	item:transform(10309)
	if Tile(player:getPosition()):hasFlag(TILESTATE_PROTECTIONZONE) then
		return true
	end

	doTargetCombat(0, player, COMBAT_PHYSICALDAMAGE, -150, -200, CONST_ME_DRAWBLOOD)
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Ouch! The serpent claw stabbed you.")
	return true
end

clawOfTheNoxiousSpawnE:type("equip")
clawOfTheNoxiousSpawnE:slot("ring")
clawOfTheNoxiousSpawnE:id(10310)
clawOfTheNoxiousSpawnE:register()

local clawOfTheNoxiousSpawnD = MoveEvent()

function clawOfTheNoxiousSpawnD.onDeEquip(player, item, slot, isCheck)
	item:transform(10310)
	return true
end

clawOfTheNoxiousSpawnD:type("deequip")
clawOfTheNoxiousSpawnD:slot("ring")
clawOfTheNoxiousSpawnD:id(10309)
clawOfTheNoxiousSpawnD:register()
