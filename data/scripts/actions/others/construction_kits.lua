local kits = {
	[2775] = 2374,
	[2776] = 2378,
	[2777] = 2360,
	[2778] = 2382,
	[2779] = 2366,
	[2780] = 2418,
	[2781] = 2422,
	[2782] = 2319,
	[2783] = 2316,
	[2784] = 2315,
	[2785] = 2314,
	[2786] = 2347,
	[2787] = 2348,
	[2788] = 2350,
	[2789] = 2433,
	[2790] = 2441,
	[2791] = 2449,
	[2792] = 2524,
	[2793] = 2523,
	[2794] = 2483,
	[2795] = 2465,
	[2796] = 2976,
	[2797] = 2979,
	[2798] = 2934,
	[2799] = 3485,
	[2800] = 2998,
	[2801] = 2445,
	[2802] = 2025,
	[2803] = 2029,
	[2804] = 2030,
	[2805] = 2904,
	[2806] = 3513,
	[2806] = 3513,
	[2807] = 2959,
	[2808] = 2963,
	[2809] = 2426,
	[2810] = 2352,
	[2811] = 2982,
	[2812] = 2986,
	[5086] = 5046,
	[5087] = 5055,
	[5088] = 5056,
	[6114] = 6111,
	[6115] = 6109,
	[6371] = 6355,
	[6372] = 6370,
	[7864] = 7860,
	[9061] = 9062,
	[10207] = 10208,
	[10209] = 10210,
	[10216] = 10212,
	[10288] = 10286,
	[14741] = 14687,
	[14756] = 14755,
	[17974] = 18015,
	[17975] = 18017,
	[17977] = 18019
}

local constructionKits = Action()

function constructionKits.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local kit = kits[item.itemid]
	if not kit then
		return false
	end

	if fromPosition.x == CONTAINER_POSITION then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, "Put the construction kit on the floor first.")
	elseif not Tile(fromPosition):getHouse() then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, "You may construct this only inside a house.")
	else
		item:transform(kit)
		fromPosition:sendMagicEffect(CONST_ME_POFF)
		player:addAchievementProgress('Interior Decorator', 1000)
	end
	return true
end

for k, v in pairs(kits) do
	constructionKits:id(k)
end

constructionKits:register()
