local mType = Game.createMonsterType("cliff strider")
local monster = {}

monster.name = "Cliff Strider"
monster.description = "a cliff strider"
monster.experience = 5700
monster.outfit = {
	lookType = 497,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 9400
monster.maxHealth = 9400
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 17420
monster.speed = 260
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
	illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	staticAttackChance = 80,
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
	{text = "Knorrrr", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 1},
	{type = COMBAT_ENERGYDAMAGE, percent = 100},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 1},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 40}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -499, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -800, radius = 4, target = true, ShootEffect = CONST_ANI_LARGEROCK, effect = CONST_ME_STONES},
	{name ="cliff strider skill reducer", interval = 2000, chance = 10},
	{name ="cliff strider electrify", interval = 2000, chance = 15, range = 1},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -1000, length = 6, spread = 0, effect = CONST_ME_GROUNDSHAKER},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_MANADRAIN, minDamage = -100, maxDamage = -300, radius = 4, target = false, effect = CONST_ME_YELLOWENERGY}
}

monster.defenses = {
	defense = 55,
	armor = 55
}

monster.loot = {
	{id = "white pearl", chance = 9630, maxCount = 3},
	{id = "black pearl", chance = 9090},
	{id = "gold coin", chance = 100000, maxCount = 100},
	{id = "gold coin", chance = 100000, maxCount = 95},
	{id = "platinum coin", chance = 100000, maxCount = 10},
	{id = "red gem", chance = 6110},
	{id = "blue gem", chance = 830},
	{id = "giant sword", chance = 620},
	{id = "hammer of wrath", chance = 70},
	{id = "knight legs", chance = 830},
	{id = "crown armor", chance = 310},
	{id = "crusader helmet", chance = 410},
	{id = "steel boots", chance = 100},
	{id = "iron ore", chance = 14490},
	{id = "magic sulphur", chance = 1450},
	{id = "soul orb", chance = 17080},
	{id = "sapphire hammer", chance = 1350},
	{id = "spiked squelcher", chance = 1040},
	{id = "great mana potion", chance = 30950, maxCount = 4},
	{id = "ultimate health potion", chance = 25260, maxCount = 2},
	{id = "crystal of balance", chance = 2690},
	{id = "crystal of power", chance = 720},
	{id = "shiny stone", chance = 11280},
	{id = "wand of defiance", chance = 2000},
	{id = "glacial rod", chance = 1240},
	{id = "blue crystal shard", chance = 6520},
	{id = "blue crystal splinter", chance = 7970, maxCount = 2},
	{id = "cyan crystal fragment", chance = 8070},
	{id = "pulverized ore", chance = 15110},
	{id = "cliff strider claw", chance = 15630},
	{id = "vein of ore", chance = 18000, maxCount = 2},
	{id = "prismatic bolt", chance = 9090, maxCount = 8},
	{id = "crystalline sword", chance = 620},
	{id = "crystal crossbow", chance = 830}
}

mType:register(monster)
