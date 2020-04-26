local mType = Game.createMonsterType("xenia")
local monster = {}

monster.name = "Xenia"
monster.description = "Xenia"
monster.experience = 255
monster.outfit = {
	lookType = 137,
	lookHead = 95,
	lookBody = 115,
	lookLegs = 115,
	lookFeet = 95,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 200
monster.maxHealth = 200
monster.runHealth = 10
monster.race = "blood"
monster.corpse = 20542
monster.speed = 176
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 5000,
	chance = 8
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = false,
	boss = true,
	illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	staticAttackChance = 90,
	targetDistance = 1,
	healthHidden = false,
	canWalkOnEnergy = false,
	canWalkOnFire = false,
	canWalkOnPoison = false
}

monster.light = {
	level = 0,
	color = 0
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Stand still!", yell = false},
	{text = "One more head for me!", yell = false},
	{text = "Head off!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -6},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 10},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE , percent = 7},
	{type = COMBAT_DEATHDAMAGE , percent = -7}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -50, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 1000, chance = 15, type = COMBAT_PHYSICALDAMAGE, minDamage = -28, maxDamage = -42, range = 7, ShootEffect = CONST_ANI_SPEAR}
}

monster.defenses = {
	defense = 14,
	armor = 12
}

monster.loot = {
	{id = "crystal necklace", chance = 2500},
	{id = "small diamond", chance = 2500, maxCount = 2},
	{id = "small ruby", chance = 2500, maxCount = 2},
	{id = "gold coin", chance = 100000, maxCount = 45},
	{id = "protection amulet", chance = 1000},
	{id = "double axe", chance = 3333},
	{id = "machete", chance = 10000},
	{id = "chain helmet", chance = 5000},
	{id = "plate armor", chance = 1333},
	{id = "chain armor", chance = 6666},
	{id = "meat", chance = 20000, maxCount = 3},
	{id = "red apple", chance = 10000, maxCount = 4},
	{id = "worm", chance = 50000, maxCount = 10}
}

mType:register(monster)
