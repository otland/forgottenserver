local items = {
	[11260] = {name = "Spiritual Shielding", text = "The Spiritual Shielding protects you.", id = BLESSING_SPIRITUAL_SHIELDING, effect = CONST_ME_LOSEENERGY},
	[11259] = {name = "Embrace of the World", text = "The Embrace of the World surrounds you.", id = BLESSING_EMBRACE_OF_THE_WORLD, effect = CONST_ME_MAGIC_BLUE},
	[11261] = {name = "Fire of the Suns", text = "The Fire of the Suns engulfs you.", id = BLESSING_FIRE_OF_THE_SUNS, effect = CONST_ME_MAGIC_RED},
	[11262] = {name = "Wisdom of Solitude", text = "The Wisdom of Solitude inspires you.", id = BLESSING_WISDOM_OF_SOLITUDE, effect = CONST_ME_MAGIC_GREEN},
	[11258] = {name = "Spark of the Phoenix", text = "The Spark of the Phoenix emblazes you.", id = BLESSING_SPARK_OF_THE_PHOENIX, effect = CONST_ME_FIREATTACK},
	[28016] = {name = "Heart of the Mountain", text = "The Heart of the Mountain encourages you.", id = BLESSING_HEART_OF_THE_MOUNTAIN, effect = CONST_ME_STONESSINGLESPACE},
	[28017] = {name = "Blood of the Mountain", text = "The Blood of the Mountain strenghtens you.", id = BLESSING_BLOOD_OF_THE_MOUNTAIN, effect = CONST_ME_DRAWBLOOD}
}

local blessingCharms = Action()

function blessingCharms.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local blessItem = items[item.itemid]
	if blessItem then
		if player:hasBlessing(blessItem.id) then
			player:say("You already possess this blessing.", TALKTYPE_MONSTER_SAY)
			return true
		end
		player:addBlessingsHistory("\"" .. blessItem.name .. "\" gained from using a blessing charm.", 1)
		player:addBlessing(blessItem.id)
		player:say(blessItem.text, TALKTYPE_MONSTER_SAY)
		player:getPosition():sendMagicEffect(blessItem.effect)
		player:sendSupplyUsed(item)
		item:remove(1)
	end
	return true
end

for k, v in pairs(items) do
	blessingCharms:id(k)
end
blessingCharms:register()
