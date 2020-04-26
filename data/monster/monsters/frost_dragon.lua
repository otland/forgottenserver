local mType = Game.createMonsterType("frost dragon")
local monster = {}

monster.name = "Frost Dragon"
monster.description = "a frost dragon"
monster.experience = 2100
monster.outfit = {
	lookType = 248,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1800
monster.maxHealth = 1800
monster.runHealth = 250
monster.race = "undead"
monster.corpse = 7091
monster.speed = 240
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = false,
	boss = false,
	illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	staticAttackChance = 70,
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
	{text = "YOU WILL FREEZE!", yell = true},
	{text = "ZCHHHHH!", yell = true},
	{text = "I am so cool", yell = false},
	{text = "Chill out!.", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -225, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_LIFEDRAIN, minDamage = -175, maxDamage = -380, length = 8, spread = 3, effect = CONST_ME_POFF},
	{name ="speed", interval = 2000, chance = 5, speed = {min = -700, max = -700}, duration = 12000, radius = 3, target = false, effect = CONST_ME_POFF},
	{name ="speed", interval = 2000, chance = 15, speed = {min = -850, max = -850}, duration = 18000, length = 7, spread = 3, effect = CONST_ME_ICEATTACK},
	{name ="combat", interval = 2000, chance = 5, type = COMBAT_ICEDAMAGE, minDamage = -60, maxDamage = -120, radius = 3, target = false, effect = CONST_ME_ICETORNADO},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_ICEDAMAGE, minDamage = -100, maxDamage = -240, radius = 4, target = true, effect = CONST_ME_ICEAREA},
	{name ="combat", interval = 2000, chance = 5, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -220, length = 1, spread = 0, effect = CONST_ME_POFF},
	{name ="speed", interval = 2000, chance = 20, speed = {min = -600, max = -600}, duration = 12000, radius = 4, target = true, effect = CONST_ME_ICEAREA}
}

monster.defenses = {
	defense = 45,
	armor = 45,
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_HEALING, minDamage = 150, maxDamage = 200, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 2000, chance = 15, speed = {min = 290, max = 290}, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = 1976, chance = 8500},
	{id = "golden mug", chance = 3000},
	{id = "small sapphire", chance = 5200},
	{id = "gold coin", chance = 33000, maxCount = 100},
	{id = "gold coin", chance = 33000, maxCount = 100},
	{id = "gold coin", chance = 33000, maxCount = 42},
	{id = "energy ring", chance = 5000},
	{id = "life crystal", chance = 520},
	{id = "ice rapier", chance = 350},
	{id = "strange helmet", chance = 450},
	{id = "dragon scale mail", chance = 80},
	{id = "royal helmet", chance = 210},
	{id = "tower shield", chance = 340},
	{id = "power bolt", chance = 6000, maxCount = 6},
	{id = "dragon ham", chance = 80370, maxCount = 5},
	{id = "green mushroom", chance = 12000},
	{id = "shard", chance = 550},
	{id = "dragon slayer", chance = 120},
	{id = 7441, chance = 4000}
}

mType:register(monster)
