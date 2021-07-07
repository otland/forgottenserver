local mType = Game.createMonsterType("crystal spider")
local monster = {}

monster.name = "Crystal Spider"
monster.description = "a crystal spider"
monster.experience = 900
monster.outfit = {
	lookType = 263,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1250
monster.maxHealth = 1250
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 7344
monster.speed = 220
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 15
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
	staticAttackChance = 80,
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
	{text = "Screeech!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = -20},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -250, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 160, interval = 4000}},
	{name ="speed", interval = 2000, chance = 15, speed = {min = -800, max = -800}, duration = 15000, range = 7, radius = 6, target = false, effect = CONST_ME_POFF},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_ICEDAMAGE, minDamage = -50, maxDamage = -100, range = 7, ShootEffect = CONST_ANI_ICE, effect = CONST_ME_ICEAREA},
	{name ="speed", interval = 2000, chance = 20, speed = {min = -600, max = -600}, duration = 10000, range = 7, ShootEffect = CONST_ANI_SNOWBALL}
}

monster.defenses = {
	defense = 40,
	armor = 40,
	{name ="speed", interval = 2000, chance = 15, speed = {min = 250, max = 250}, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = 2124, chance = 1560},
	{id = "crystal necklace", chance = 1730},
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "gold coin", chance = 50000, maxCount = 92},
	{id = 2169, chance = 1587},
	{id = "platinum amulet", chance = 120},
	{id = "steel helmet", chance = 4545},
	{id = "plate armor", chance = 10000},
	{id = "plate armor", chance = 5263},
	{id = "knight armor", chance = 540},
	{id = "knight legs", chance = 690},
	{id = "jewelled backpack", chance = 70},
	{id = "spider silk", chance = 1950},
	{id = "shard", chance = 750},
	{id = "sniper arrow", chance = 6250, maxCount = 6},
	{id = "sapphire hammer", chance = 130},
	{id = "crystal sword", chance = 2564},
	{id = "strong mana potion", chance = 14810},
	{id = "glacier mask", chance = 650}
}

mType:register(monster)
