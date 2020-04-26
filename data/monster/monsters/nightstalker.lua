local mType = Game.createMonsterType("nightstalker")
local monster = {}

monster.name = "Nightstalker"
monster.description = "a nightstalker"
monster.experience = 500
monster.outfit = {
	lookType = 320,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 700
monster.maxHealth = 700
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 9915
monster.speed = 260
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 0,
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
	{text = "The sunlight is so depressing.", yell = false},
	{text = "Come with me, my child.", yell = false},
	{text = "I've been in the shadow under your bed last night.", yell = false},
	{text = "You never know what hides in the night.", yell = false},
	{text = "I remember your face - and I know where you sleep.", yell = false},
	{text = "Only the sweetest and cruelest dreams for you, my love.", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = 20},
	{type = COMBAT_DEATHDAMAGE , percent = -5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -90, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 80, interval = 4000}},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_LIFEDRAIN, minDamage = -60, maxDamage = -170, range = 7, effect = CONST_ME_HOLYAREA},
	{name ="speed", interval = 2000, chance = 15, speed = {min = -600, max = -600}, duration = 15000, effect = CONST_ME_SLEEP}
}

monster.defenses = {
	defense = 15,
	armor = 15,
	{name ="speed", interval = 2000, chance = 15, speed = {min = 240, max = 240}, duration = 5000, effect = CONST_ME_MAGIC_RED},
	{name ="invisible", interval = 2000, chance = 10, duration = 4000, effect = CONST_ME_YELLOW_RINGS},
	{name ="outfit", interval = 5000, chance = 10, monster = "nightstalker", duration = 4000},
	{name ="outfit", interval = 5000, chance = 10, monster = "werewolf", duration = 4000},
	{name ="outfit", interval = 5000, chance = 10, monster = "the count", duration = 4000},
	{name ="outfit", interval = 5000, chance = 10, monster = "grim reaper", duration = 4000},
	{name ="outfit", interval = 5000, chance = 10, monster = "tarantula", duration = 4000},
	{name ="outfit", interval = 5000, chance = 1, monster = "ferumbras", duration = 4000}
}

monster.loot = {
	{id = 2124, chance = 1030},
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "gold coin", chance = 50000, maxCount = 10},
	{id = "platinum amulet", chance = 121},
	{id = "boots of haste", chance = 121},
	{id = "protection amulet", chance = 847},
	{id = "shadow herb", chance = 4761},
	{id = "haunted blade", chance = 318},
	{id = "chaos mace", chance = 121},
	{id = "strong mana potion", chance = 1612},
	{id = "spirit cloak", chance = 520},
	{id = "crystal of balance", chance = 127}
}

mType:register(monster)
