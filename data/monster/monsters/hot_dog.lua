local mType = Game.createMonsterType("hot dog")
local monster = {}

monster.name = "Hot Dog"
monster.description = "a hot dog"
monster.experience = 190
monster.outfit = {
	lookType = 32,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 505
monster.maxHealth = 505
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 5971
monster.speed = 300
monster.summonCost = 220
monster.maxSummons = 0

monster.changeTarget = {
	interval = 2000,
	chance = 0
}

monster.flags = {
	summonable = true,
	attackable = true,
	hostile = true,
	convinceable = true,
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
	{text = "Wuff Wuff", yell = false},
	{text = "Grrr Wuff", yell = false},
	{text = "Show me how good you are without some rolled newspaper!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -20},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = -5},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 1200, chance = 100, minDamage = 0, maxDamage = -55, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 30, type = COMBAT_FIREDAMAGE, minDamage = -30, maxDamage = -60, length = 8, spread = 3, effect = CONST_ME_HITBYFIRE},
	{name ="combat", interval = 1000, chance = 30, type = COMBAT_FIREDAMAGE, minDamage = -50, maxDamage = -50, range = 7, effect = CONST_ME_FIREATTACK}
}

monster.defenses = {
	defense = 2,
	armor = 1
}

monster.loot = {
	{id = "gold coin", chance = 50000, maxCount = 75}
}

mType:register(monster)
