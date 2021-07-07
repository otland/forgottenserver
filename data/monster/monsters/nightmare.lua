local mType = Game.createMonsterType("nightmare")
local monster = {}

monster.name = "Nightmare"
monster.description = "a nightmare"
monster.experience = 2150
monster.outfit = {
	lookType = 245,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 2700
monster.maxHealth = 2700
monster.runHealth = 300
monster.race = "blood"
monster.corpse = 6340
monster.speed = 240
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
	{text = "Close your eyes... I want to show you something.", yell = false},
	{text = "I will haunt you forever!", yell = false},
	{text = "Pffffrrrrrrrrrrrr.", yell = false},
	{text = "I will make you scream.", yell = false},
	{text = "Take a ride with me.", yell = false},
	{text = "Weeeheeheeeheee!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 20},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE , percent = -25},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -150, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_DEATHDAMAGE, minDamage = -120, maxDamage = -170, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_DEATH, effect = CONST_ME_SMALLCLOUDS},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_EARTHDAMAGE, minDamage = -150, maxDamage = -350, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_POISONAREA}
}

monster.defenses = {
	defense = 25,
	armor = 25,
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_HEALING, minDamage = 60, maxDamage = 100, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 2000, chance = 15, speed = {min = 420, max = 420}, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "gold coin", chance = 50000, maxCount = 55},
	{id = "platinum coin", chance = 2564, maxCount = 3},
	{id = "boots of haste", chance = 337},
	{id = "war axe", chance = 95},
	{id = "knight legs", chance = 961},
	{id = "ancient shield", chance = 990},
	{id = "power bolt", chance = 9090, maxCount = 4},
	{id = "ham", chance = 29000, maxCount = 2},
	{id = "mysterious voodoo skull", chance = 123},
	{id = "soul orb", chance = 20000},
	{id = 6300, chance = 1298},
	{id = "demonic essence", chance = 10000},
	{id = "skeleton decoration", chance = 337},
	{id = "concentrated demonic blood", chance = 19666, maxCount = 2},
	{id = "essence of a bad dream", chance = 15240},
	{id = "scythe leg", chance = 9090}
}

mType:register(monster)
