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
	-- stop if player is not premium
	if not player:isPremium() then
		player:sendCancelMessage("You need a premium account to use this outfit.")
		return true
	end

	local outfitConfig = config[item.itemid]
	local looktype = player:getSex() == PLAYERSEX_FEMALE and outfitConfig.female or outfitConfig.male

	-- If player is missing the outfit
	if not player:hasOutfit(looktype) then
		-- And this is an addon item, then stop
		if outfitConfig.addon then
			player:sendCancelMessage("You need the outfit for this part.")
			return true
		end

		-- Give outfit
		player:addOutfit(outfitConfig.female)
		player:addOutfit(outfitConfig.male)
		player:getPosition():sendMagicEffect(outfitConfig.effect)
		item:remove(1)
		return true
	end

	-- stop if player already have this addon
	if player:hasOutfit(looktype, outfitConfig.addon) then
		player:sendCancelMessage("You already own this outfit part.")
		return true
	end

	-- Give addon
	player:addOutfitAddon(outfitConfig.female, outfitConfig.addon)
	player:addOutfitAddon(outfitConfig.male, outfitConfig.addon)
	player:getPosition():sendMagicEffect(outfitConfig.effect)
	item:remove(1)

	-- full set achievement check and give
	if player:hasOutfit(looktype, 3) then
		player:addAchievement(outfitConfig.achievement)
	end

	return true
end

for k, v in pairs(config) do
	usableOutfitItems:id(k)
end

usableOutfitItems:register()
