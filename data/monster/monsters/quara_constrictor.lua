local mType = Game.createMonsterType("quara constrictor")
local monster = {}

monster.name = "Quara Constrictor"
monster.description = "a quara constrictor"
monster.experience = 250
monster.outfit = {
	lookType = 46,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 450
monster.maxHealth = 450
monster.runHealth = 30
monster.race = "blood"
monster.corpse = 6065
monster.speed = 230
monster.summonCost = 670
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = true,
	attackable = true,
	hostile = true,
	convinceable = true,
	pushable = false,
	boss = false,
	illusionable = true,
	canPushItems = true,
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
	{text = "Gaaahhh!", yell = false},
	{text = "Gluh! Gluh!", yell = false},
	{text = "Tssss!", yell = false},
	{text = "Boohaa!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = -25},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -150, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 20, interval = 4000}},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_LIFEDRAIN, minDamage = -50, maxDamage = -90, radius = 3, target = false, effect = CONST_ME_HITAREA},
	{name ="quara constrictor freeze", interval = 2000, chance = 10},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_ICEDAMAGE, minDamage = -40, maxDamage = -70, range = 7, radius = 4, target = false, effect = CONST_ME_ICEATTACK},
	{name ="quara constrictor electrify", interval = 2000, chance = 10, range = 1}
}

monster.defenses = {
	defense = 20,
	armor = 20
}

monster.loot = {
	{id = "gold coin", chance = 87500, maxCount = 100},
	{id = "small amethyst", chance = 2860},
	{id = "longsword", chance = 7761},
	{id = "brass armor", chance = 5000},
	{id = "shrimp", chance = 5000, maxCount = 5},
	{id = "fish fin", chance = 430},
	{id = "quara tentacle", chance = 14520}
}

mType:register(monster)
