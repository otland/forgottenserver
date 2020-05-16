local config = {
	[26340] = { -- crackling egg
		name = "neon sparkid",
		mountId = 98,
		tameMessage = "You receive the permission to ride a neon sparkid."
	},
	[26341] = { -- menacing egg
		name = "vortexion",
		mountId = 99,
		tameMessage = "You receive the permission to ride a vortexion."
	},
	[25521] = { -- mysterious scroll
		name = "rift runner",
		mountId = 87,
		tameMessage = "You receive the permission to ride a rift runner."
	}
}

local usableItemMounts = Action()

function usableItemMounts.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if not player:isPremium() then
		player:sendCancelMessage(RETURNVALUE_YOUNEEDPREMIUMACCOUNT)
		return true
	end

	local useItem = config[item.itemid]
	if player:hasMount(useItem.mountId) then
		return false
	end

	player:addMount(useItem.mountId)
	player:say(useItem.tameMessage, TALKTYPE_MONSTER_SAY)
	item:remove(1)
	return true
end

for k, v in pairs(config) do
	usableItemMounts:id(k)
end
usableItemMounts:register()
