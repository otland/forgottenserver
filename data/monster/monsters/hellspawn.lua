local mType = Game.createMonsterType("hellspawn")
local monster = {}

monster.name = "Hellspawn"
monster.description = "a hellspawn"
monster.experience = 2550
monster.outfit = {
	lookType = 322,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 3500
monster.maxHealth = 3500
monster.runHealth = 0
monster.race = "fire"
monster.corpse = 9923
monster.speed = 260
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 15
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
	{text = "Your fragile bones are like toothpicks to me.", yell = false},
	{text = "You little weasel will not live to see another day.", yell = false},
	{text = "I'm just a messenger of what's yet to come.", yell = false},
	{text = "HRAAAAAAAAAAAAAAAARRRR", yell = true},
	{text = "I'm taking you down with me!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 20},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = 80},
	{type = COMBAT_FIREDAMAGE, percent = 40},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE , percent = 30},
	{type = COMBAT_DEATHDAMAGE , percent = -5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -352, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_FIREDAMAGE, minDamage = -150, maxDamage = -175, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_FIREATTACK},
	{name ="hellspawn soulfire", interval = 2000, chance = 10, range = 5}
}

monster.defenses = {
	defense = 40,
	armor = 40,
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_HEALING, minDamage = 120, maxDamage = 230, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 2000, chance = 15, speed = {min = 270, max = 270}, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = "gold coin", chance = 60000, maxCount = 100},
	{id = "gold coin", chance = 60000, maxCount = 100},
	{id = "gold coin", chance = 60000, maxCount = 36},
	{id = "morning star", chance = 10000},
	{id = "broadsword", chance = 10000},
	{id = "warrior helmet", chance = 1886},
	{id = "knight legs", chance = 3030},
	{id = "red mushroom", chance = 7692, maxCount = 2},
	{id = "demonic essence", chance = 9090},
	{id = "assassin star", chance = 9090, maxCount = 2},
	{id = "onyx flail", chance = 103},
	{id = "berserk potion", chance = 934},
	{id = "spiked squelcher", chance = 970},
	{id = "great health potion", chance = 40333},
	{id = "ultimate health potion", chance = 9090},
	{id = 9809, chance = 3125},
	{id = 9810, chance = 3125},
	{id = 9948, chance = 140},
	{id = "black skull", chance = 151},
	{id = "small topaz", chance = 5882, maxCount = 3},
	{id = "hellspawn tail", chance = 20000}
}

mType:register(monster)
