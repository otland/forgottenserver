local mType = Game.createMonsterType("gnomevil")
local monster = {}

monster.name = "Gnomevil"
monster.description = "Gnomevil"
monster.experience = 45000
monster.outfit = {
	lookType = 504,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 250000
monster.maxHealth = 250000
monster.runHealth = 10000
monster.race = "blood"
monster.corpse = 18443
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
	{text = "No more Mr. Nicegnome!", yell = false},
	{text = "Taste the power of the dark side!", yell = false},
	{text = "I've seen the light! And it was all engulfing fire!", yell = false},
	{text = "Tatatata!!!", yell = false},
	{text = "Muhahaha!", yell = false},
	{text = "I'm feeling grrrreat!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 10},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = -20}
}

monster.attacks = {
	{name ="melee", interval = 1300, chance = 100, minDamage = 0, maxDamage = -630, effect = CONST_ME_DRAWBLOOD},
	{name ="speed", interval = 2000, chance = 20, speed = {min = -400, max = -400}, duration = 2500, range = 7, ShootEffect = CONST_ANI_WHIRLWINDAXE},
	{name ="combat", interval = 2000, chance = 100, type = COMBAT_FIREDAMAGE, minDamage = -820, maxDamage = -950, range = 7, effect = CONST_ME_ENERGYAREA},
	{name ="combat", interval = 2000, chance = 9, type = COMBAT_MANADRAIN, minDamage = -230, maxDamage = -500, length = 8, spread = 0, effect = CONST_ME_MAGIC_BLUE},
	{name ="combat", interval = 1000, chance = 12, type = COMBAT_ENERGYDAMAGE, minDamage = -350, maxDamage = -800, range = 3, effect = CONST_ME_PURPLEENERGY}
}

monster.defenses = {
	defense = 35,
	armor = 25,
	{name ="combat", interval = 1000, chance = 25, type = COMBAT_HEALING, minDamage = 1000, maxDamage = 4000, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "decorative ribbon", chance = 1710},
	{id = "crystalline sword", chance = 14100},
	{id = "crystalline axe", chance = 9830},
	{id = "mycological mace", chance = 8120},
	{id = "crystal crossbow", chance = 8120},
	{id = "mycological bow", chance = 9400},
	{id = "shiny blade", chance = 7690},
	{id = "Gnomevil's hat", chance = 99150}
}

mType:register(monster)
