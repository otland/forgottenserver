local mType = Game.createMonsterType("mephiles")
local monster = {}

monster.name = "Mephiles"
monster.description = "Mephiles"
monster.experience = 415
monster.outfit = {
	lookType = 237,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 415
monster.maxHealth = 415
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 10323
monster.speed = 300
monster.summonCost = 0
monster.maxSummons = 0

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
	targetDistance = 3,
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
	{text = "I have a contract here which you should sign!", yell = false},
	{text = "I sence so much potential in you. It's almost a shame I have to kill you.", yell = false},
	{text = "Yes, slay me for the loot I might have. Give in to your greed.", yell = false},
	{text = "Wealth, Power, it is all at your fingertips. All you have to do is a bit blackmailing and bullying.", yell = false},
	{text = "Come on. being a bit evil won't hurt you.", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 50},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE , percent = -5},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 1200, chance = 100, minDamage = 0, maxDamage = -35, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 1500, chance = 70, type = COMBAT_FIREDAMAGE, minDamage = -15, maxDamage = -45, range = 7, radius = 2, target = true, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_FIREAREA}
}

monster.defenses = {
	defense = 35,
	armor = 30,
	{name ="speed", interval = 1000, chance = 40, speed = {min = 400, max = 400}, duration = 40000, effect = CONST_ME_MAGIC_RED},
	{name ="invisible", interval = 4000, chance = 50, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
}

mType:register(monster)
