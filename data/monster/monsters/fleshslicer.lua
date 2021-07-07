local mType = Game.createMonsterType("fleshslicer")
local monster = {}

monster.name = "Fleshslicer"
monster.description = "Fleshslicer"
monster.experience = 5500
monster.outfit = {
	lookType = 457,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 5700
monster.maxHealth = 5700
monster.runHealth = 0
monster.race = "venom"
monster.corpse = 15296
monster.speed = 360
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 10
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
	staticAttackChance = 95,
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
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -300, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_EARTHDAMAGE, minDamage = -100, maxDamage = -450, length = 5, spread = 3, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_EARTHDAMAGE, minDamage = -200, maxDamage = -500, range = 7, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_POISONAREA}
}

monster.defenses = {
	defense = 30,
	armor = 30,
	{name ="speed", interval = 2000, chance = 15, speed = {min = 450, max = 450}, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = 2148, chance = 50000, maxCount = 100},
	{id = 2148, chance = 50000, maxCount = 85},
	{id = 15486, chance = 100000, maxCount = 2},
	{id = 2152, chance = 100000, maxCount = 5},
	{id = 15485, chance = 100000},
	{id = 7590, chance = 78000, maxCount = 2},
	{id = "white pearl", chance = 35000, maxCount = 5},
	{id = "red gem", chance = 28000},
	{id = "Ripper Lance", chance = 28000},
	{id = "Small Ruby", chance = 28000, maxCount = 5},
	{id = 8473, chance = 21000},
	{id = 7413, chance = 14000},
	{id = 6300, chance = 7000},
	{id = 7632, chance = 7000}
}

mType:register(monster)
