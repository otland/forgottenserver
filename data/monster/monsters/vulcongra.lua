local mType = Game.createMonsterType("vulcongra")
local monster = {}

monster.name = "Vulcongra"
monster.description = "Vulcongra"
monster.experience = 1100
monster.outfit = {
	lookType = 509,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1600
monster.maxHealth = 1600
monster.runHealth = 220
monster.race = "fire"
monster.corpse = 18476
monster.speed = 230
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
	{text = "Fuchah! Fuchah!", yell = false},
	{text = "Fuchah!", yell = false},
	{text = "Yag! Yag! Yag!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -235, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_FIREDAMAGE, minDamage = -195, maxDamage = -340, length = 8, spread = 0, effect = CONST_ME_EXPLOSIONHIT},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_FIREDAMAGE, minDamage = -100, maxDamage = -200, range = 7, ShootEffect = CONST_ANI_FIRE},
	{name ="vulcongra soulfire", interval = 3000, chance = 100}
}

monster.defenses = {
	defense = 35,
	armor = 35
}

monster.loot = {
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "gold coin", chance = 50000, maxCount = 7},
	{id = "platinum coin", chance = 14780},
	{id = "wand of inferno", chance = 1000},
	{id = "sword ring", chance = 2920},
	{id = "fire sword", chance = 260},
	{id = "banana", chance = 10420, maxCount = 10},
	{id = "strong health potion", chance = 7100},
	{id = "strong mana potion", chance = 7100},
	{id = "magma amulet", chance = 970},
	{id = "magma coat", chance = 140},
	{id = "fiery heart", chance = 9380},
	{id = 13757, chance = 940},
	{id = "brown crystal splinter", chance = 4850},
	{id = "red crystal fragment", chance = 2280},
	{id = "magma clump", chance = 12440},
	{id = "blazing bone", chance = 8030}
}

mType:register(monster)
