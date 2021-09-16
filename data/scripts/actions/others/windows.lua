local windows = {
	--[itemid] = {toItemid}
	[5303] = {6448}, -- white stone wall window
	[5304] = {6449}, -- white stone wall window
	[6438] = {6436}, -- framework window
	[6436] = {6438}, -- framework window
	[6439] = {6437}, -- framework window
	[6437] = {6439}, -- framework window
	[6442] = {6440}, -- brick window
	[6440] = {6442}, -- brick window
	[6443] = {6441}, -- brick window
	[6441] = {6443}, -- brick window
	[6446] = {6444}, -- stone window
	[6444] = {6446}, -- stone window
	[6447] = {6445}, -- stone window
	[6445] = {6447}, -- stone window
	[6448] = {5303}, -- marble window
	[6449] = {5304}, -- marble window
	[6452] = {6450}, -- tree window
	[6450] = {6452}, -- tree window
	[6453] = {6451}, -- tree window
	[6451] = {6453}, -- tree window
	[6456] = {6454}, -- sandstone window
	[6454] = {6456}, -- sandstone window
	[6457] = {6455}, -- sandstone window
	[6455] = {6457}, -- sandstone window
	[6460] = {6458}, -- bamboo window
	[6458] = {6460}, -- bamboo window
	[6461] = {6459}, -- bamboo window
	[6459] = {6461}, -- bamboo window
	[6464] = {6462}, -- sandstone window
	[6462] = {6464}, -- sandstone window
	[6465] = {6463}, -- sandstone window
	[6463] = {6465}, -- sandstone window
	[6468] = {6466}, -- stone window
	[6466] = {6468}, -- stone window
	[6469] = {6467}, -- stone window
	[6467] = {6469}, -- stone window
	[6472] = {6470}, -- wooden window
	[6470] = {6472}, -- wooden window
	[6473] = {6471}, -- wooden window
	[6471] = {6473}, -- wooden window
	[6790] = {6788}, -- fur wall window
	[6788] = {6790}, -- fur wall window
	[6791] = {6789}, -- fur wall window
	[6789] = {6791}, -- fur wall window
	[7027] = {7025}, -- nordic wall window
	[7025] = {7027}, -- nordic wall window
	[7028] = {7026}, -- nordic wall window
	[7026] = {7028}, -- nordic wall window
	[7031] = {7029}, -- ice wall window
	[7029] = {7031}, -- ice wall window
	[7032] = {7030}, -- ice wall window
	[7030] = {7032}, -- ice wall window
	[10264] = {10266}, -- framework window
	[10266] = {10264}, -- framework window
	[10265] = {10267}, -- framework window
	[10267] = {10265}, -- framework window
	[10488] = {10490}, -- limestone window
	[10490] = {10488}, -- limestone window
	[10489] = {10491}, -- limestone window
	[10491] = {10489}, -- limestone window
	[19427] = {19447}, -- window
	[19428] = {19448}, -- window
	[19441] = {19450}, -- window
	[19440] = {19449}, -- window
	[19443] = {20180}, -- small window
	[19444] = {20181}, -- small window
	[19445] = {20183}, -- small window
	[19446] = {20184}, -- small window
	[19447] = {19427}, -- window
	[19448] = {19428}, -- window
	[19449] = {19440}, -- window
	[19450] = {19441}, -- window
	[19974] = {20182}, -- window
	[19975] = {20185}, -- window
	[20180] = {19443}, -- window
	[20181] = {19444}, -- small window
	[20182] = {19974}, -- window
	[20183] = {19445}, -- window
	[20184] = {19446}, -- small window
	[20185] = {19975}, -- window
	[22813] = {22839}, -- window
	[22839] = {22813}, -- window
	[22812] = {22838}, -- window
	[22838] = {22812}, -- window
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

for k, v in pairs(windows) do
	window:id(k)
end
window:register()
