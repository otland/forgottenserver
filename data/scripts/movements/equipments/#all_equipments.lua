--[[ 

Here starts the items that are arranged in order of ids (from smallest to largest).
These are items that have no level requirement, only vocation, will be organized within a single script to shrink the code.

]]

-- mage
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end 
equipment:id(813,818,819,820,827,828,829,830,3210,7992,9653,11674,12599,13996,19366)
equipment:vocation("sorcerer", true, true)
equipment:vocation("druid", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(813,818,819,820,827,828,829,830,3210,7992,9653,11674,12599,13996,19366)
equipment:vocation("sorcerer", true, true)
equipment:vocation("druid", true, true)
equipment:register()

-- paladin
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(3571,8044,8063,8095)
equipment:vocation("paladin", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(3571,8044,8063,8095)
equipment:vocation("paladin", true, true)
equipment:register()

-- knight
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(10385)
equipment:vocation("knight", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(10385)
equipment:vocation("knight", true, true)
equipment:register()

-- knight and paladin
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(3360,3364,3366,3371,3381,3382,3386,21166)
equipment:vocation("knight", true, true)
equipment:vocation("paladin", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(3360,3364,3366,3371,3381,3382,3386,21166)
equipment:vocation("knight", true, true)
equipment:vocation("paladin", true, true)
equipment:register()

--[[ 

Here starts the items that are arranged in order of level (from smallest to largest).

]]

-- mage of level 30
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8072)
equipment:level(30)
equipment:vocation("sorcerer", true, true)
equipment:vocation("druid", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8072)
equipment:level(30)
equipment:vocation("sorcerer", true, true)
equipment:vocation("druid", true, true)
equipment:register()

-- mage of level 40
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(812,821,822,823,8073,21164)
equipment:level(40)
equipment:vocation("druid", true, true)
equipment:vocation("sorcerer", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(812,821,822,823,8073,21164)
equipment:level(40)
equipment:vocation("druid", true, true)
equipment:vocation("sorcerer", true, true)
equipment:register()

-- player of level 50
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(7993)
equipment:level(50)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(7993)
equipment:level(50)
equipment:register()

-- mage of level 50
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(811,824,825,826,8074,9103)
equipment:level(50)
equipment:vocation("druid", true, true)
equipment:vocation("sorcerer", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(811,824,825,826,8074,9103)
equipment:level(50)
equipment:vocation("druid", true, true)
equipment:vocation("sorcerer", true, true)
equipment:register()

-- paladin and knight of level 50
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(10384,21169)
equipment:level(50)
equipment:vocation("paladin", true, true)
equipment:vocation("knight", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(10384,21169)
equipment:level(50)
equipment:vocation("paladin", true, true)
equipment:vocation("knight", true, true)
equipment:register()

-- player of level 60
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(814,815,816,817,22195,25779)
equipment:level(60)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(814,815,816,817,22195,25779)
equipment:level(60)
equipment:register()

-- mage of level 60
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8075,10438,10439,10451)
equipment:level(60)
equipment:vocation("druid", true, true)
equipment:vocation("sorcerer", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8075,10438,10439,10451)
equipment:level(60)
equipment:vocation("druid", true, true)
equipment:vocation("sorcerer", true, true)
equipment:register()

-- paladin of level 60
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(3394)
equipment:level(60)
equipment:vocation("paladin", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(3394)
equipment:level(60)
equipment:vocation("paladin", true, true)
equipment:register()

-- paladin and knight of level 60
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8049,8050,8051,8052)
equipment:level(60)
equipment:vocation("paladin", true, true)
equipment:vocation("knight", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8049,8050,8051,8052)
equipment:level(60)
equipment:vocation("paladin", true, true)
equipment:vocation("knight", true, true)
equipment:register()

-- sorcerer of level 65
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8037)
equipment:level(65)
equipment:vocation("sorcerer", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8037)
equipment:level(65)
equipment:vocation("sorcerer", true, true)
equipment:register()

-- mage of level 70
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8076,21165)
equipment:level(70)
equipment:vocation("sorcerer", true, true)
equipment:vocation("druid", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8076,21165)
equipment:level(70)
equipment:vocation("sorcerer", true, true)
equipment:vocation("druid", true, true)
equipment:register()

-- paladin and knight of level 70
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(10200,10201,10323)
equipment:level(70)
equipment:vocation("paladin", true, true)
equipment:vocation("knight", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(10200,10201,10323)
equipment:level(70)
equipment:vocation("paladin", true, true)
equipment:vocation("knight", true, true)
equipment:register()

-- player of level 75
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(14087,21170)
equipment:level(75)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(14087,21170)
equipment:level(75)
equipment:register()

-- druid of level 75
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8038,8041)
equipment:level(75)
equipment:vocation("druid", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8038,8041)
equipment:level(75)
equipment:vocation("druid", true, true)
equipment:register()

-- sorcerer of level 75
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8039,8040)
equipment:level(75)
equipment:vocation("sorcerer", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8039,8040)
equipment:level(75)
equipment:vocation("sorcerer", true, true)
equipment:register()

-- mage of level 75
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(20088,21183)
equipment:level(75)
equipment:vocation("sorcerer", true, true)
equipment:vocation("druid", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(20088,21183)
equipment:level(75)
equipment:vocation("sorcerer", true, true)
equipment:vocation("druid", true, true)
equipment:register()

-- paladin of level 75
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8057,8058,8059)
equipment:level(75)
equipment:vocation("paladin", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8057,8058,8059)
equipment:level(75)
equipment:vocation("paladin", true, true)
equipment:register()

-- players of level 80
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(9301,9302,9303,9304,21892)
equipment:level(80)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(9301,9302,9303,9304,21892)
equipment:level(80)
equipment:register()

-- mage of level 80
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8090,8864,14086,21981)
equipment:level(80)
equipment:vocation("druid", true, true)
equipment:vocation("sorcerer", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8090,8864,14086,21981)
equipment:level(80)
equipment:vocation("druid", true, true)
equipment:vocation("sorcerer", true, true)
equipment:register()

-- knight of level 80
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8862,11688)
equipment:level(80)
equipment:vocation("knight", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8862,11688)
equipment:level(80)
equipment:vocation("knight", true, true)
equipment:register()

