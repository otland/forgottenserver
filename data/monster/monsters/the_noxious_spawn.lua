local mType = Game.createMonsterType("the noxious spawn")
local monster = {}

monster.name = "The Noxious Spawn"
monster.description = "The Noxious Spawn"
monster.experience = 6000
monster.outfit = {
	lookType = 220,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 9500
monster.maxHealth = 9500
monster.runHealth = 275
monster.race = "venom"
monster.corpse = 4323
monster.speed = 360
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 5000,
	chance = 8
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = false,
	boss = true,
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
	{text = "Sssssouls for the one", yell = false},
	{text = "HISSSS", yell = true},
	{text = "Tsssse one will risssse again", yell = false},
	{text = "I bring your deathhh, mortalssss", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
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
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -250, effect = CONST_ME_DRAWBLOOD},
	{name ="speed", interval = 2000, chance = 6, speed = {min = -660, max = -660}, duration = 5000, range = 7},
	{name ="combat", interval = 2000, chance = 7, type = COMBAT_EARTHDAMAGE, minDamage = 0, maxDamage = -550, length = 8, spread = 3, effect = CONST_ME_SMALLPLANTS},
	{name ="combat", interval = 2000, chance = 9, type = COMBAT_LIFEDRAIN, minDamage = 0, maxDamage = -550, length = 8, spread = 0, effect = CONST_ME_SOUND_RED},
	{name ="combat", interval = 2000, chance = 12, type = COMBAT_EARTHDAMAGE, minDamage = 0, maxDamage = -300, range = 1, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_POISONAREA},
	{name ="outfit", interval = 2000, chance = 11, item = 3976, duration = 4000, range = 7, effect = CONST_ME_MAGIC_BLUE}
}

monster.defenses = {
	defense = 25,
	armor = 18,
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_HEALING, minDamage = 900, maxDamage = 1000, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "platinum coin", chance = 51850, maxCount = 5},
	{id = "platinum coin", chance = 10750, maxCount = 95},
	{id = "life ring", chance = 35000},
	{id = "power bolt", chance = 36250, maxCount = 29},
	{id = "green mushroom", chance = 11111},
	{id = "onyx arrow", chance = 525},
	{id = "mercenary sword", chance = 27750},
	{id = "noble axe", chance = 37000},
	{id = 10309, chance = 31500}
}

mType:register(monster)
