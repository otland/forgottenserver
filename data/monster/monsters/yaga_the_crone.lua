local mType = Game.createMonsterType("yaga the crone")
local monster = {}

monster.name = "Yaga The Crone"
monster.description = "Yaga the Crone"
monster.experience = 375
monster.outfit = {
	lookType = 54,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 620
monster.maxHealth = 620
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 6081
monster.speed = 240
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
	{text = "Where did I park my hut?", yell = false},
	{text = "You will taste so sweet!", yell = false},
	{text = "Hexipooh, bewitched are you!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -1},
	{type = COMBAT_ENERGYDAMAGE, percent = 100},
	{type = COMBAT_EARTHDAMAGE, percent = 1},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = -5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -90, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2500, chance = 50, type = COMBAT_FIREDAMAGE, minDamage = -30, maxDamage = -50, range = 7, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_HITBYFIRE},
	{name ="condition", interval = 3000, chance = 13, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -200, maxDamage = -300, range = 7, ShootEffect = CONST_ANI_POISON},
	{name ="firefield", interval = 2000, chance = 13, range = 7, ShootEffect = CONST_ANI_FIRE}
}

monster.defenses = {
	defense = 20,
	armor = 15,
	{name ="invisible", interval = 1000, chance = 9, duration = 3000, effect = CONST_ME_MAGIC_RED},
	{name ="outfit", interval = 4000, chance = 9, monster = "green frog", duration = 4000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = "wolf tooth chain", chance = 10000},
	{id = "necrotic rod", chance = 2500},
	{id = "garlic necklace", chance = 8000},
	{id = 2324, chance = 15000},
	{id = "coat", chance = 25000},
	{id = "cape", chance = 15000},
	{id = "cookie", chance = 30000, maxCount = 8},
	{id = "star herb", chance = 20000},
	{id = "spellbook of mind control", chance = 2000}
}

mType:register(monster)
