if Game.getClientVersion().min < 792 then
	return
end

local partyHat = Action()

function partyHat.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local headSlotItem = player:getSlotItem(CONST_SLOT_HEAD)
	if not headSlotItem or item.uid ~= headSlotItem:getUniqueId() then
		return false
	end
	player:addAchievementProgress("Party Animal", 200)
	player:getPosition():sendMagicEffect(CONST_ME_GIFT_WRAPS)
	return true
end

partyHat:id(6578)
partyHat:register()
