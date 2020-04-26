local mType = Game.createMonsterType("adept of the cult")
local monster = {}

monster.name = "Adept of the Cult"
monster.description = "an adept of the cult"
monster.experience = 400
monster.outfit = {
	lookType = 194,
	lookHead = 95,
	lookBody = 94,
	lookLegs = 94,
	lookFeet = 19,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 430
monster.maxHealth = 430
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 20311
monster.speed = 215
monster.summonCost = 0
monster.maxSummons = 2

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
	{text = "Feel the power of the cult!", yell = false},
	{text = "Praise the voodoo!", yell = false},
	{text = "Power to the cult!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 40},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = 30},
	{type = COMBAT_DEATHDAMAGE , percent = -5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -90, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 2, interval = 4000}},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_LIFEDRAIN, minDamage = -70, maxDamage = -150, range = 7, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_POISONAREA},
	{name ="drunk", interval = 2000, chance = 10, drunkenness = 25, duration = 4000, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_HOLY, effect = CONST_ME_HOLYDAMAGE}
}

monster.defenses = {
	defense = 20,
	armor = 20,
	{name ="combat", interval = 3000, chance = 20, type = COMBAT_HEALING, minDamage = 45, maxDamage = 60, effect = CONST_ME_MAGIC_BLUE},
	{name ="invisible", interval = 2000, chance = 10, duration = 3000, effect = CONST_ME_YELLOW_RINGS}
}

monster.summons = {
	{name = "Ghoul", chance = 10, interval = 2000}
}

monster.loot = {
	{id = 1962, chance = 940},
	{id = "small ruby", chance = 320},
	{id = "gold coin", chance = 65520, maxCount = 60},
	{id = 2169, chance = 420},
	{id = "silver amulet", chance = 1020},
	{id = "hailstorm rod", chance = 220},
	{id = "clerical mace", chance = 1260},
	{id = "red robe", chance = 80},
	{id = "pirate voodoo doll", chance = 1730},
	{id = 6089, chance = 700},
	{id = "lunar staff", chance = 120},
	{id = "amber staff", chance = 680},
	{id = "cultish robe", chance = 10080},
	{id = "cultish symbol", chance = 90},
	{id = "rope belt", chance = 10000},
	{id = "broken key ring", chance = 120}
}

mType:register(monster)
