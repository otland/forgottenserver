local mType = Game.createMonsterType("elder mummy")
local monster = {}

monster.name = "Elder Mummy"
monster.description = "an elder mummy"
monster.experience = 560
monster.outfit = {
	lookType = 65,
	lookHead = 20,
	lookBody = 30,
	lookLegs = 40,
	lookFeet = 50,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 850
monster.maxHealth = 850
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 6004
monster.speed = 180
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
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -120, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 3, interval = 4000}},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_DEATHDAMAGE, minDamage = 0, maxDamage = -130, range = 1, effect = CONST_ME_MORTAREA},
	{name ="speed", interval = 2000, chance = 15, speed = {min = -300, max = -300}, duration = 15000, range = 7, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 30,
	armor = 30
}

monster.loot = {
	{id = 2124, chance = 1650},
	{id = "silver brooch", chance = 4000},
	{id = "black pearl", chance = 1340},
	{id = "gold coin", chance = 43000, maxCount = 100},
	{id = "gold coin", chance = 43000, maxCount = 60},
	{id = "scarab coin", chance = 10000, maxCount = 3},
	{id = "strange talisman", chance = 4500},
	{id = 2162, chance = 6000},
	{id = "poison dagger", chance = 380},
	{id = "worm", chance = 20000, maxCount = 3},
	{id = "gauze bandage", chance = 10000},
	{id = "flask of embalming fluid", chance = 12600},
	{id = 13472, chance = 2400}
}

mType:register(monster)
