local mType = Game.createMonsterType("destroyer")
local monster = {}

monster.name = "Destroyer"
monster.description = "a destroyer"
monster.experience = 2500
monster.outfit = {
	lookType = 236,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 3700
monster.maxHealth = 3700
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 6320
monster.speed = 270
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
	illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	staticAttackChance = 70,
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
	{text = "COME HERE AND DIE!", yell = false},
	{text = "Destructiooooon!", yell = false},
	{text = "It's a good day to destroy!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 25},
	{type = COMBAT_ENERGYDAMAGE, percent = 100},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 30},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -15},
	{type = COMBAT_HOLYDAMAGE , percent = -3},
	{type = COMBAT_DEATHDAMAGE , percent = 20}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -500, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -200, range = 7, ShootEffect = CONST_ANI_LARGEROCK}
}

monster.defenses = {
	defense = 30,
	armor = 30,
	{name ="speed", interval = 2000, chance = 15, speed = {min = 420, max = 420}, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = "crystal necklace", chance = 578},
	{id = "gold coin", chance = 60000, maxCount = 100},
	{id = "gold coin", chance = 40000, maxCount = 100},
	{id = "gold coin", chance = 40000, maxCount = 100},
	{id = "gold coin", chance = 60000, maxCount = 41},
	{id = "small amethyst", chance = 7692, maxCount = 2},
	{id = "platinum coin", chance = 4166, maxCount = 3},
	{id = "mind stone", chance = 564},
	{id = "giant sword", chance = 1694},
	{id = "crowbar", chance = 14285},
	{id = "plate armor", chance = 4347},
	{id = "dark armor", chance = 10000},
	{id = "burst arrow", chance = 12500, maxCount = 12},
	{id = "pick", chance = 6250},
	{id = "steel boots", chance = 992},
	{id = "meat", chance = 50000, maxCount = 6},
	{id = "skull helmet", chance = 108},
	{id = "soul orb", chance = 6666},
	{id = 6300, chance = 144},
	{id = "demonic essence", chance = 20000},
	{id = "dreaded cleaver", chance = 833},
	{id = "chaos mace", chance = 869},
	{id = "great health potion", chance = 1136},
	{id = "metal spike", chance = 7142}
}

mType:register(monster)
