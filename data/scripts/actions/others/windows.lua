if Game.getClientVersion().min < 780 then
	return
end

local windows = {
	--[itemid] {toItemid, client_version}
	[5303] = {6448, 790}, -- white stone wall window
	[5304] = {6449, 790}, -- white stone wall window
	[6438] = {6436, 790}, -- framework window
	[6436] = {6438, 790}, -- framework window
	[6439] = {6437, 790}, -- framework window
	[6437] = {6439, 790}, -- framework window
	[6442] = {6440, 790}, -- brick window
	[6440] = {6442, 790}, -- brick window
	[6443] = {6441, 790}, -- brick window
	[6441] = {6443, 790}, -- brick window
	[6446] = {6444, 790}, -- stone window
	[6444] = {6446, 790}, -- stone window
	[6447] = {6445, 790}, -- stone window
	[6445] = {6447, 790}, -- stone window
	[6448] = {5303, 790}, -- marble window
	[6449] = {5304, 790}, -- marble window
	[6452] = {6450, 790}, -- tree window
	[6450] = {6452, 790}, -- tree window
	[6453] = {6451, 790}, -- tree window
	[6451] = {6453, 790}, -- tree window
	[6456] = {6454, 790}, -- sandstone window
	[6454] = {6456, 790}, -- sandstone window
	[6457] = {6455, 790}, -- sandstone window
	[6455] = {6457, 790}, -- sandstone window
	[6460] = {6458, 790}, -- bamboo window
	[6458] = {6460, 790}, -- bamboo window
	[6461] = {6459, 790}, -- bamboo window
	[6459] = {6461, 790}, -- bamboo window
	[6464] = {6462, 790}, -- sandstone window
	[6462] = {6464, 790}, -- sandstone window
	[6465] = {6463, 790}, -- sandstone window
	[6463] = {6465, 790}, -- sandstone window
	[6468] = {6466, 790}, -- stone window
	[6466] = {6468, 790}, -- stone window
	[6469] = {6467, 790}, -- stone window
	[6467] = {6469, 790}, -- stone window
	[6472] = {6470, 790}, -- wooden window
	[6470] = {6472, 790}, -- wooden window
	[6473] = {6471, 790}, -- wooden window
	[6471] = {6473, 790}, -- wooden window
	[6790] = {6788, 800}, -- fur wall window
	[6788] = {6790, 800}, -- fur wall window
	[6791] = {6789, 800}, -- fur wall window
	[6789] = {6791, 800}, -- fur wall window
	[7027] = {7025, 800}, -- nordic wall window
	[7025] = {7027, 800}, -- nordic wall window
	[7028] = {7026, 800}, -- nordic wall window
	[7026] = {7028, 800}, -- nordic wall window
	[7031] = {7029, 800}, -- ice wall window
	[7029] = {7031, 800}, -- ice wall window
	[7032] = {7030, 800}, -- ice wall window
	[7030] = {7032, 800}, -- ice wall window
	[10264] = {10266, 850}, -- framework window
	[10266] = {10264, 850}, -- framework window
	[10265] = {10267, 850}, -- framework window
	[10267] = {10265, 850}, -- framework window
	[10488] = {10490, 850}, -- limestone window
	[10490] = {10488, 850}, -- limestone window
	[10489] = {10491, 850}, -- limestone window
	[10491] = {10489, 850}, -- limestone window
	[19427] = {19447, 980}, -- window
	[19428] = {19448, 980}, -- window
	[19441] = {19450, 980}, -- window
	[19440] = {19449, 980}, -- window
	[19443] = {20180, 980}, -- small window
	[19444] = {20181, 980}, -- small window
	[19445] = {20183, 980}, -- small window
	[19446] = {20184, 980}, -- small window
	[19447] = {19427, 980}, -- window
	[19448] = {19428, 980}, -- window
	[19449] = {19440, 980}, -- window
	[19450] = {19441, 980}, -- window
	[19974] = {20182, 980}, -- window
	[19975] = {20185, 980}, -- window
	[20180] = {19443, 980}, -- window
	[20181] = {19444, 980}, -- small window
	[20182] = {19974, 980}, -- window
	[20183] = {19445, 980}, -- window
	[20184] = {19446, 980}, -- small window
	[20185] = {19975, 980} -- window
}

local window = Action()

function window.onUse(player, item, fromPosition, target, toPosition, isHotkey)
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
	item:transform(window[1])
	return true
end

for i, w in pairs(windows) do
	if Game.getClientVersion().min >= w[2] then
		window:id(i)
	end
end
window:register()
