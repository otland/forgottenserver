local mType = Game.createMonsterType("vashresamun")
local monster = {}

monster.name = "Vashresamun"
monster.description = "Vashresamun"
monster.experience = 2950
monster.outfit = {
	lookType = 90,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 4000
monster.maxHealth = 4000
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 6025
monster.speed = 340
monster.summonCost = 0
monster.maxSummons = 2

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
	{text = "If music is the food of death, drop dead.", yell = false},
	{text = "Are you enjoying my music?", yell = false},
	{text = "Come my maidens, we have visitors!", yell = false},
	{text = "Chakka Chakka!", yell = false},
	{text = "Heheheheee!", yell = false}
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
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 20},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = -10},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -200, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 65, interval = 4000}},
	{name ="combat", interval = 2000, chance = 30, type = COMBAT_DEATHDAMAGE, minDamage = -200, maxDamage = -600, radius = 5, target = false, effect = CONST_ME_SOUND_PURPLE},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_LIFEDRAIN, minDamage = 0, maxDamage = -800, range = 1}
}

monster.defenses = {
	defense = 40,
	armor = 40,
	{name ="combat", interval = 1000, chance = 20, type = COMBAT_HEALING, minDamage = 250, maxDamage = 450, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 1000, chance = 12, speed = {min = 370, max = 370}, duration = 30000, range = 7, effect = CONST_ME_MAGIC_RED}
}

monster.summons = {
	{name = "Banshee", chance = 20, interval = 2000}
}

monster.loot = {
	{id = "blue note", chance = 100000},
	{id = "gold coin", chance = 31000, maxCount = 100},
	{id = "gold coin", chance = 31000, maxCount = 100},
	{id = "gold coin", chance = 31000, maxCount = 50},
	{id = "white pearl", chance = 10000},
	{id = 2072, chance = 9200},
	{id = "great mana potion", chance = 8000},
	{id = "blue robe", chance = 1200},
	{id = "crystal mace", chance = 1000},
	{id = 2124, chance = 1000},
	{id = 2074, chance = 330},
	{id = "ancient tiara", chance = 170},
	{id = "mini mummy", chance = 170}
}

mType:register(monster)
