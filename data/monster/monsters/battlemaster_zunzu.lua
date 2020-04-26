local mType = Game.createMonsterType("battlemaster zunzu")
local monster = {}

monster.name = "Battlemaster Zunzu"
monster.description = "Battlemaster Zunzu"
monster.experience = 2500
monster.outfit = {
	lookType = 343,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 4000
monster.maxHealth = 4000
monster.runHealth = 150
monster.race = "blood"
monster.corpse = 11281
monster.speed = 420
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
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 15},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 25},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 15},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -300, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_EARTHDAMAGE, minDamage = -115, maxDamage = -350, range = 1, radius = 1, target = true, effect = CONST_ME_POISONAREA}
}

monster.defenses = {
	defense = 35,
	armor = 45,
	{name ="combat", interval = 1000, chance = 18, type = COMBAT_HEALING, minDamage = 200, maxDamage = 400, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "platinum coin", chance = 26800, maxCount = 3},
	{id = "lizard leather", chance = 1825},
	{id = "lizard scale", chance = 800},
	{id = "strong health potion", chance = 2875, maxCount = 2},
	{id = "great health potion", chance = 2775, maxCount = 2},
	{id = "red lantern", chance = 100000},
	{id = "Zaoan armor", chance = 1050},
	{id = "Zaoan shoes", chance = 3150},
	{id = "Zaoan legs", chance = 2625},
	{id = "zaogun flag", chance = 11250},
	{id = "zaogun shoulderplates", chance = 100000}
}

mType:register(monster)
