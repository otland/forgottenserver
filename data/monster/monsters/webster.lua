local mType = Game.createMonsterType("webster")
local monster = {}

monster.name = "Webster"
monster.description = "Webster"
monster.experience = 1200
monster.outfit = {
	lookType = 263,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 2950
monster.maxHealth = 2950
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 7349
monster.speed = 290
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 0,
	chance = 0
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
	canPushCreatures = false,
	staticAttackChance = 95,
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
	{text = "You are lost!", yell = false},
	{text = "Come my little morsel.", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = -20},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -250, effect = CONST_ME_DRAWBLOOD},
	{name ="speed", interval = 3500, chance = 65, speed = {min = -500, max = -500}, duration = 40, range = 1, radius = 1, target = true, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 3000, chance = 75, type = COMBAT_ENERGYDAMAGE, minDamage = -13, maxDamage = -80, range = 7, ShootEffect = CONST_ANI_ENERGY}
}

monster.defenses = {
	defense = 34,
	armor = 29,
	{name ="speed", interval = 5000, chance = 100, speed = {min = 500, max = 500}, duration = 2500, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
}

mType:register(monster)
