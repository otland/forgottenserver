local mType = Game.createMonsterType("chakoya tribewarden")
local monster = {}

monster.name = "Chakoya Tribewarden"
monster.description = "a chakoya tribewarden"
monster.experience = 40
monster.outfit = {
	lookType = 259,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 68
monster.maxHealth = 68
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 7320
monster.speed = 200
monster.summonCost = 305
monster.maxSummons = 0

monster.changeTarget = {
	interval = 60000,
	chance = 0
}

monster.flags = {
	summonable = true,
	attackable = true,
	hostile = true,
	convinceable = true,
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
	{text = "Quisavu tukavi!", yell = false},
	{text = "Si siyoqua jamjam!", yell = false},
	{text = "Achuq! jinuma!", yell = false},
	{text = "Si ji jusipa!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = -15},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 25},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = 10},
	{type = COMBAT_DEATHDAMAGE , percent = -5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -30, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 10,
	armor = 10
}

monster.loot = {
	{id = "gold coin", chance = 60000, maxCount = 30},
	{id = "short sword", chance = 4680},
	{id = "bone shield", chance = 1000},
	{id = 2667, chance = 20000},
	{id = "northern pike", chance = 90},
	{id = "rainbow trout", chance = 130},
	{id = "green perch", chance = 190},
	{id = "mammoth whopper", chance = 300}
}

mType:register(monster)
