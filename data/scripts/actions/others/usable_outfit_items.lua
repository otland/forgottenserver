local config = {
	[18517] = { -- gnomish voucher type MB
		female = 514,
		male = 516,
		effect = CONST_ME_GREEN_RINGS
	},
	[18518] = { -- gnomish voucher type MA1
		female = 514,
		male = 516,
		addon = 1,
		effect = CONST_ME_GREEN_RINGS,
		achievement = "Funghitastic"
	},
	[18519] = { -- gnomish voucher type MA2
		female = 514,
		male = 516,
		addon = 2,
		effect = CONST_ME_GREEN_RINGS,
		achievement = "Funghitastic"
	},
	[18520] = { -- gnomish voucher type CB
		female = 513,
		male = 512,
		effect = CONST_ME_GIANTICE
	},
	[18521] = { -- gnomish voucher type CA1
		female = 513,
		male = 512,
		addon = 1,
		effect = CONST_ME_GIANTICE,
		achievement = "Crystal Clear"
	},
	[18522] = { -- gnomish voucher type CA2
		female = 513,
		male = 512,
		addon = 2,
		effect = CONST_ME_GIANTICE,
		achievement = "Crystal Clear"
	},
	[35286] = { -- spooky hood
		female = 1271,
		male = 1270,
		addon = 1,
		effect = CONST_ME_GREEN_RINGS,
		achievement = "Mainstreet Nightmare"
	},
	[35287] = { -- ghost claw
		female = 1271,
		male = 1270,
		addon = 2,
		effect = CONST_ME_GREEN_RINGS,
		achievement = "Mainstreet Nightmare"
	}
}

local usableOutfitItems = Action()

function usableOutfitItems.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local useItem = config[item.itemid]
	local looktype = player:getSex() == PLAYERSEX_FEMALE and useItem.female or useItem.male
	if useItem.addon then
		if not player:isPremium() or not player:hasOutfit(looktype) or player:hasOutfit(looktype, useItem.addon) then
			player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You own no premium account, lack the base outfit or already own this outfit part.")
			return true
		end

		player:addOutfitAddon(useItem.female, useItem.addon)
		player:addOutfitAddon(useItem.male, useItem.addon)
		player:getPosition():sendMagicEffect(useItem.effect)
		if player:hasOutfit(looktype, 3) then
			player:addAchievement(useItem.achievement)
		end

		item:remove(1)
	else
		if not player:isPremium() or player:hasOutfit(looktype) then
			player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You own no premium account or already own this outfit part.")
			return true
		end

		player:addOutfit(useItem.female)
		player:addOutfit(useItem.male)
		player:getPosition():sendMagicEffect(useItem.effect)
		item:remove(1)
	end
	return true
end

for k, v in pairs(config) do
	usableOutfitItems:id(k)
end

usableOutfitItems:register()
