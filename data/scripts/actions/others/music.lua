local instruments = {
	[2948] = {effect = CONST_ME_SOUND_GREEN}, -- wooden flute
	[2949] = {effect = CONST_ME_SOUND_GREEN}, -- lyre
	[2950] = {effect = CONST_ME_SOUND_GREEN}, -- lute
	[2951] = {effect = CONST_ME_SOUND_GREEN}, -- drum
	[2952] = {effect = CONST_ME_SOUND_GREEN}, -- panpipes
	[2953] = {effect = CONST_ME_SOUND_GREEN}, -- simple fanfare
	[2954] = {effect = CONST_ME_SOUND_GREEN}, -- fanfare
	[2955] = {effect = CONST_ME_SOUND_GREEN}, -- royal fanfare
	[2956] = {effect = CONST_ME_SOUND_GREEN}, -- post horn
	[2957] = {effect = CONST_ME_SOUND_GREEN}, -- war horn
	[2958] = {effects = {failure = CONST_ME_SOUND_PURPLE, success = CONST_ME_SOUND_GREEN}, chance = 50}, -- piano
	[2959] = {effects = {failure = CONST_ME_SOUND_PURPLE, success = CONST_ME_SOUND_GREEN}, chance = 50}, -- piano
	[2961] = {effects = {failure = CONST_ME_SOUND_PURPLE, success = CONST_ME_SOUND_GREEN}, chance = 50}, -- piano
	[2962] = {effects = {failure = CONST_ME_SOUND_PURPLE, success = CONST_ME_SOUND_GREEN}, chance = 50}, -- piano
	[2963] = {effect = CONST_ME_SOUND_GREEN}, -- harp
	[2964] = {effect = CONST_ME_SOUND_GREEN}, -- harp
	[3219] = {effect = CONST_ME_SOUND_GREEN}, -- Waldo's post horn
	[3255] = {effect = CONST_ME_SOUND_GREEN}, -- drum (immovable)
	[3256] = {effect = CONST_ME_SOUND_GREEN}, -- simple fanfare (immovable)
	[3257] = {effect = CONST_ME_SOUND_YELLOW, itemId = 3592, itemCount = 10, chance = 80, remove = true}, -- cornucopia (immovable)
	[3258] = {effect = CONST_ME_SOUND_GREEN}, -- lute (immovable)
	[3259] = {effect = CONST_ME_SOUND_BLUE}, -- the horn of sundering (actual effect is unknown; immovable)
	[3260] = {effect = CONST_ME_SOUND_GREEN}, -- lyre (immovable)
	[3261] = {effect = CONST_ME_SOUND_GREEN}, -- panpipes (immovable)
	[2951] = {effect = CONST_ME_SOUND_BLUE}, -- bongo drum (actual effect is unknown)
	[2965] = {effects = {failure = CONST_ME_POFF, success = CONST_ME_SOUND_GREEN}, chance = 20}, -- didgeridoo
	[2966] = {effect = CONST_ME_SOUND_RED}, -- war drum
	[3103] = {effect = CONST_ME_SOUND_YELLOW, itemId = 3592, itemCount = 10, chance = 80, remove = true}, -- cornucopia
	[5786] = {effects = {failure = CONST_ME_SOUND_RED, success = CONST_ME_SOUND_YELLOW}, monster = "war wolf", chance = 60, remove = true}, -- wooden whistle
	[6572] = {effect = CONST_ME_SOUND_GREEN, text = "TOOOOOOT", transformId = 6573, decayId = 6572}, -- party trumpet
	[6573] = {effect = CONST_ME_SOUND_GREEN, text = "TOOOOOOT", transformId = 6572, decayId = 6573}, -- party trumpet
	[12602] = {effect = CONST_ME_SOUND_BLUE}, -- small whistle (actual effect is unknown)
	[23725] = {effect = CONST_ME_SOUND_WHITE} -- small crystal bell
}

local music = Action()

function music.onUse(player, item, fromPosition, target, toPosition, isHotkey)
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

for k, v in pairs(instruments) do
	music:id(k)
end

music:register()
