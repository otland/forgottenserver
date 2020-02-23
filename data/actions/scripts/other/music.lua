local instruments = {
	[2070] = {effect = CONST_ME_SOUND_GREEN}, -- wooden flute
	[2071] = {effect = CONST_ME_SOUND_GREEN}, -- lyre
	[2072] = {effect = CONST_ME_SOUND_GREEN}, -- lute
	[2073] = {effect = CONST_ME_SOUND_GREEN}, -- drum
	[2074] = {effect = CONST_ME_SOUND_GREEN}, -- panpipes
	[2075] = {effect = CONST_ME_SOUND_GREEN}, -- simple fanfare
	[2076] = {effect = CONST_ME_SOUND_GREEN}, -- fanfare
	[2077] = {effect = CONST_ME_SOUND_GREEN}, -- royal fanfare
	[2078] = {effect = CONST_ME_SOUND_GREEN}, -- post horn
	[2079] = {effect = CONST_ME_SOUND_GREEN}, -- war horn
	[2080] = {effects = {failure = CONST_ME_SOUND_PURPLE, success = CONST_ME_SOUND_GREEN}, chance = 50}, -- piano
	[2081] = {effects = {failure = CONST_ME_SOUND_PURPLE, success = CONST_ME_SOUND_GREEN}, chance = 50}, -- piano
	[2082] = {effects = {failure = CONST_ME_SOUND_PURPLE, success = CONST_ME_SOUND_GREEN}, chance = 50}, -- piano
	[2083] = {effects = {failure = CONST_ME_SOUND_PURPLE, success = CONST_ME_SOUND_GREEN}, chance = 50}, -- piano
	[2084] = {effect = CONST_ME_SOUND_GREEN}, -- harp
	[2085] = {effect = CONST_ME_SOUND_GREEN}, -- harp
	[2332] = {effect = CONST_ME_SOUND_GREEN}, -- Waldo's post horn
	[2367] = {effect = CONST_ME_SOUND_GREEN}, -- drum (immovable)
	[2368] = {effect = CONST_ME_SOUND_GREEN}, -- simple fanfare (immovable)
	[2369] = {effect = CONST_ME_SOUND_YELLOW, itemId = 2681, itemCount = 10, chance = 80, remove = true}, -- cornucopia (immovable)
	[2370] = {effect = CONST_ME_SOUND_GREEN}, -- lute (immovable)
	[2371] = {effect = CONST_ME_SOUND_BLUE}, -- the horn of sundering (actual effect is unknown; immovable)
	[2372] = {effect = CONST_ME_SOUND_GREEN}, -- lyre (immovable)
	[2373] = {effect = CONST_ME_SOUND_GREEN}, -- panpipes (immovable)
	[3951] = {effect = CONST_ME_SOUND_BLUE}, -- bongo drum (actual effect is unknown)
	[3952] = {effects = {failure = CONST_ME_POFF, success = CONST_ME_SOUND_GREEN}, chance = 20}, -- didgeridoo
	[3953] = {effect = CONST_ME_SOUND_RED}, -- war drum
	[3957] = {effect = CONST_ME_SOUND_YELLOW, itemId = 2681, itemCount = 10, chance = 80, remove = true}, -- cornucopia
	[5786] = {effects = {failure = CONST_ME_SOUND_RED, success = CONST_ME_SOUND_YELLOW}, monster = "war wolf", chance = 60, remove = true}, -- wooden whistle
	[6572] = {effect = CONST_ME_SOUND_GREEN, text = "TOOOOOOT", transformId = 13578, decayId = 6572}, -- party trumpet
	[6573] = {effect = CONST_ME_SOUND_GREEN, text = "TOOOOOOT", transformId = 13578, decayId = 6573}, -- party trumpet
	[13759] = {effect = CONST_ME_SOUND_BLUE}, -- small whistle (actual effect is unknown)
	[23923] = {effect = CONST_ME_SOUND_WHITE} -- small crystal bell
}

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local instrument, chance = instruments[item:getId()]
	if instrument.chance then
		chance = instrument.chance >= math.random(1, 100)

		if instrument.monster and chance then
			local monster = Game.createMonster(instrument.monster, player:getPosition(), true)
			if monster then
				player:addSummon(monster)
			end
		elseif instrument.itemId and chance then
			player:addItem(instrument.itemId, instrument.itemCount)
		end
	end

	item:getPosition():sendMagicEffect(instrument.effect or instrument.effects and chance and instrument.effects.success or instrument.effects.failure)

	if instrument.transformId then
		player:say(instrument.text, TALKTYPE_MONSTER_SAY, false, nil, item:getPosition())
		item:transform(instrument.transformId)
		item:decay(instrument.decayId)
	end

	if not chance and instrument.remove then
		item:remove()
	end
	return true
end
