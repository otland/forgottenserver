local mType = Game.createMonsterType("shiversleep")
local monster = {}

monster.name = "Shiversleep"
monster.description = "Shiversleep"
monster.experience = 1900
monster.outfit = {
	lookType = 592,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 4600
monster.maxHealth = 4600
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 22497
monster.speed = 270
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
	{text = "Knorrrr!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 100},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -450, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 1800, chance = 10, type = COMBAT_EARTHDAMAGE, minDamage = -300, maxDamage = -685, length = 7, spread = 0, effect = CONST_ME_STONES},
	{name ="combat", interval = 2000, chance = 9, type = COMBAT_EARTHDAMAGE, minDamage = -250, maxDamage = -590, radius = 6, target = false, effect = CONST_ME_BIGPLANTS}
}

monster.defenses = {
	defense = 55,
	armor = 55
}

monster.loot = {
	{id = "white pearl", chance = 866},
	{id = "black pearl", chance = 866},
	{id = "gold coin", chance = 30000, maxCount = 100},
	{id = "platinum coin", chance = 33333, maxCount = 10},
	{id = "giant sword", chance = 422},
	{id = "knight legs", chance = 444},
	{id = "crown armor", chance = 632},
	{id = "iron ore", chance = 2195, maxCount = 2},
	{id = "magic sulphur", chance = 632},
	{id = "great mana potion", chance = 3100, maxCount = 4},
	{id = "ultimate health potion", chance = 2195, maxCount = 2},
	{id = "crystal of power", chance = 1066},
	{id = "shiny stone", chance = 1032},
	{id = "wand of defiance", chance = 1766},
	{id = "glacial rod", chance = 1066},
	{id = "prismatic bolt", chance = 366, maxCount = 8},
	{id = "crystalline sword", chance = 603},
	{id = "crystal crossbow", chance = 603}
}

mType:register(monster)
