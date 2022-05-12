local transformDisplay = {
	[34165] = 34156,
	[34156] = 34161,
	[34161] = 34169,
	[34169] = 34165,
	[34166] = 34158,
	[34158] = 34162,
	[34162] = 34170,
	[34170] = 34166,
	[34167] = 34159,
	[34159] = 34163,
	[34163] = 34171,
	[34171] = 34167,
	[34168] = 34160,
	[34160] = 34164,
	[34164] = 34172,
	[34172] = 34168
}

local goldenOutfitDisplay = Action()

function goldenOutfitDisplay.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local transformIds = transformDisplay[item:getId()]
	if not transformIds then
		return false
	end

	if player:getStorageValue(PlayerStorageKeys.goldenOutfit) == 3 then
		item:transform(transformIds)
		item:getPosition():sendMagicEffect(CONST_ME_EARLY_THUNDER)
	else
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You need Full Golden Outfit to use it.")
		item:getPosition():sendMagicEffect(CONST_ME_POFF)
	end
	return true
end

for index, value in pairs(transformDisplay) do
	goldenOutfitDisplay:id(index)
end

goldenOutfitDisplay:register()
