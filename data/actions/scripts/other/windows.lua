local windows = {
	[5303] = 6448, [5304] = 6449, [6438] = 6436, [6436] = 6438,
	[6439] = 6437, [6437] = 6439, [6442] = 6440, [6440] = 6442,
	[6443] = 6441, [6441] = 6443, [6446] = 6444, [6444] = 6446,
	[6447] = 6445, [6445] = 6447, [6448] = 5303, [6449] = 5304,
	[6452] = 6450, [6450] = 6452, [6453] = 6451, [6451] = 6453,
	[6456] = 6454, [6454] = 6456, [6457] = 6455, [6455] = 6457,
	[6460] = 6458, [6458] = 6460, [6461] = 6459, [6459] = 6461,
	[6464] = 6462, [6462] = 6464, [6465] = 6463, [6463] = 6465,
	[6468] = 6466, [6466] = 6468, [6469] = 6467, [6467] = 6469,
	[6472] = 6470, [6470] = 6472, [6473] = 6471, [6471] = 6473,
	[6790] = 6788, [6788] = 6790, [6791] = 6789, [6789] = 6791,
	[7027] = 7025, [7025] = 7027, [7028] = 7026, [7026] = 7028,
	[7031] = 7029, [7029] = 7031, [7032] = 7030, [7030] = 7032,
	[10264] = 10266, [10266] = 10264, [10265] = 10267, [10267] = 10265,
	[10488] = 10490, [10490] = 10488, [10489] = 10491, [10491] = 10489,
	[19427] = 19447, [19428] = 19448, [19441] = 19450, [19440] = 19449,
	[19443] = 20180, [19444] = 20181, [19445] = 20183, [19446] = 20184,
	[19447] = 19427, [19448] = 19428, [19449] = 19440, [19450] = 19441,
	[19974] = 20182, [19975] = 20185, [20180] = 19443, [20181] = 19444,
	[20182] = 19974, [20183] = 19445, [20184] = 19446, [20185] = 19975
}

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local window = windows[item:getId()]
	if not window then
		return false
	end

	local tile = Tile(fromPosition)
	local house = tile and tile:getHouse()
	if not house then
		fromPosition.y = fromPosition.y - 1
		tile = Tile(fromPosition)
		house = tile and tile:getHouse()
		if not house then
			fromPosition.y = fromPosition.y + 1
			fromPosition.x = fromPosition.x - 1
			tile = Tile(fromPosition)
			house = tile and tile:getHouse()
		end
	end

	if house and player:getTile():getHouse() ~= house and player:getAccountType() < ACCOUNT_TYPE_GAMEMASTER then
		return false
	end

	player:addAchievementProgress("Do Not Disturb", 100)
	player:addAchievementProgress("Let the Sunshine In", 100)
	item:transform(window)
	return true
end
