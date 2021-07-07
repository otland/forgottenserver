local mType = Game.createMonsterType("water elemental")
local monster = {}

monster.name = "Water Elemental"
monster.description = "a water elemental"
monster.experience = 650
monster.outfit = {
	lookType = 286,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 550
monster.maxHealth = 550
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 10499
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
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 35},
	{type = COMBAT_ENERGYDAMAGE, percent = -25},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = 50},
	{type = COMBAT_DEATHDAMAGE , percent = 50}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -160, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 60, interval = 4000}},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_DROWNDAMAGE, minDamage = -125, maxDamage = -235, range = 7, radius = 2, target = true, effect = CONST_ME_LOSEENERGY},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_ICEDAMAGE, minDamage = -88, maxDamage = -150, range = 7, ShootEffect = CONST_ANI_SMALLICE},
	{name ="condition", interval = 2000, chance = 15, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -225, maxDamage = -260, radius = 5, target = false, effect = CONST_ME_POISONAREA}
}

monster.defenses = {
	defense = 20,
	armor = 20,
	{name ="combat", interval = 2000, chance = 5, type = COMBAT_HEALING, minDamage = 50, maxDamage = 80, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 2000, chance = 15, speed = {min = 300, max = 300}, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = "small diamond", chance = 1000},
	{id = "small sapphire", chance = 1000},
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "small emerald", chance = 1000, maxCount = 2},
	{id = "platinum coin", chance = 10000},
	{id = "energy ring", chance = 950},
	{id = "life ring", chance = 930},
	{id = 2667, chance = 20000},
	{id = "rainbow trout", chance = 940},
	{id = "green perch", chance = 1050},
	{id = "strong health potion", chance = 10000},
	{id = "strong mana potion", chance = 10000},
	{id = 7632, chance = 800},
	{id = 7633, chance = 800},
	{id = "leviathan's amulet", chance = 260}
}

mType:register(monster)
