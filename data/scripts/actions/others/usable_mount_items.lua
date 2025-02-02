local config = {
	[26194] = { -- vibrant egg
		name = "vortexion",
		mountId = 94,
		tameMessage = "You receive the permission to ride a sparkion.",
		achievementProgress = { name = "Vortex Tamer", progress = 3 }
	},
	[26340] = { -- crackling egg
		name = "neon sparkid",
		mountId = 98,
		tameMessage = "You receive the permission to ride a neon sparkid.",
		achievementProgress = { name = "Vortex Tamer", progress = 3 }
	},
	[26341] = { -- menacing egg
		name = "vortexion",
		mountId = 99,
		tameMessage = "You receive the permission to ride a vortexion.",
		achievementProgress = { name = "Vortex Tamer", progress = 3 }
	},
	[25521] = { -- mysterious scroll
		name = "rift runner",
		mountId = 87,
		achievement = "Running the Rift",
		tameMessage = "You receive the permission to ride a rift runner."
	},
	[35285] = { -- spectral scrap of cloth
		name = "haze",
		mountId = 162,
		achievement = "Nothing but Hot Air",
		tameMessage = "You are now versed to ride the haze!"
	}
}

local usableItemMounts = Action()

function usableItemMounts.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local useItem = config[item.itemid]
	if not useItem then
		return true
	end

	if not player:isPremium() then
		player:sendCancelMessage(RETURNVALUE_YOUNEEDPREMIUMACCOUNT)
		return true
	end

	if player:hasMount(useItem.mountId) then
		return true
	end

	if useItem.achievementProgress then
		player:addAchievementProgress(useItem.achievementProgress.name, useItem.achievementProgress.progress)
	end

	if useItem.achievement then
		player:addAchievement(useItem.achievement)
	end

	player:addMount(useItem.mountId)
	player:addAchievement("Natural Born Cowboy")
	player:say(useItem.tameMessage, TALKTYPE_MONSTER_SAY)
	item:remove(1)
	return true
end

for itemId in pairs(config) do
	usableItemMounts:id(itemId)
end

usableItemMounts:register()
