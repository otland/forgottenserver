local mType = Game.createMonsterType("warlock")
local monster = {}

monster.name = "Warlock"
monster.description = "a warlock"
monster.experience = 4000
monster.outfit = {
	lookType = 130,
	lookHead = 0,
	lookBody = 52,
	lookLegs = 128,
	lookFeet = 95,
	lookAddons = 1,
	lookMount = 0
}

monster.health = 3500
monster.maxHealth = 3500
monster.runHealth = 900
monster.race = "blood"
monster.corpse = 20527
monster.speed = 230
monster.summonCost = 0
monster.maxSummons = 1

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
	{text = "Learn the secret of our magic! YOUR death!", yell = false},
	{text = "Even a rat is a better mage than you.", yell = false},
	{text = "We don't like intruders!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = 100},
	{type = COMBAT_EARTHDAMAGE, percent = 95},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = -8},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -130, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_ENERGYDAMAGE, minDamage = -90, maxDamage = -180, range = 7, ShootEffect = CONST_ANI_ENERGY},
	{name ="warlock skill reducer", interval = 2000, chance = 5, range = 5},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_MANADRAIN, minDamage = 0, maxDamage = -120, range = 7},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_FIREDAMAGE, minDamage = -50, maxDamage = -180, range = 7, radius = 3, target = true, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_FIREAREA},
	{name ="firefield", interval = 2000, chance = 10, range = 7, radius = 2, target = true, ShootEffect = CONST_ANI_FIRE},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_ENERGYDAMAGE, minDamage = -150, maxDamage = -230, length = 8, spread = 0, effect = CONST_ME_BIGCLOUDS},
	{name ="speed", interval = 2000, chance = 15, speed = {min = -600, max = -600}, duration = 20000, range = 7, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 33,
	armor = 33,
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_HEALING, minDamage = 100, maxDamage = 225, effect = CONST_ME_MAGIC_BLUE},
	{name ="invisible", interval = 2000, chance = 20, duration = 10000, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "stone golem", chance = 10, interval = 2000}
}

monster.loot = {
	{id = "gold coin", chance = 30000, maxCount = 80},
	{id = "cherry", chance = 19000, maxCount = 4},
	{id = "bread", chance = 9000},
	{id = "poison dagger", chance = 7600},
	{id = "skull staff", chance = 6370},
	{id = "great health potion", chance = 5000},
	{id = "great mana potion", chance = 5000},
	{id = "assassin star", chance = 3500, maxCount = 4},
	{id = "dark mushroom", chance = 3000},
	{id = "mind stone", chance = 2000},
	{id = "energy ring", chance = 2000},
	{id = "blue robe", chance = 1500},
	{id = "candlestick", chance = 1500},
	{id = "small sapphire", chance = 1200},
	{id = "talon", chance = 1150},
	{id = "lightning robe", chance = 1000},
	{id = 2600, chance = 1000},
	{id = 2124, chance = 730},
	{id = "luminous orb", chance = 450},
	{id = "ring of the sky", chance = 380},
	{id = "stone skin amulet", chance = 330},
	{id = "red tome", chance = 240},
	{id = "golden armor", chance = 210},
	{id = "piggy bank", chance = 60}
}

mType:register(monster)
