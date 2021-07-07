local mType = Game.createMonsterType("deathstrike")
local monster = {}

monster.name = "Deathstrike"
monster.description = "Deathstrike"
monster.experience = 40000
monster.outfit = {
	lookType = 500,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 200000
monster.maxHealth = 200000
monster.runHealth = 10000
monster.race = "undead"
monster.corpse = 18384
monster.speed = 470
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
	staticAttackChance = 50,
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
	{text = "Deeestructiooon!", yell = false},
	{text = "Maaahhhrrr!", yell = false},
	{text = "You are nothing!", yell = false},
	{text = "Taaake ... this!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = -20},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = -20}
}

monster.attacks = {
	{name ="melee", interval = 1300, chance = 100, minDamage = 0, maxDamage = -540, effect = CONST_ME_DRAWBLOOD},
	{name ="speed", interval = 2000, chance = 20, speed = {min = -400, max = -400}, duration = 2500, range = 7, ShootEffect = CONST_ANI_WHIRLWINDAXE},
	{name ="combat", interval = 2000, chance = 100, type = COMBAT_FIREDAMAGE, minDamage = -820, maxDamage = -950, range = 7, effect = CONST_ME_FIREAREA},
	{name ="combat", interval = 1000, chance = 12, type = COMBAT_ENERGYDAMAGE, minDamage = -350, maxDamage = -800, range = 3, effect = CONST_ME_PURPLEENERGY}
}

monster.defenses = {
	defense = 35,
	armor = 25,
	{name ="combat", interval = 1000, chance = 25, type = COMBAT_HEALING, minDamage = 1000, maxDamage = 5500, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "Deathstrike's snippet", chance = 100000},
	{id = "crystalline sword", chance = 10480},
	{id = "crystalline axe", chance = 11430},
	{id = "mycological mace", chance = 8570},
	{id = "crystal crossbow", chance = 12380},
	{id = "mycological bow", chance = 11430},
	{id = "shiny blade", chance = 11430}
}

mType:register(monster)
