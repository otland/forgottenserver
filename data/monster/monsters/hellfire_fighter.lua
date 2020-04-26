local mType = Game.createMonsterType("hellfire fighter")
local monster = {}

monster.name = "Hellfire Fighter"
monster.description = "a hellfire fighter"
monster.experience = 3900
monster.outfit = {
	lookType = 243,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 3800
monster.maxHealth = 3800
monster.runHealth = 0
monster.race = "fire"
monster.corpse = 6324
monster.speed = 220
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
	boss = false,
	illusionable = true,
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
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 50},
	{type = COMBAT_ENERGYDAMAGE, percent = 25},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -25},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 20}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -520, effect = CONST_ME_DRAWBLOOD},
	{name ="firefield", interval = 2000, chance = 10, range = 7, radius = 3, target = false, ShootEffect = CONST_ANI_FIRE},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_FIREDAMAGE, minDamage = -392, maxDamage = -1500, length = 8, spread = 0, effect = CONST_ME_FIREATTACK},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_FIREDAMAGE, minDamage = -60, maxDamage = -330, range = 7, radius = 3, target = false, effect = CONST_ME_HITBYFIRE},
	{name ="hellfire fighter soulfire", interval = 2000, chance = 15}
}

monster.defenses = {
	defense = 35,
	armor = 35
}

monster.loot = {
	{id = "emerald bangle", chance = 2200},
	{id = 2136, chance = 190},
	{id = "small diamond", chance = 1400},
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "gold coin", chance = 40000, maxCount = 100},
	{id = "gold coin", chance = 10000, maxCount = 46},
	{id = "bronze amulet", chance = 1000},
	{id = "wand of inferno", chance = 9450},
	{id = "burnt scroll", chance = 50000},
	{id = "blank rune", chance = 30000, maxCount = 2},
	{id = "fire sword", chance = 4140},
	{id = "fire axe", chance = 440},
	{id = "soul orb", chance = 12150},
	{id = "demonic essence", chance = 14500},
	{id = "magma legs", chance = 730},
	{id = "magma coat", chance = 470},
	{id = "fiery heart", chance = 9570},
	{id = "piece of hellfire armor", chance = 5060},
	{id = 13757, chance = 670}
}

mType:register(monster)
