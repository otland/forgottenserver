local mType = Game.createMonsterType("pirate buccaneer")
local monster = {}

monster.name = "Pirate Buccaneer"
monster.description = "a pirate buccaneer"
monster.experience = 250
monster.outfit = {
	lookType = 97,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 425
monster.maxHealth = 425
monster.runHealth = 50
monster.race = "blood"
monster.corpse = 20471
monster.speed = 220
monster.summonCost = 595
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 15
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
	{text = "Give up!", yell = false},
	{text = "Hiyaa", yell = false},
	{text = "Plundeeeeer!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 10},
	{type = COMBAT_FIREDAMAGE, percent = -5},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE , percent = 10},
	{type = COMBAT_DEATHDAMAGE , percent = -5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -160, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -100, range = 4, ShootEffect = CONST_ANI_THROWINGKNIFE}
}

monster.defenses = {
	defense = 30,
	armor = 30
}

monster.loot = {
	{id = 2050, chance = 10190},
	{id = "gold coin", chance = 67740, maxCount = 59},
	{id = 2238, chance = 9900},
	{id = "sabre", chance = 10100},
	{id = "throwing knife", chance = 9000, maxCount = 5},
	{id = "plate armor", chance = 1130},
	{id = "battle shield", chance = 3850},
	{id = 5091, chance = 1000},
	{id = "rum flask", chance = 120},
	{id = 5792, chance = 40},
	{id = "pirate backpack", chance = 430},
	{id = "pirate shirt", chance = 1200},
	{id = "hook", chance = 450},
	{id = "eye patch", chance = 420},
	{id = "peg leg", chance = 510},
	{id = "strong health potion", chance = 670},
	{id = "compass", chance = 9780}
}

mType:register(monster)
