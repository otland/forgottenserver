local mType = Game.createMonsterType("grave robber")
local monster = {}

monster.name = "Grave Robber"
monster.description = "a grave robber"
monster.experience = 65
monster.outfit = {
	lookType = 146,
	lookHead = 57,
	lookBody = 95,
	lookLegs = 57,
	lookFeet = 19,
	lookAddons = 3,
	lookMount = 0
}

monster.health = 165
monster.maxHealth = 165
monster.runHealth = 15
monster.race = "blood"
monster.corpse = 20411
monster.speed = 205
monster.summonCost = 355
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
	pushable = true,
	boss = false,
	illusionable = true,
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
	{text = "This crypt is taken!", yell = false},
	{text = "Leave or you are hunted!", yell = false},
	{text = "The treasure will be mine!", yell = false},
	{text = "The X marks the spot!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 20},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = -10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -90, effect = CONST_ME_DRAWBLOOD},
	{name ="condition", interval = 2000, chance = 15, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -100, maxDamage = -160, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_POISON}
}

monster.defenses = {
	defense = 15,
	armor = 15
}

monster.loot = {
	{id = "gold coin", chance = 54260, maxCount = 38},
	{id = "axe", chance = 2500},
	{id = "mace", chance = 1700},
	{id = "iron helmet", chance = 500},
	{id = "brass armor", chance = 1700},
	{id = "steel shield", chance = 500},
	{id = "nomad parchment", chance = 170},
	{id = "potato", chance = 5680, maxCount = 3},
	{id = "dirty turban", chance = 1700},
	{id = "rope belt", chance = 7510}
}

mType:register(monster)
