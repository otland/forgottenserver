local mType = Game.createMonsterType("betrayed wraith")
local monster = {}

monster.name = "Betrayed Wraith"
monster.description = "a betrayed wraith"
monster.experience = 3500
monster.outfit = {
	lookType = 233,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 4200
monster.maxHealth = 4200
monster.runHealth = 300
monster.race = "undead"
monster.corpse = 6316
monster.speed = 230
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 15
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = false,
	boss = false,
	illusionable = true,
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
	{text = "Rrrah!", yell = false},
	{text = "Gnarr!", yell = false},
	{text = "Tcharrr!", yell = false}
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
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 50},
	{type = COMBAT_HOLYDAMAGE , percent = -20},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -450, effect = CONST_ME_DRAWBLOOD},
	{name ="betrayed wraith skill reducer", interval = 2000, chance = 10},
	{name ="speed", interval = 2000, chance = 20, speed = {min = -600, max = -600}, duration = 3000, range = 7, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_SMALLCLOUDS}
}

monster.defenses = {
	defense = 55,
	armor = 55,
	{name ="combat", interval = 2000, chance = 30, type = COMBAT_HEALING, minDamage = 350, maxDamage = 600, effect = CONST_ME_MAGIC_BLUE},
	{name ="invisible", interval = 2000, chance = 10, duration = 5000, effect = CONST_ME_DRAWBLOOD},
	{name ="speed", interval = 2000, chance = 15, speed = {min = 460, max = 460}, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = "small diamond", chance = 11800, maxCount = 4},
	{id = "gold coin", chance = 40000, maxCount = 100},
	{id = "gold coin", chance = 40000, maxCount = 100},
	{id = "gold coin", chance = 20000, maxCount = 20},
	{id = "platinum coin", chance = 100000, maxCount = 8},
	{id = "amulet of loss", chance = 130},
	{id = "power bolt", chance = 50000, maxCount = 5},
	{id = "orichalcum pearl", chance = 8000, maxCount = 2},
	{id = "skull helmet", chance = 390},
	{id = "golden figurine", chance = 160},
	{id = "soul orb", chance = 10000},
	{id = 6300, chance = 390},
	{id = "demonic essence", chance = 19430},
	{id = "concentrated demonic blood", chance = 65250},
	{id = "assassin star", chance = 10780, maxCount = 5},
	{id = "mercenary sword", chance = 1890},
	{id = "bloody edge", chance = 80},
	{id = "great mana potion", chance = 15000, maxCount = 3},
	{id = "ultimate health potion", chance = 15410},
	{id = "unholy bone", chance = 18410}
}

mType:register(monster)
