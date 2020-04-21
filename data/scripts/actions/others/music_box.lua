local config = {
	["dragonling"] = {
		mountId = 31,
		tameMessage = "The wild dragonling will accompany you as a friend from now on.",
		sound = "FI?"
	},
	["draptor"] = {
		mountId = 6,
		tameMessage = "The wild draptor will accompany you as a friend from now on.",
		sound = "Screeeeeeeeech"
	},
	["enraged white deer"] = {
		mountId = 18,
		tameMessage = "The wild deer will accompany you as a friend from now on.",
		sound = "*bell*"
	},
	["ironblight"] = {
		mountId = 29,
		tameMessage = "The ironblight will accompany you as a friend from now on.",
		sound = "Plinngggg"
	},
	["magma crawler"] = {
		mountId = 30,
		tameMessage = "The magma crawler will accompany you as a friend from now on.",
		sound = "ZzzZzzZzzzZz"
	},
	["midnight panther"] = {
		mountId = 5,
		tameMessage = "The wild panther will accompany you as a friend from now on.",
		sound = "Purrrrrrr"
	},
	["wailing widow"] = {
		mountId = 1,
		tameMessage = "You have tamed the wailing widow.",
		sound = "Sssssssss"
	},
	["wild horse"] = {
		mountId = 17,
		tameMessage = "The horse accepts you as its new master.",
		sound = "*snort*"
	},
	["panda"] = {
		mountId = 19,
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
	player:say(monsterConfig.tameMessage, TALKTYPE_MONSTER_SAY)
	toPosition:sendMagicEffect(CONST_ME_SOUND_RED)

	target:say(monsterConfig.sound, TALKTYPE_MONSTER_SAY)
	target:remove()

	item:remove(1)
	return true
end

musicBox:id(18511)
musicBox:register()
