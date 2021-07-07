local mType = Game.createMonsterType("tanjis")
local monster = {}

monster.name = "Tanjis"
monster.description = "Tanjis"
monster.experience = 15000
monster.outfit = {
	lookType = 446,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 30000
monster.maxHealth = 30000
monster.runHealth = 60
monster.race = "blood"
monster.corpse = 15228
monster.speed = 560
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 2000,
	chance = 50
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = true,
	boss = true,
	illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
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
	{text = "JAUL QJELL, JAKNH JEH KENH!!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -20},
	{type = COMBAT_ENERGYDAMAGE, percent = -20},
	{type = COMBAT_EARTHDAMAGE, percent = -20},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = -20}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -600, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2500, chance = 20, type = COMBAT_MANADRAIN, minDamage = -200, maxDamage = -600, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_CARNIPHILA},
	{name ="combat", interval = 3500, chance = 27, type = COMBAT_ICEDAMAGE, minDamage = -200, maxDamage = -400, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_ICE, effect = CONST_ME_ICEAREA},
	{name ="combat", interval = 3500, chance = 15, type = COMBAT_ICEDAMAGE, minDamage = -100, maxDamage = -400, range = 1, radius = 1, target = true},
	{name ="combat", interval = 2300, chance = 11, type = COMBAT_DROWNDAMAGE, minDamage = -200, maxDamage = -500, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_ICE, effect = CONST_ME_WATERSPLASH},
	{name ="combat", interval = 2300, chance = 14, type = COMBAT_MANADRAIN, minDamage = -200, maxDamage = -600, range = 7, radius = 7, target = false, effect = CONST_ME_BUBBLES},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_PHYSICALDAMAGE, minDamage = -100, maxDamage = -400, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_LARGEROCK},
	{name ="combat", interval = 1200, chance = 7, type = COMBAT_DEATHDAMAGE, minDamage = -300, maxDamage = -800, length = 8, spread = 0, effect = CONST_ME_SMALLCLOUDS},
	{name ="combat", interval = 2000, chance = 14, type = COMBAT_ENERGYDAMAGE, minDamage = -200, maxDamage = -500, length = 8, spread = 3, effect = CONST_ME_PURPLEENERGY},
	{name ="speed", interval = 2150, chance = 16, speed = {min = -600, max = -600}, duration = 15000, range = 7, radius = 1, target = true}
}

monster.defenses = {
	defense = 40,
	armor = 40,
	{name ="combat", interval = 3000, chance = 40, type = COMBAT_HEALING, minDamage = 800, maxDamage = 1100, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "depth ocrea", chance = 50000},
	{id = "ornate shield", chance = 20000},
	{id = "ornate mace", chance = 25000}
}

mType:register(monster)
