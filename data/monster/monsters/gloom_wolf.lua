local mType = Game.createMonsterType("gloom wolf")
local monster = {}

monster.name = "Gloom Wolf"
monster.description = "a gloom wolf"
monster.experience = 70
monster.outfit = {
	lookType = 716,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 200
monster.maxHealth = 200
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 24639
monster.speed = 125
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 0
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
	staticAttackChance = 80,
	targetDistance = 1,
	healthHidden = false,
	canWalkOnEnergy = true,
	canWalkOnFire = true,
	canWalkOnPoison = true
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
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -5},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE , percent = -5},
	{type = COMBAT_DEATHDAMAGE , percent = 10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -92, effect = CONST_ME_DRAWBLOOD},
	{name ="condition", interval = 2000, chance = 15, type = CONDITION_BLEEDING, startDamage = 0, tick = 4000, minDamage = -12, maxDamage = -26, radius = 2, target = false, effect = CONST_ME_HITAREA}
}

monster.defenses = {
	defense = 8,
	armor = 10,
	{name ="speed", interval = 2000, chance = 15, speed = {min = 250, max = 250}, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = "gold coin", chance = 66710, maxCount = 15},
	{id = "meat", chance = 40860, maxCount = 2},
	{id = "ham", chance = 39530, maxCount = 2},
	{id = "gloom wolf fur", chance = 8260},
	{id = "wolf tooth chain", chance = 7250},
	{id = "wolf paw", chance = 3030}
}

mType:register(monster)
