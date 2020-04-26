local mType = Game.createMonsterType("vicious squire")
local monster = {}

monster.name = "Vicious Squire"
monster.description = "a vicious squire"
monster.experience = 900
monster.outfit = {
	lookType = 131,
	lookHead = 97,
	lookBody = 24,
	lookLegs = 73,
	lookFeet = 116,
	lookAddons = 1,
	lookMount = 0
}

monster.health = 1000
monster.maxHealth = 1000
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 20418
monster.speed = 210
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
	{text = "For hurting me, my sire will kill you!", yell = false},
	{text = "Your stuff will be mine soon!", yell = false},
	{text = "I'll cut you a bloody grin!", yell = false},
	{text = "You shouldn't have come here!", yell = false},
	{text = "You are as good as dead!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 40},
	{type = COMBAT_EARTHDAMAGE, percent = 50},
	{type = COMBAT_FIREDAMAGE, percent = 30},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE , percent = 50},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, skill = 82, attack = 50, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -110, range = 7, ShootEffect = CONST_ANI_BOLT}
}

monster.defenses = {
	defense = 33,
	armor = 27,
	{name ="combat", interval = 2000, chance = 7, type = COMBAT_HEALING, minDamage = 150, maxDamage = 200, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = 2455, chance = 12763},
	{id = 2148, chance = 100000, maxCount = 70},
	{id = 2661, chance = 743},
	{id = 2543, chance = 21685, maxCount = 10},
	{id = 2666, chance = 11771},
	{id = 2145, chance = 2602},
	{id = 2149, chance = 3097},
	{id = 2150, chance = 2973},
	{id = 2391, chance = 991},
	{id = 2381, chance = 2230},
	{id = 2477, chance = 619},
	{id = 2515, chance = 247},
	{id = 2475, chance = 123},
	{id = 7591, chance = 5576},
	{id = 2164, chance = 247}
}

mType:register(monster)
