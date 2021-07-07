local mType = Game.createMonsterType("giant spider")
local monster = {}

monster.name = "Giant Spider"
monster.description = "a giant spider"
monster.experience = 900
monster.outfit = {
	lookType = 38,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1300
monster.maxHealth = 1300
monster.runHealth = 0
monster.race = "venom"
monster.corpse = 5977
monster.speed = 230
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
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -300, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 160, interval = 4000}},
	{name ="poisonfield", interval = 2000, chance = 10, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_POISON},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_EARTHDAMAGE, minDamage = -40, maxDamage = -70, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_POISON}
}

monster.defenses = {
	defense = 30,
	armor = 30,
	{name ="speed", interval = 2000, chance = 15, speed = {min = 390, max = 390}, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.summons = {
	{name = "Poison Spider", chance = 10, interval = 2000}
}

monster.loot = {
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "gold coin", chance = 50000, maxCount = 95},
	{id = "poison arrow", chance = 12500, maxCount = 12},
	{id = "plate armor", chance = 10000},
	{id = "plate legs", chance = 8000},
	{id = "two handed sword", chance = 5000},
	{id = "steel helmet", chance = 5000},
	{id = "strong health potion", chance = 3571},
	{id = "spider silk", chance = 2000},
	{id = "knight legs", chance = 870},
	{id = 2169, chance = 710},
	{id = "knight armor", chance = 500},
	{id = "platinum amulet", chance = 280},
	{id = "lightning headband", chance = 270}
}

mType:register(monster)
