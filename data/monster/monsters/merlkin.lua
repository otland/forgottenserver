local mType = Game.createMonsterType("merlkin")
local monster = {}

monster.name = "Merlkin"
monster.description = "a merlkin"
monster.experience = 145
monster.outfit = {
	lookType = 117,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 235
monster.maxHealth = 235
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 6044
monster.speed = 194
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
	staticAttackChance = 70,
	targetDistance = 4,
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
	{text = "Ugh! Ugh! Ugh!", yell = false},
	{text = "Holy banana!", yell = false},
	{text = "Chakka! Chakka!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 20},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -15},
	{type = COMBAT_HOLYDAMAGE , percent = 10},
	{type = COMBAT_DEATHDAMAGE , percent = -5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -30, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_FIREDAMAGE, minDamage = -60, maxDamage = -90, range = 7, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_HITBYFIRE},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_ENERGYDAMAGE, minDamage = -15, maxDamage = -45, range = 7, ShootEffect = CONST_ANI_ENERGY, effect = CONST_ME_ENERGYHIT},
	{name ="poisonfield", interval = 2000, chance = 15, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_POISON}
}

monster.defenses = {
	defense = 16,
	armor = 16,
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_HEALING, minDamage = 30, maxDamage = 40, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "gold coin", chance = 83000, maxCount = 45},
	{id = "banana", chance = 30350, maxCount = 12},
	{id = 2162, chance = 3000},
	{id = "banana sash", chance = 1900},
	{id = "ape fur", chance = 1000},
	{id = "orange", chance = 1000, maxCount = 5},
	{id = "wand of decay", chance = 980},
	{id = "mana potion", chance = 700},
	{id = "small amethyst", chance = 310},
	{id = "banana staff", chance = 100}
}

mType:register(monster)
