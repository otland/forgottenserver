local mType = Game.createMonsterType("countess sorrow")
local monster = {}

monster.name = "Countess Sorrow"
monster.description = "Countess Sorrow"
monster.experience = 13000
monster.outfit = {
	lookType = 241,
	lookHead = 20,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 6500
monster.maxHealth = 6500
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 6344
monster.speed = 250
monster.summonCost = 0
monster.maxSummons = 3

monster.changeTarget = {
	interval = 60000,
	chance = 0
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
	{type = "paralyze", condition = true},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 100},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 50},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -360, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 50, interval = 4000}},
	{name ="combat", interval = 1000, chance = 10, type = COMBAT_EARTHDAMAGE, minDamage = -150, maxDamage = -350, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_POISONAREA},
	{name ="poisonfield", interval = 1000, chance = 17, range = 7, radius = 3, target = true, ShootEffect = CONST_ANI_POISON},
	{name ="combat", interval = 1000, chance = 25, type = COMBAT_MANADRAIN, minDamage = -100, maxDamage = -400, range = 7, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 20,
	armor = 25,
	{name ="combat", interval = 1000, chance = 50, type = COMBAT_HEALING, minDamage = 409, maxDamage = 812, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "Phantasm", chance = 20, interval = 2000},
	{name = "Phantasm Summon", chance = 20, interval = 2000}
}

monster.loot = {
	{id = "Countess Sorrow's frozen tear", chance = 100000}
}

mType:register(monster)
