local config = {
	["dragonling"] = {
		mountId = 31,
		achievement = "Dragon Mimicry",
		tameMessage = "The wild dragonling will accompany you as a friend from now on.",
		sound = "FI?"
	},
	["draptor"] = {
		mountId = 6,
		achievement = "Scales and Tail",
		tameMessage = "The wild draptor will accompany you as a friend from now on.",
		sound = "Screeeeeeeeech"
	},
	["enraged white deer"] = {
		mountId = 18,
		achievement = "Friend of Elves",
		tameMessage = "The wild deer will accompany you as a friend from now on.",
		sound = "*bell*"
	},
	["ironblight"] = {
		mountId = 29,
		achievement = "Magnetised",
		tameMessage = "The ironblight will accompany you as a friend from now on.",
		sound = "Plinngggg"
	},
	["magma crawler"] = {
		mountId = 30,
		achievement = "Way to Hell",
		tameMessage = "The magma crawler will accompany you as a friend from now on.",
		sound = "ZzzZzzZzzzZz"
	},
	["midnight panther"] = {
		mountId = 5,
		achievement = "Starless Night",
		tameMessage = "The wild panther will accompany you as a friend from now on.",
		sound = "Purrrrrrr"
	},
	["wailing widow"] = {
		mountId = 1,
		achievement = "Spin-Off",
		tameMessage = "You have tamed the wailing widow.",
		sound = "Sssssssss"
	},
	["wild horse"] = {
		mountId = 17,
		achievement = "Lucky Horseshoe",
		tameMessage = "The horse accepts you as its new master.",
		sound = "*snort*"
	},
	["panda"] = {
		mountId = 19,
		achievement = "Chequered Teddy",
		tameMessage = "The panda will accompany you as a friend from now on.",
		sound = "Rrrrr... Grrrr"
	}
}

local musicBox = Action()

function musicBox.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if not target:isCreature() or not target:isMonster() or target:getMaster() then
		return false
	end

	local monsterConfig = config[target:getName():lower()]
	if not monsterConfig then
		return false
	end

	if player:hasMount(monsterConfig.mountId) then
		return false
	end

	player:addMount(monsterConfig.mountId)
	player:addAchievement("Natural Born Cowboy")
	player:addAchievement(monsterConfig.achievement)
	player:say(monsterConfig.tameMessage, TALKTYPE_MONSTER_SAY)
	toPosition:sendMagicEffect(CONST_ME_SOUND_RED)

	target:say(monsterConfig.sound, TALKTYPE_MONSTER_SAY)
	target:remove()

	item:remove(1)
	return true
end

musicBox:id(18511)
musicBox:register()
