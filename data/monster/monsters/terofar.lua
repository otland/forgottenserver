local mType = Game.createMonsterType("terofar")
local monster = {}

monster.name = "Terofar"
monster.description = "Terofar"
monster.experience = 24000
monster.outfit = {
	lookType = 12,
	lookHead = 19,
	lookBody = 0,
	lookLegs = 77,
	lookFeet = 79,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 40000
monster.maxHealth = 40000
monster.runHealth = 0
monster.race = "fire"
monster.corpse = 6068
monster.speed = 440
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 2000,
	chance = 25
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
	canPushCreatures = true,
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
	{text = "IT TOOK AN ARMY OF YOUR KIND TO DEFEAT THE WARDEN. NOW YOU ARE ALONE!", yell = true},
	{text = "FEELS GOOD TO BE BACK IN ACTION!", yell = true},
	{text = "THE WARDS ARE FAILING! MY ESCAPE IS ONLY A MATTER OF TIME!!", yell = true},
	{text = "Terofar cast a greater death curse on you!", yell = true}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 50},
	{type = COMBAT_ENERGYDAMAGE, percent = 50},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 50},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 50},
	{type = COMBAT_HOLYDAMAGE , percent = 50},
	{type = COMBAT_DEATHDAMAGE , percent = 50}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, skill = 180, attack = 100, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 30, type = COMBAT_DEATHDAMAGE, minDamage = -700, maxDamage = -1500, length = 8, spread = 0, effect = CONST_ME_SMALLCLOUDS},
	{name ="combat", interval = 2000, chance = 18, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -750, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_WHIRLWINDAXE}
}

monster.defenses = {
	defense = 65,
	armor = 55,
	{name ="combat", interval = 2000, chance = 30, type = COMBAT_HEALING, minDamage = 300, maxDamage = 500, radius = 8, target = false, effect = CONST_ME_DRAWBLOOD},
	{name ="speed", interval = 2000, chance = 18, speed = {min = 784, max = 784}, duration = 7000, effect = CONST_ME_MAGIC_GREEN}
}

monster.loot = {
	{id = 22396, chance = 100000},
	{id = 22598, chance = 100000},
	{id = 22397, chance = 100000},
	{id = 6500, chance = 100000},
	{id = 5954, chance = 100000},
	{id = 22610, chance = 3560},
	{id = 2148, chance = 50000, maxCount = 100},
	{id = 2148, chance = 50000, maxCount = 100},
	{id = 2152, chance = 100000, maxCount = 50},
	{id = 7590, chance = 33330, maxCount = 10},
	{id = 8473, chance = 3890, maxCount = 10},
	{id = 8472, chance = 2780, maxCount = 5},
	{id = 18413, chance = 22220, maxCount = 8},
	{id = 18414, chance = 33330, maxCount = 8},
	{id = 18415, chance = 44440, maxCount = 8},
	{id = 9971, chance = 16670},
	{id = 7632, chance = 38890},
	{id = 7633, chance = 38890},
	{id = 2520, chance = 11110},
	{id = 2515, chance = 22220},
	{id = 2519, chance = 11110},
	{id = 2514, chance = 960},
	{id = 8891, chance = 5560},
	{id = 8879, chance = 11110},
	{id = 8877, chance = 6110},
	{id = 2155, chance = 16670},
	{id = 2158, chance = 11110},
	{id = 8882, chance = 410},
	{id = 2472, chance = 610},
	{id = 2645, chance = 910},
	{id = 2498, chance = 910},
	{id = 7886, chance = 2910},
	{id = 2195, chance = 1910},
	{id = 5741, chance = 1910}
}

mType:register(monster)
