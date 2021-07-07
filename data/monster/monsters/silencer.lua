local mType = Game.createMonsterType("silencer")
local monster = {}

monster.name = "Silencer"
monster.description = "a silencer"
monster.experience = 5100
monster.outfit = {
	lookType = 585,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 5400
monster.maxHealth = 5400
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 22489
monster.speed = 230
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 5
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
	{text = "Prrrroooaaaah!!! PRROAAAH!!", yell = false},
	{text = "PRRRROOOOOAAAAAHHHH!!!", yell = false},
	{text = "HUUUSSSSSSSSH!!", yell = false},
	{text = "Hussssssh!!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 5},
	{type = COMBAT_ENERGYDAMAGE, percent = 15},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 30},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE , percent = -25},
	{type = COMBAT_DEATHDAMAGE , percent = 70}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -315, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 600, interval = 4000}},
	{name ="silencer skill reducer", interval = 2000, chance = 10, range = 3},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_MANADRAIN, minDamage = -40, maxDamage = -150, radius = 4, target = true, ShootEffect = CONST_ANI_ONYXARROW, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 20,
	armor = 20,
	{name ="speed", interval = 2000, chance = 15, speed = {min = 450, max = 450}, duration = 5000, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_HEALING, minDamage = 220, maxDamage = 425, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "gold coin", chance = 100000, maxCount = 100},
	{id = "platinum coin", chance = 100000, maxCount = 8},
	{id = "stealth ring", chance = 1200},
	{id = "boots of haste", chance = 360},
	{id = "dark shield", chance = 2000},
	{id = "assassin star", chance = 7600, maxCount = 10},
	{id = "diamond sceptre", chance = 960},
	{id = "haunted blade", chance = 2000},
	{id = "titan axe", chance = 2200},
	{id = "shadow sceptre", chance = 640},
	{id = "glorious axe", chance = 2400},
	{id = "terra legs", chance = 960},
	{id = "terra boots", chance = 480},
	{id = "cluster of solace", chance = 560},
	{id = "silencer claws", chance = 17000},
	{id = "silencer resonating chamber", chance = 8410}
}

mType:register(monster)
