local mType = Game.createMonsterType("versperoth")
local monster = {}

monster.name = "Versperoth"
monster.description = "Versperoth"
monster.experience = 20000
monster.outfit = {
	lookType = 295,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 100000
monster.maxHealth = 100000
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 0
monster.speed = 0
monster.summonCost = 0
monster.maxSummons = 7

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
	{text = "GrrroaR!", yell = false},
	{text = "GROWL!", yell = false},
	{text = "Waaaah!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 90},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 1200, chance = 100, minDamage = 0, maxDamage = -650, effect = CONST_ME_DRAWBLOOD},
	{name ="firefield", interval = 1200, chance = 30, range = 7, radius = 4, target = false, ShootEffect = CONST_ANI_FIRE},
	{name ="combat", interval = 1000, chance = 40, type = COMBAT_FIREDAMAGE, minDamage = -700, maxDamage = -1615, range = 5, length = 8, spread = 3, effect = CONST_ME_FIREAREA},
	{name ="melee", interval = 1000, chance = 25, minDamage = -750, maxDamage = -950, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 55,
	armor = 55
}

monster.summons = {
	{name = "Minion of Vesperoth", chance = 16, interval = 2000, max = 4}
}

monster.loot = {
}

mType:register(monster)
