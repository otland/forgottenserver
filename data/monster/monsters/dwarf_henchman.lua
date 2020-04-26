local mType = Game.createMonsterType("dwarf henchman")
local monster = {}

monster.name = "Dwarf Henchman"
monster.description = "a dwarf henchman"
monster.experience = 15
monster.outfit = {
	lookType = 160,
	lookHead = 115,
	lookBody = 77,
	lookLegs = 93,
	lookFeet = 114,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 350
monster.maxHealth = 350
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 6007
monster.speed = 210
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
	pushable = true,
	boss = false,
	illusionable = false,
	canPushItems = false,
	canPushCreatures = false,
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
	{text = "This place is for our eyes only!", yell = false},
	{text = "We will live and let you die!", yell = false},
	{text = "I will die another day!", yell = false},
	{text = "We have license to kill!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -50, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 20, interval = 4000}},
	{name ="condition", interval = 2000, chance = 20, type = CONDITION_DROWN, startDamage = 0, tick = 5000}
}

monster.defenses = {
	defense = 16,
	armor = 16
}

monster.loot = {
}

mType:register(monster)