-- for paladin of level 80
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8863)
equipment:level(80)
equipment:vocation("paladin", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8863)
equipment:level(80)
equipment:vocation("paladin", true, true)
equipment:register()

-- paladin and knight of level 80
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(4033)
equipment:level(80)
equipment:vocation("paladin", true, true)
equipment:vocation("knight", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(4033)
equipment:level(80)
equipment:vocation("paladin", true, true)
equipment:vocation("knight", true, true)
equipment:register()

-- players of level 100
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(22062,22773,22774,27565,22767,22768)
equipment:level(100)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(22062,22773,22774,27565,22767,22768)
equipment:level(100)
equipment:register()

-- sorcerer of level 100
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8062)
equipment:level(100)
equipment:vocation("sorcerer", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8062)
equipment:level(100)
equipment:vocation("sorcerer", true, true)
equipment:register()

-- paladin of level 100
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8060,11689,23529,23530)
equipment:level(100)
equipment:vocation("paladin", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8060,11689,23529,23530)
equipment:level(100)
equipment:vocation("paladin", true, true)
equipment:register()

-- knight of level 100
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8053,8054,8055,8056,8077,8078,8079,8080,8081,8889,11686,23533,23534)
equipment:level(100)
equipment:vocation("knight", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(8053,8054,8055,8056,8077,8078,8079,8080,8081,8889,11686,23533,23534)
equipment:level(100)
equipment:vocation("knight", true, true)
equipment:register()

-- mage of level 100
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(11687,11691,23531,23532)
equipment:level(100)
equipment:vocation("druid", true, true)
equipment:vocation("sorcerer", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(11687,11691,23531,23532)
equipment:level(100)
equipment:vocation("druid", true, true)
equipment:vocation("sorcerer", true, true)
equipment:register()

-- knight and paladin of level 100
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(11651)
equipment:level(100)
equipment:vocation("paladin", true, true)
equipment:vocation("knight", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(11651)
equipment:level(100)
equipment:vocation("paladin", true, true)
equipment:vocation("knight", true, true)
equipment:register()

-- players of level 120
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(13990,16114,16264)
equipment:level(120)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(13990,16114,16264)
equipment:level(120)
equipment:register()

-- mage of level 120
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(13998)
equipment:level(120)
equipment:vocation("sorcerer", true, true)
equipment:vocation("druid", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(13998)
equipment:level(120)
equipment:vocation("sorcerer", true, true)
equipment:vocation("druid", true, true)
equipment:register()

-- knight and paladin of level 120
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(16110)
equipment:level(120)
equipment:vocation("paladin", true, true)
equipment:vocation("knight", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(16110)
equipment:level(120)
equipment:vocation("paladin", true, true)
equipment:vocation("knight", true, true)
equipment:register()

-- players of level 130
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(9018,9019)
equipment:level(130)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(9018,9019)
equipment:level(130)
equipment:register()

-- knight of level 130
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(14000)
equipment:level(130)
equipment:vocation("knight", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(14000)
equipment:level(130)
equipment:vocation("knight", true, true)
equipment:register()

-- mage of level 130
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(16107,19391)
equipment:level(130)
equipment:vocation("druid", true, true)
equipment:vocation("sorcerer", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(16107,19391)
equipment:level(130)
equipment:vocation("druid", true, true)
equipment:vocation("sorcerer", true, true)
equipment:register()

-- player of level 150
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(13995,16108,16113,19357,22757,23476,23477)
equipment:level(150)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(13995,16108,16113,19357,22757,23476,23477)
equipment:level(150)
equipment:register()

-- paladin of level 150
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(13994,16111,16112,23526,23542)
equipment:level(150)
equipment:vocation("paladin", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(13994,16111,16112,23526,23542)
equipment:level(150)
equipment:vocation("paladin", true, true)
equipment:register()

-- knight of level 150
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(13997,16109,16116,23528,23544)
equipment:level(150)
equipment:vocation("knight", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(13997,16109,16116,23528,23544)
equipment:level(150)
equipment:vocation("knight", true, true)
equipment:register()

-- mage of level 150
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(14769,16104,16105,16106,20089,22755,23527,23543)
equipment:level(150)
equipment:vocation("sorcerer", true, true)
equipment:vocation("druid", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(14769,16104,16105,16106,20089,22755,23527,23543)
equipment:level(150)
equipment:vocation("sorcerer", true, true)
equipment:vocation("druid", true, true)
equipment:register()

-- paladin of level 180
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(29427,29428,29429,30342,30343,30344,30345)
equipment:level(180)
equipment:vocation("paladin", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(29427,29428,29429,30342,30343,30344,30345)
equipment:level(180)
equipment:vocation("paladin", true, true)
equipment:register()

-- knight of level 180
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(29430)
equipment:level(180)
equipment:vocation("knight", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(29430)
equipment:level(180)
equipment:vocation("knight", true, true)
equipment:register()

-- mage of level 180
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(29418,29420,29423,29426,29424,29431)
equipment:level(180)
equipment:vocation("druid", true, true)
equipment:vocation("sorcerer", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(29418,29420,29423,29426,29424,29431)
equipment:level(180)
equipment:vocation("druid", true, true)
equipment:vocation("sorcerer", true, true)
equipment:register()

-- knight of level 185
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(13999)
equipment:level(185)
equipment:vocation("knight", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(13999)
equipment:level(185)
equipment:vocation("knight", true, true)
equipment:register()

-- player of level 200
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(20182,22758)
equipment:level(200)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(20182,22758)
equipment:level(200)
equipment:register()

-- paladin of level 200
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(22530,22531,22532,27648)
equipment:level(200)
equipment:vocation("paladin", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(22530,22531,22532,27648)
equipment:level(200)
equipment:vocation("paladin", true, true)
equipment:register()

-- knight of level 200
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(22518,22519,22520,22521,22522,22523,22524,22525,22526,22527,22528,22529,13993)
equipment:level(200)
equipment:vocation("knight", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(22518,22519,22520,22521,22522,22523,22524,22525,22526,22527,22528,22529,13993)
equipment:level(200)
equipment:vocation("knight", true, true)
equipment:register()

-- mage of level 200
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(22534,22535,22536,22537,27649)
equipment:level(200)
equipment:vocation("druid", true, true)
equipment:vocation("sorcerer", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(22534,22535,22536,22537,27649)
equipment:level(200)
equipment:vocation("druid", true, true)
equipment:vocation("sorcerer", true, true)
equipment:register()

-- paladin and knight of level 200
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(27650)
equipment:level(200)
equipment:vocation("paladin", true, true)
equipment:vocation("knight", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(27650)
equipment:level(200)
equipment:vocation("paladin", true, true)
equipment:vocation("knight", true, true)
equipment:register()

-- players of level 220
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(30323,31556,31557,31621)
equipment:level(220)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(30323,31556,31557,31621)
equipment:level(220)
equipment:register()

-- sorcerer of level 220
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(31582,31583)
equipment:level(220)
equipment:vocation("sorcerer", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(31582,31583)
equipment:level(220)
equipment:vocation("sorcerer", true, true)
equipment:register()

-- mage of level 220
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(30401,30402,30403)
equipment:level(220)
equipment:vocation("sorcerer", true, true)
equipment:vocation("druid", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(30401,30402,30403)
equipment:level(220)
equipment:vocation("sorcerer", true, true)
equipment:vocation("druid", true, true)
equipment:register()

-- paladin of level 220
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(31617)
equipment:level(220)
equipment:vocation("paladin", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(31617)
equipment:level(220)
equipment:vocation("paladin", true, true)
equipment:register()

-- knight of level 220
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(30394)
equipment:level(220)
equipment:vocation("knight", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(30394)
equipment:level(220)
equipment:vocation("knight", true, true)
equipment:register()

-- druid of level 230
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(31578)
equipment:level(230)
equipment:vocation("druid", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(31578)
equipment:level(230)
equipment:vocation("druid", true, true)
equipment:register()

-- paladin of level 230
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(31579)
equipment:level(230)
equipment:vocation("paladin", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(31579)
equipment:level(230)
equipment:vocation("paladin", true, true)
equipment:register()

-- knight of level 230
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(31577)
equipment:level(230)
equipment:vocation("knight", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(31577)
equipment:level(230)
equipment:vocation("knight", true, true)
equipment:register()

-- players of level 250
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(31631)
equipment:level(250)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(31631)
equipment:level(250)
equipment:register()

-- mage of level 250
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(20090)
equipment:level(250)
equipment:vocation("sorcerer", true, true)
equipment:vocation("druid", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(20090)
equipment:level(250)
equipment:vocation("sorcerer", true, true)
equipment:vocation("druid", true, true)
equipment:register()

-- knight of level 300
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(28719)
equipment:level(300)
equipment:vocation("knight", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(28719)
equipment:level(300)
equipment:vocation("knight", true, true)
equipment:register()

-- mage of level 300
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(27647,28714)
equipment:level(300)
equipment:vocation("druid", true, true)
equipment:vocation("sorcerer", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(27647,28714)
equipment:level(300)
equipment:vocation("druid", true, true)
equipment:vocation("sorcerer", true, true)
equipment:register()

-- paladin and knight of level 300
local equipment = MoveEvent()
function equipment.onEquip(player, item, slot, isCheck)
	return true
end
equipment:id(28715,28720,28721,28722)
equipment:level(300)
equipment:vocation("paladin", true, true)
equipment:vocation("knight", true, true)
equipment:register()

local equipment = MoveEvent()
function equipment.onDeEquip(player, item, slot, isCheck)
	return true
end
equipment:id(28715,28720,28721,28722)
equipment:level(300)
equipment:vocation("paladin", true, true)
equipment:vocation("knight", true, true)
equipment:register()
