local mType = Game.createMonsterType("rustheap golem")
local monster = {}

monster.name = "Rustheap Golem"
monster.description = "a rustheap golem"
monster.experience = 2300
monster.outfit = {
	lookType = 603,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 2800
monster.maxHealth = 2800
monster.runHealth = 0
monster.race = "venom"
monster.corpse = 23355
monster.speed = 250
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 2000,
	chance = 5
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
	{text = "*clatter*", yell = false},
	{text = "*krrk*", yell = false},
	{text = "*frzzp*", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 70},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, skill = 118, attack = 50, effect = CONST_ME_DRAWBLOOD},
	{name ="war golem electrify", interval = 2000, chance = 15, range = 1},
	{name ="speed", interval = 2000, chance = 25, speed = {min = -900, max = -900}, duration = 10000, radius = 3, target = true, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_ENERGYDAMAGE, minDamage = -110, maxDamage = -210, range = 7, ShootEffect = CONST_ANI_ENERGY}
}

monster.defenses = {
	defense = 33,
	armor = 26,
	{name ="speed", interval = 2000, chance = 11, speed = {min = 428, max = 428}, duration = 6000, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = 8309, chance = 1950},
	{id = 9930, chance = 5860},
	{id = 23567, chance = 14980},
	{id = 2148, chance = 100000, maxCount = 100},
	{id = 2148, chance = 80000, maxCount = 100},
	{id = 2148, chance = 60000, maxCount = 90},
	{id = 5880, chance = 330},
	{id = 24124, chance = 1950},
	{id = 2152, chance = 58310, maxCount = 3},
	{id = 2143, chance = 1300},
	{id = 2144, chance = 1630},
	{id = 7588, chance = 10100, maxCount = 2},
	{id = 7589, chance = 7170, maxCount = 2},
	{id = 2391, chance = 3580},
	{id = 7452, chance = 330},
	{id = 23542, chance = 330},
	{id = 23541, chance = 2930},
	{id = 9810, chance = 5860},
	{id = 9813, chance = 4230}
}

mType:register(monster)
