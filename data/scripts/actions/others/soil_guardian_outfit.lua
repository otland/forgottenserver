local config = {
	[18517] = {female = 514, male = 516, effect = CONST_ME_GREEN_RINGS}, -- gnomish voucher type MB
	[18518] = {female = 514, male = 516, addon = 1, effect = CONST_ME_GREEN_RINGS}, -- gnomish voucher type MA1
	[18519] = {female = 514, male = 516, addon = 2, effect = CONST_ME_GREEN_RINGS} -- gnomish voucher type MA2
}

local soilGuardian = Action()

function soilGuardian.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local useItem = config[item.itemid]
	if not useItem then
		return true
	end

	local looktype = player:getSex() == PLAYERSEX_FEMALE and useItem.female or useItem.male
	if useItem.addon then
		if not player:isPremium()
				or not player:hasOutfit(looktype)
				or player:hasOutfit(looktype, useItem.addon) then
			player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You own no premium account, lack the base outfit or already own this outfit part.")
			return true
		end
		player:addOutfitAddon(useItem.female, useItem.addon)
		player:addOutfitAddon(useItem.male, useItem.addon)
		player:getPosition():sendMagicEffect(useItem.effect)
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
	soilGuardian:id(k)
end
soilGuardian:register()
