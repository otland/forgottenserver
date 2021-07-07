local mType = Game.createMonsterType("draptor")
local monster = {}

monster.name = "Draptor"
monster.description = "a draptor"
monster.experience = 2400
monster.outfit = {
	lookType = 382,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 3000
monster.maxHealth = 3000
monster.runHealth = 350
monster.race = "blood"
monster.corpse = 13316
monster.speed = 282
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
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_ENERGYDAMAGE, percent = 100},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -150, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 3000, chance = 30, type = COMBAT_ENERGYDAMAGE, minDamage = -130, maxDamage = -310, radius = 3, target = false, effect = CONST_ME_YELLOWENERGY},
	{name ="combat", interval = 3000, chance = 35, type = COMBAT_ENERGYDAMAGE, minDamage = -200, maxDamage = -300, range = 7, ShootEffect = CONST_ANI_ENERGY},
	{name ="combat", interval = 2500, chance = 17, type = COMBAT_FIREDAMAGE, minDamage = -70, maxDamage = -250, length = 8, spread = 3, effect = CONST_ME_FIREAREA}
}

monster.defenses = {
	defense = 37,
	armor = 23,
	{name ="combat", interval = 1000, chance = 25, type = COMBAT_HEALING, minDamage = 57, maxDamage = 93, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 2000, chance = 12, speed = {min = 457, max = 457}, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = "gold coin", chance = 33750, maxCount = 90},
	{id = "gold coin", chance = 33750, maxCount = 60},
	{id = "strong health potion", chance = 3150},
	{id = "strong mana potion", chance = 4150},
	{id = "dragon robe", chance = 950},
	{id = "draptor scales", chance = 6650}
}

mType:register(monster)
