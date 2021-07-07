local mType = Game.createMonsterType("dromedary")
local monster = {}

monster.name = "Dromedary"
monster.description = "a dromedary"
monster.experience = 0
monster.outfit = {
	lookType = 404,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 45
monster.maxHealth = 45
monster.runHealth = 45
monster.race = "blood"
monster.corpse = 13528
monster.speed = 170
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 20
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = false,
	convinceable = false,
	pushable = true,
	boss = false,
	illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	staticAttackChance = 90,
	targetDistance = 4,
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
	{text = "Snort", yell = false},
	{text = "Grunt!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -8, effect = CONST_ME_DRAWBLOOD},
	{name ="drunk", interval = 4000, chance = 5, drunkenness = 25, duration = 6000, range = 1, ShootEffect = CONST_ANI_EXPLOSION, effect = CONST_ME_STUN}
}

monster.defenses = {
	defense = 5,
	armor = 5
}

monster.loot = {
	{id = "meat", chance = 36000, maxCount = 2},
	{id = "sugar oat", chance = 1000}
}

mType:register(monster)
