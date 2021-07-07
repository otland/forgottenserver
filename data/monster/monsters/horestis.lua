local mType = Game.createMonsterType("horestis")
local monster = {}

monster.name = "Horestis"
monster.description = "Horestis"
monster.experience = 3500
monster.outfit = {
	lookType = 88,
	lookHead = 20,
	lookBody = 30,
	lookLegs = 40,
	lookFeet = 50,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 6000
monster.maxHealth = 6000
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 6031
monster.speed = 276
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
	{text = "Feel my eternal wrath!", yell = false},
	{text = "I might be dead but I'm not gone!", yell = false},
	{text = "Receive a lesson of the past!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = 5},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -592, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 165, interval = 4000}},
	{name ="combat", interval = 1000, chance = 8, type = COMBAT_LIFEDRAIN, minDamage = -300, maxDamage = -500, range = 1},
	{name ="combat", interval = 1000, chance = 12, type = COMBAT_EARTHDAMAGE, minDamage = -300, maxDamage = -500, range = 7, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_POISONAREA},
	{name ="speed", interval = 1000, chance = 25, speed = {min = -850, max = -850}, duration = 50000, range = 7, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 30,
	armor = 25,
	{name ="combat", interval = 1000, chance = 20, type = COMBAT_HEALING, minDamage = 200, maxDamage = 400, effect = CONST_ME_MAGIC_BLUE},
	{name ="invisible", interval = 1000, chance = 7, duration = 2000, effect = CONST_ME_MAGIC_BLUE},
	{name ="outfit", interval = 1000, chance = 3, monster = "ancient scarab", duration = 6000, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "silver brooch", chance = 12500},
	{id = "gold coin", chance = 17000, maxCount = 100},
	{id = "gold coin", chance = 17000, maxCount = 100},
	{id = "gold coin", chance = 17000, maxCount = 43},
	{id = "platinum coin", chance = 33333, maxCount = 5},
	{id = "scarab coin", chance = 14285, maxCount = 5},
	{id = "pharaoh sword", chance = 5000},
	{id = "twin axe", chance = 2500},
	{id = "great mana potion", chance = 10000, maxCount = 3},
	{id = "great health potion", chance = 5000, maxCount = 4},
	{id = 9813, chance = 2500},
	{id = "mini mummy", chance = 2500},
	{id = 13472, chance = 16666},
	{id = "scorpion sceptre", chance = 100000},
	{id = 13739, chance = 12500}
}

mType:register(monster)
