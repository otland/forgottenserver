local transformID = {
	[25393] = 25392, [25392] = 25393, -- rift carpet
	[26193] = 26192, [26192] = 26193, -- void carpet
	[26087] = 26109, [26109] = 26087, -- yalaharian carpet
	[26088] = 26110, [26110] = 26088, -- white fur carpet
	[26089] = 26111, [26111] = 26089, -- bamboo mat carpet
	[26371] = 26363, [26363] = 26371, -- crimson carpet
	[26366] = 26372, [26372] = 26366, -- azure carpet
	[26367] = 26373, [26373] = 26367, -- emerald carpet
	[26368] = 26374, [26374] = 26368, -- light parquet carpet
	[26369] = 26375, [26375] = 26369, -- dark parquet carpet
	[26370] = 26376, [26376] = 26370, -- marble floor
	[27072] = 27080, [27080] = 27072, -- colourful carpet
	[27073] = 27081, [27081] = 27073, -- flowery carpet
	[27074] = 27082, [27082] = 27074, -- striped carpet
	[27075] = 27083, [27083] = 27075, -- fur carpet
	[27076] = 27084, [27084] = 27076, -- diamond carpet
	[27077] = 27085, [27085] = 27077, -- patterned carpet
	[27078] = 27086, [27086] = 27078, -- night sky carpet
	[27079] = 27087, [27087] = 27079, -- star carpet
	[28770] = 28771, [28771] = 28770, -- verdant carpet
	[28772] = 28773, [28773] = 28772, -- shaggy carpet
	[28774] = 28775, [28775] = 28774, -- mystic carpet
	[28776] = 28777, [28777] = 28776, -- stone tiles
	[28778] = 28779, [28779] = 28778, -- wooden planks
	[28806] = 28807, [28807] = 28806, -- wheat carpet
	[28808] = 28809, [28809] = 28808, -- crested carpet
	[28810] = 28811, [28811] = 28810, -- decorated carpet
	[34122] = 34124, [34124] = 34122, -- tournament carpet
	[34123] = 34125, [34125] = 34123, -- sublime tournament carpet
	[38543] = 38544, [38544] = 38543, -- lilac carpet
	[38545] = 38546, [38546] = 38545, -- colourful pom-pom carpet
	[38547] = 38548, [38548] = 38547, -- natural pom-pom carpet
	[38549] = 38550, [38550] = 38549, -- owin rug
	[38551] = 38552, [38552] = 38551, -- midnight panther rug
	[38553] = 38554, [38554] = 38553, -- moon carpet
	[38555] = 38556, [38556] = 38555, -- romantic carpet
	[38597] = 38598, [38598] = 38597, -- grandiose carpet
	[39152] = 39494, [39494] = 39152, -- eldritch carpet
	[40505] = 40506, [40506] = 40505, -- dragon lord carpet
	[40507] = 40509, [40509] = 40507, -- dragon carpet
	[40508] = 40510, [40510] = 40508, -- fire elemental carpet
	[40511] = 40512, [40512] = 40511, -- Morgaroth carpet
	[40513] = 40514, [40514] = 40513, -- Ghazbaran carpet
	[40515] = 40516, [40516] = 40515, -- Orshabaal carpet
	[40517] = 40518, [40518] = 40517, -- red cake carpet
	[40519] = 40526, [40526] = 40519, -- orange cake carpet
	[40520] = 40527, [40527] = 40520, -- yellow cake carpet
	[40521] = 40528, [40528] = 40521, -- green cake carpet
	[40522] = 40529, [40529] = 40522, -- sky cake carpet
	[40523] = 40530, [40530] = 40523, -- blue cake carpet
	[40524] = 40531, [40531] = 40524, -- purple cake carpet
	[40525] = 40532, [40532] = 40525, -- pink cake carpet
	[40533] = 40534, [40534] = 40533, -- red t carpet
	[40535] = 40542, [40542] = 40535, -- orange t carpet
	[40536] = 40543, [40543] = 40536, -- yellow t carpet
	[40537] = 40544, [40544] = 40537, -- green t carpet
	[40538] = 40545, [40545] = 40538, -- sky t carpet
	[40539] = 40546, [40546] = 40539, -- blue t carpet
	[40540] = 40547, [40547] = 40540, -- purple t carpet
	[40541] = 40548, [40548] = 40541, -- pink t carpet
	[40911] = 40912, [40912] = 40911, -- Zaoan bamboo tiles
	[40913] = 40914, [40914] = 40913, -- Zaoan bamboo tiles
	[40915] = 40916, [40916] = 40915, -- Zaoan bamboo tiles
	[40917] = 40918, [40918] = 40917, -- Zaoan bamboo tiles
	[40919] = 40920, [40920] = 40919, -- Zaoan bamboo tiles
	[40921] = 40922, [40922] = 40921, -- Zaoan bamboo tiles
}

local carpets = Action()

function carpets.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local carpet = transformID[item.itemid]
	if not carpet then
		return false
	end

	if fromPosition.x == CONTAINER_POSITION then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, "Put the item on the floor first.")
		return true
	end

	local tile = Tile(item:getPosition())
	if not tile:getHouse() then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, "You may use this only inside a house.")
		return true
	end

	if tile:getItemByType(ITEM_TYPE_DOOR) then
		player:sendCancelMessage("You cannot use this item on house doors.")
		return true
	end

	local carpetStack = 0
	for _, carpetId in pairs(transformID) do
		carpetStack = carpetStack + tile:getItemCountById(carpetId)
	end

	if carpetStack > 3 then
		player:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		return true
	end

	item:transform(carpet)
	return true
end

for k, v in pairs(transformID) do
	carpets:id(k)
end
carpets:register()
