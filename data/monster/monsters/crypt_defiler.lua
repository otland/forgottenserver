local mType = Game.createMonsterType("crypt defiler")
local monster = {}

monster.name = "Crypt Defiler"
monster.description = "a crypt defiler"
monster.experience = 70
monster.outfit = {
	lookType = 146,
	lookHead = 62,
	lookBody = 132,
	lookLegs = 42,
	lookFeet = 75,
	lookAddons = 3,
	lookMount = 0
}

monster.health = 180
monster.maxHealth = 180
monster.runHealth = 15
monster.race = "blood"
monster.corpse = 20359
monster.speed = 205
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 10
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
	{text = "I smell gold!", yell = false},
	{text = "You'll make a fine fake-mummy to be sold!", yell = false},
	{text = "Untold riches are awaiting me!", yell = false},
	{text = "I don't like competition", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -90, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -40, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_THROWINGSTAR}
}

monster.defenses = {
	defense = 15,
	armor = 15
}

monster.loot = {
	{id = "gold coin", chance = 50900, maxCount = 39},
	{id = "axe", chance = 2730},
	{id = "mace", chance = 1800},
	{id = "iron helmet", chance = 450},
	{id = "brass armor", chance = 2240},
	{id = "steel shield", chance = 450},
	{id = "nomad parchment", chance = 220},
	{id = "potato", chance = 4840, maxCount = 3},
	{id = "dirty turban", chance = 1570},
	{id = "rope belt", chance = 4040}
}

mType:register(monster)
