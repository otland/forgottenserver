local mType = Game.createMonsterType("glooth anemone")
local monster = {}

monster.name = "Glooth Anemone"
monster.description = "a glooth anemone"
monster.experience = 1800
monster.outfit = {
	lookType = 604,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 2400
monster.maxHealth = 2400
monster.runHealth = 0
monster.race = "venom"
monster.corpse = 23359
monster.speed = 180
monster.summonCost = 0
monster.maxSummons = 2

monster.changeTarget = {
	interval = 2000,
	chance = 3
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
	{text = "*shglib*", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = -5},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, skill = 60, attack = 50, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 9, type = COMBAT_EARTHDAMAGE, minDamage = -100, maxDamage = -200, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_GLOOTHSPEAR, effect = CONST_ME_HITBYPOISON},
	{name ="combat", interval = 2000, chance = 7, type = COMBAT_LIFEDRAIN, minDamage = -50, maxDamage = -100, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_EARTHDAMAGE, radius = 5, target = false, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 25,
	armor = 15,
	{name ="combat", interval = 2000, chance = 9, type = COMBAT_HEALING, minDamage = 50, maxDamage = 100, effect = CONST_ME_MAGIC_GREEN}
}

monster.summons = {
	{name = "Glooth Blob", chance = 10, interval = 2000}
}

monster.loot = {
	{id = 23515, chance = 10190},
	{id = 23568, chance = 21020},
	{id = 2796, chance = 3180},
	{id = 23554, chance = 320},
	{id = 2148, chance = 100000, maxCount = 100},
	{id = 2148, chance = 60000, maxCount = 70},
	{id = 7588, chance = 6690, maxCount = 2},
	{id = 7589, chance = 6690, maxCount = 2},
	{id = 8473, chance = 960},
	{id = 2152, chance = 57320, maxCount = 3},
	{id = 9970, chance = 9240, maxCount = 3},
	{id = 2149, chance = 8600, maxCount = 3},
	{id = 2147, chance = 11150, maxCount = 3},
	{id = 23534, chance = 640},
	{id = 23543, chance = 5100},
	{id = 23550, chance = 320},
	{id = 23549, chance = 1270},
	{id = 23551, chance = 640},
	{id = 23529, chance = 1270}
}

mType:register(monster)
