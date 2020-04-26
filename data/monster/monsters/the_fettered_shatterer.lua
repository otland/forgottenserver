local mType = Game.createMonsterType("the fettered shatterer")
local monster = {}

monster.name = "The Fettered Shatterer"
monster.description = "The Fettered Shatterer"
monster.experience = 58000
monster.outfit = {
	lookType = 842,
	lookHead = 77,
	lookBody = 132,
	lookLegs = 20,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 220000
monster.maxHealth = 220000
monster.runHealth = 2500
monster.race = "fire"
monster.corpse = 6068
monster.speed = 320
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
	targetDistance = 1,
	healthHidden = false,
	canWalkOnEnergy = true,
	canWalkOnFire = true,
	canWalkOnPoison = true
}

monster.light = {
	level = 0,
	color = 0
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "RAAAAR! I! WILL! SHATTER! ALL!!", yell = false},
	{text = "WANT! TO! BREAK! FREE!!", yell = false},
	{text = "CRASH! BOOM! BANG! AHAHAHAAH!", yell = false},
	{text = "I WILL BREAK THE WORLD LIKE GLASS!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 100},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE , percent = 10},
	{type = COMBAT_DEATHDAMAGE , percent = 10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = -200, maxDamage = -3000, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_MANADRAIN, minDamage = -200, maxDamage = -1000, range = 7},
	{name ="combat", interval = 3000, chance = 44, type = COMBAT_PHYSICALDAMAGE, minDamage = -400, maxDamage = -2000, range = 7, ShootEffect = CONST_ANI_WHIRLWINDSWORD, effect = CONST_ME_DRAWBLOOD},
	{name ="speed", interval = 2000, chance = 15, speed = {min = -400, max = -400}, duration = 15000, range = 7, ShootEffect = CONST_ANI_THROWINGKNIFE},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_ICEDAMAGE, minDamage = 0, maxDamage = -800, range = 7, radius = 7, target = false, effect = CONST_ME_BIGPLANTS}
}

monster.defenses = {
	defense = 65,
	armor = 55,
	{name ="combat", interval = 3000, chance = 35, type = COMBAT_HEALING, minDamage = 400, maxDamage = 6000, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 4000, chance = 80, speed = {min = 440, max = 440}, duration = 6000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
}

mType:register(monster)
