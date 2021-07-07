local mType = Game.createMonsterType("lizard sentinel")
local monster = {}

monster.name = "Lizard Sentinel"
monster.description = "a lizard sentinel"
monster.experience = 110
monster.outfit = {
	lookType = 114,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 265
monster.maxHealth = 265
monster.runHealth = 10
monster.race = "blood"
monster.corpse = 6040
monster.speed = 180
monster.summonCost = 560
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
	canPushItems = true,
	canPushCreatures = false,
	staticAttackChance = 90,
	targetDistance = 4,
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
	{text = "Tssss!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -45, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 30, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -70, range = 7, ShootEffect = CONST_ANI_SPEAR}
}

monster.defenses = {
	defense = 17,
	armor = 17
}

monster.loot = {
	{id = "gold coin", chance = 86000, maxCount = 80},
	{id = "spear", chance = 10000, maxCount = 3},
	{id = "chain armor", chance = 9300},
	{id = "scale armor", chance = 7730},
	{id = "hunting spear", chance = 4300},
	{id = "lizard leather", chance = 2100},
	{id = "lizard scale", chance = 1960},
	{id = "halberd", chance = 1300},
	{id = "obsidian lance", chance = 1320},
	{id = "health potion", chance = 1100},
	{id = "sentinel shield", chance = 320},
	{id = "small diamond", chance = 190}
}

mType:register(monster)
