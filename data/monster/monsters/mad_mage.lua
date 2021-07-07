local mType = Game.createMonsterType("mad mage")
local monster = {}

monster.name = "mad mage"
monster.description = "a mad mage"
monster.experience = 1800
monster.outfit = {
	lookType = 394,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 2500
monster.maxHealth = 2500
monster.runHealth = 1000
monster.race = "blood"
monster.corpse = 13603
monster.speed = 240
monster.summonCost = 0
monster.maxSummons = 1

monster.changeTarget = {
	interval = 5000,
	chance = 30
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
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -20},
	{type = COMBAT_ENERGYDAMAGE, percent = 100},
	{type = COMBAT_EARTHDAMAGE, percent = 50},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = -10},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -78, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 1000, chance = 12, type = COMBAT_FIREDAMAGE, minDamage = -50, maxDamage = -185, range = 7, ShootEffect = CONST_ANI_FIRE},
	{name ="combat", interval = 1400, chance = 24, type = COMBAT_MANADRAIN, minDamage = -30, maxDamage = -90, range = 6, ShootEffect = CONST_ANI_ICE},
	{name ="firefield", interval = 1600, chance = 20, range = 7, radius = 2, target = true, ShootEffect = CONST_ANI_FIRE},
	{name ="combat", interval = 1900, chance = 18, type = COMBAT_ENERGYDAMAGE, minDamage = -70, maxDamage = -150, length = 8, spread = 0, effect = CONST_ME_BIGCLOUDS}
}

monster.defenses = {
	defense = 20,
	armor = 20,
	{name ="combat", interval = 1000, chance = 25, type = COMBAT_HEALING, minDamage = 35, maxDamage = 80, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "stone golem", chance = 10, interval = 1000}
}

monster.loot = {
	{id = "gold coin", chance = 100000, maxCount = 66},
	{id = "gold coin", chance = 100000, maxCount = 20},
	{id = "dark mushroom", chance = 5000, maxCount = 5},
	{id = "red piece of cloth", chance = 800},
	{id = "assassin star", chance = 2400, maxCount = 4},
	{id = "strong health potion", chance = 1300, maxCount = 5},
	{id = "strong mana potion", chance = 1200, maxCount = 2},
	{id = "lightning legs", chance = 800}
}

mType:register(monster)
