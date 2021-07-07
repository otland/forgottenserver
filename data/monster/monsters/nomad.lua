local mType = Game.createMonsterType("nomad")
local monster = {}

monster.name = "Nomad"
monster.description = "a nomad"
monster.experience = 60
monster.outfit = {
	lookType = 146,
	lookHead = 97,
	lookBody = 39,
	lookLegs = 40,
	lookFeet = 3,
	lookAddons = 3,
	lookMount = 0
}

monster.health = 160
monster.maxHealth = 160
monster.runHealth = 15
monster.race = "blood"
monster.corpse = 20466
monster.speed = 205
monster.summonCost = 420
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
	{text = "We are the true sons of the desert!", yell = false},
	{text = "I will leave your remains to the vultures!", yell = false},
	{text = "We are swift as the wind of the desert!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 20},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE , percent = 20},
	{type = COMBAT_DEATHDAMAGE , percent = -10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -80, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_PHYSICALDAMAGE, radius = 1, target = false, effect = CONST_ME_SOUND_WHITE}
}

monster.defenses = {
	defense = 15,
	armor = 15
}

monster.loot = {
	{id = "gold coin", chance = 56250, maxCount = 40},
	{id = "axe", chance = 2730},
	{id = "mace", chance = 2120},
	{id = "iron helmet", chance = 650},
	{id = "brass armor", chance = 2350},
	{id = "steel shield", chance = 920},
	{id = "nomad parchment", chance = 200},
	{id = "potato", chance = 4840, maxCount = 3},
	{id = "dirty turban", chance = 2160},
	{id = "rope belt", chance = 6420}
}

mType:register(monster)
