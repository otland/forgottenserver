local mType = Game.createMonsterType("dipthrah")
local monster = {}

monster.name = "Dipthrah"
monster.description = "Dipthrah"
monster.experience = 2900
monster.outfit = {
	lookType = 87,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 4200
monster.maxHealth = 4200
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 6031
monster.speed = 320
monster.summonCost = 0
monster.maxSummons = 4

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
	{text = "Come closer to learn the final lesson.", yell = false},
	{text = "You can't escape death forever.", yell = false},
	{text = "Undeath will shatter my shackles.", yell = false},
	{text = "You don't need this magic anymore.", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 100},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = -10},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -200, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 65, interval = 4000}},
	{name ="combat", interval = 4000, chance = 20, type = COMBAT_LIFEDRAIN, minDamage = -100, maxDamage = -800, range = 1},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_MANADRAIN, minDamage = -100, maxDamage = -500, range = 7, effect = CONST_ME_MAGIC_RED},
	{name ="speed", interval = 1000, chance = 15, speed = {min = -650, max = -650}, duration = 50000, range = 7, effect = CONST_ME_MAGIC_RED},
	{name ="drunk", interval = 1000, chance = 12, drunkenness = 25, duration = 10000, radius = 7, target = false, effect = CONST_ME_LOSEENERGY},
	{name ="melee", interval = 3000, chance = 34, minDamage = -50, maxDamage = -600, effect = CONST_ME_DRAWBLOOD},
	{name ="dipthrah skill reducer", interval = 2000, chance = 10}
}

monster.defenses = {
	defense = 25,
	armor = 25,
	{name ="combat", interval = 1000, chance = 25, type = COMBAT_HEALING, minDamage = 100, maxDamage = 200, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "Priestess", chance = 15, interval = 2000}
}

monster.loot = {
	{id = "ornamented ankh", chance = 100000},
	{id = "gold coin", chance = 30000, maxCount = 100},
	{id = "gold coin", chance = 30000, maxCount = 100},
	{id = "gold coin", chance = 30000, maxCount = 26},
	{id = "small sapphire", chance = 9800, maxCount = 3},
	{id = "great mana potion", chance = 8900},
	{id = "energy ring", chance = 4800},
	{id = "mind stone", chance = 1000},
	{id = "blue gem", chance = 900},
	{id = "ankh", chance = 600},
	{id = "skull staff", chance = 560},
	{id = "pharaoh sword", chance = 90},
	{id = "mini mummy", chance = 90}
}

mType:register(monster)
