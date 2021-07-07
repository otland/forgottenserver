local mType = Game.createMonsterType("gnorre chyllson")
local monster = {}

monster.name = "Gnorre Chyllson"
monster.description = "Gnorre Chyllson"
monster.experience = 4000
monster.outfit = {
	lookType = 251,
	lookHead = 11,
	lookBody = 9,
	lookLegs = 11,
	lookFeet = 85,
	lookAddons = 1,
	lookMount = 0
}

monster.health = 7150
monster.maxHealth = 7150
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 7349
monster.speed = 370
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 0,
	chance = 0
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
	canPushCreatures = false,
	staticAttackChance = 95,
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
	{text = "I am like the merciless northwind.", yell = false},
	{text = "Snow will be your death shroud.", yell = false},
	{text = "Feel the wrath of father chyll!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -15},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = -10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -455, effect = CONST_ME_DRAWBLOOD},
	{name ="melee", interval = 3000, chance = 50, minDamage = -500, maxDamage = -800, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 1000, chance = 15, type = COMBAT_ICEDAMAGE, minDamage = -170, maxDamage = -200, range = 7, ShootEffect = CONST_ANI_SNOWBALL}
}

monster.defenses = {
	defense = 52,
	armor = 51
}

monster.loot = {
}

mType:register(monster)
