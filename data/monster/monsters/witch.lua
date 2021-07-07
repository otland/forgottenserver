local mType = Game.createMonsterType("witch")
local monster = {}

monster.name = "Witch"
monster.description = "a witch"
monster.experience = 120
monster.outfit = {
	lookType = 54,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 300
monster.maxHealth = 300
monster.runHealth = 30
monster.race = "blood"
monster.corpse = 20535
monster.speed = 202
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
	{text = "Horax pokti!", yell = false},
	{text = "Herba budinia ex!", yell = false},
	{text = "Hihihihi!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = 100},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = -5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -20, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_FIREDAMAGE, minDamage = -30, maxDamage = -75, range = 7, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_HITBYFIRE},
	{name ="firefield", interval = 2000, chance = 10, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_FIRE},
	{name ="outfit", interval = 2000, chance = 1, monster = "green frog", duration = 2000, range = 5}
}

monster.defenses = {
	defense = 8,
	armor = 8
}

monster.loot = {
	{id = "gold coin", chance = 64000, maxCount = 40},
	{id = "cookie", chance = 30000, maxCount = 8},
	{id = "wolf tooth chain", chance = 10120},
	{id = "witch broom", chance = 10000},
	{id = "star herb", chance = 9000},
	{id = "leather boots", chance = 5000},
	{id = "cape", chance = 4870},
	{id = "sickle", chance = 3910},
	{id = "garlic necklace", chance = 2500},
	{id = "coat", chance = 2010},
	{id = "necrotic rod", chance = 1140},
	{id = "bag of apple slices", chance = 920},
	{id = "silver dagger", chance = 560},
	{id = "witch hat", chance = 90},
	{id = "stuffed toad", chance = 10}
}

mType:register(monster)
