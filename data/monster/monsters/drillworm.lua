local mType = Game.createMonsterType("drillworm")
local monster = {}

monster.name = "Drillworm"
monster.description = "a drillworm"
monster.experience = 1100
monster.outfit = {
	lookType = 527,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1500
monster.maxHealth = 1500
monster.runHealth = 0
monster.race = "venom"
monster.corpse = 19705
monster.speed = 240
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 20
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = false,
	boss = false,
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
	{text = "Knarrrk!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 15},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = -5},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 16},
	{type = COMBAT_HOLYDAMAGE , percent = 25},
	{type = COMBAT_DEATHDAMAGE , percent = 15}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -300, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 100, interval = 4000}},
	{name ="speed", interval = 2000, chance = 15, speed = {min = -600, max = -600}, duration = 15000, length = 8, spread = 0, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_EARTHDAMAGE, minDamage = -150, maxDamage = -300, length = 8, spread = 0, effect = CONST_ME_HITBYPOISON},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_EARTHDAMAGE, minDamage = -100, maxDamage = -150, radius = 3, target = true, effect = CONST_ME_POISONAREA}
}

monster.defenses = {
	defense = 35,
	armor = 35
}

monster.loot = {
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "gold coin", chance = 50000, maxCount = 50},
	{id = "guardian shield", chance = 2240},
	{id = "pick", chance = 5280},
	{id = "worm", chance = 4580, maxCount = 5},
	{id = "iron ore", chance = 1750},
	{id = "spiked squelcher", chance = 540},
	{id = "terra amulet", chance = 2360},
	{id = "lump of earth", chance = 15000},
	{id = "clay lump", chance = 700},
	{id = 13757, chance = 12130},
	{id = "green crystal splinter", chance = 9780},
	{id = "brown crystal splinter", chance = 9260},
	{id = "blue crystal splinter", chance = 9610},
	{id = "pulverized ore", chance = 5400},
	{id = "vein of ore", chance = 7280},
	{id = "drill bolt", chance = 4870, maxCount = 2}
}

mType:register(monster)
