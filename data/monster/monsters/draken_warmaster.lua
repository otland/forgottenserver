local mType = Game.createMonsterType("draken warmaster")
local monster = {}

monster.name = "Draken Warmaster"
monster.description = "a draken warmaster"
monster.experience = 2400
monster.outfit = {
	lookType = 334,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 4150
monster.maxHealth = 4150
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 11107
monster.speed = 220
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
	{text = "Attack aggrezzively! Dezztroy zze intruderzz!", yell = false},
	{text = "Hizzzzz!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 5},
	{type = COMBAT_ENERGYDAMAGE, percent = 5},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE , percent = 5},
	{type = COMBAT_DEATHDAMAGE , percent = 50}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -300, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_FIREDAMAGE, minDamage = -240, maxDamage = -520, length = 4, spread = 3, effect = CONST_ME_EXPLOSIONHIT}
}

monster.defenses = {
	defense = 40,
	armor = 40,
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_HEALING, minDamage = 510, maxDamage = 600, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "ring of the sky", chance = 180},
	{id = "small ruby", chance = 1525, maxCount = 5},
	{id = "gold coin", chance = 47000, maxCount = 100},
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "platinum coin", chance = 50625, maxCount = 5},
	{id = "tower shield", chance = 2035},
	{id = "meat", chance = 30300},
	{id = "great health potion", chance = 4850, maxCount = 3},
	{id = "ultimate health potion", chance = 4020},
	{id = "Zaoan armor", chance = 790},
	{id = "Zaoan shoes", chance = 1900},
	{id = "Zaoan legs", chance = 960},
	{id = "drakinata", chance = 860},
	{id = "bone shoulderplate", chance = 12010},
	{id = "warmaster's wristguards", chance = 7000},
	{id = "Zaoan halberd", chance = 7925}
}

mType:register(monster)
