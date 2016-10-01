local instruments = {
	[2070] = {effect = CONST_ME_SOUND_GREEN}, -- wooden flute
	[2071] = {effect = CONST_ME_SOUND_GREEN}, -- lyre
	[2072] = {effect = CONST_ME_SOUND_GREEN}, -- lute
	[2073] = {effect = CONST_ME_SOUND_GREEN}, -- drum
	[2074] = {effect = CONST_ME_SOUND_GREEN}, -- panpipes
	[2075] = {effect = CONST_ME_SOUND_GREEN}, -- simple fanfare
	[2076] = {effect = CONST_ME_SOUND_GREEN}, -- fanfare
	[2077] = {effect = CONST_ME_SOUND_GREEN}, -- royal fanfare (actual effect is unknown)
	[2078] = {effect = CONST_ME_SOUND_GREEN}, -- post horn
	[2079] = {effect = CONST_ME_SOUND_GREEN}, -- war horn
	[2080] = {effect = CONST_ME_SOUND_BLUE}, -- piano (actual effect is unknown)
	[2081] = {effect = CONST_ME_SOUND_BLUE}, -- piano
	[2082] = {effect = CONST_ME_SOUND_BLUE}, -- piano
	[2083] = {effect = CONST_ME_SOUND_BLUE}, -- piano
	[2084] = {effect = CONST_ME_SOUND_BLUE}, -- harp (actual effect is unknown)
	[2085] = {effect = CONST_ME_SOUND_BLUE}, -- harp
	[2332] = {effect = CONST_ME_SOUND_BLUE}, -- Waldo's post horn (actual effect is unknown)
	[2367] = {effect = CONST_ME_SOUND_GREEN}, -- drum
	[2368] = {effect = CONST_ME_SOUND_GREEN}, -- simple fanfare
	[2369] = {effect = CONST_ME_SOUND_YELLOW, item = {id = 2681, count = 10}, chance = 80, remove = true}, -- cornucopia
	[2370] = {effect = CONST_ME_SOUND_GREEN}, -- lute
	[2371] = {effect = CONST_ME_SOUND_BLUE}, -- the horn of sundering (actual effect is unknown)
	[2372] = {effect = CONST_ME_SOUND_GREEN}, -- lyre
	[2373] = {effect = CONST_ME_SOUND_GREEN}, -- panpipes
	[3951] = {effect = CONST_ME_SOUND_BLUE}, -- bongo drum (actual effect is unknown)
	[3952] = {effects = {failure = CONST_ME_POFF, success = CONST_ME_SOUND_GREEN}, chance = 20}, -- didgeridoo
	[3953] = {effect = CONST_ME_SOUND_RED}, -- war drum
	[3957] = {effect = CONST_ME_SOUND_YELLOW, item = {id = 2681, count = 10}, chance = 80, remove = true}, -- cornucopia
	[5786] = {effects = {failure = CONST_ME_SOUND_RED, success = CONST_ME_SOUND_YELLOW}, monster = "war wolf", chance = 60, remove = true}, -- wooden whistle
	[6572] = {effect = CONST_ME_SOUND_GREEN, text = "TOOOOOOT", transformId = 13578}, -- party trumpet
	[6573] = {effect = CONST_ME_SOUND_GREEN, text = "TOOOOOOT", transformId = 13578}, -- party trumpet
	[13759] = {effect = CONST_ME_SOUND_BLUE} -- small whistle (actual effect is unknown)
}

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local instrument, effect, chance = instruments[item:getId()]
	if instrument.chance then
		chance = instrument.chance > math.random(0, 100)
		if instrument.monster and chance then
			local position = player:getPosition()
			local monster = Game.createMonster(instrument.monster, Position(
				position.x - math.random(-1, 1),
				position.y - math.random(-1, 1),
				position.z
			), false, true)
			
			if monster then
				monster:setMaster(player)
			end
		end

		if instrument.item and chance then
			player:addItem(instrument.item.id, instrument.item.count)
		end

		effect = instrument.effect or (chance and instrument.effects.success or instrument.effects.failure)
	end

	if instrument.transformId then
		player:say(instrument.text, TALKTYPE_MONSTER_SAY, false, nil, item:getPosition())
		item:transform(instrument.transformId)
		item:decay()
	end

	item:getPosition():sendMagicEffect(instrument.effect or effect)

	if not chance and instrument.remove then
		item:remove(1)
	end
	return true
end
