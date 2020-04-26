local mType = Game.createMonsterType("mutated human")
local monster = {}

monster.name = "Mutated Human"
monster.description = "a mutated human"
monster.experience = 150
monster.outfit = {
	lookType = 323,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 240
monster.maxHealth = 240
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 9107
monster.speed = 200
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
	{text = "Take that creature off my back!! I can fell it!", yell = false},
	{text = "HEEEEEEEELP!", yell = false},
	{text = "You will be the next infected one... GRAAAAAAAAARRR!", yell = false},
	{text = "Science... is a curse.", yell = false},
	{text = "Run as fast as you can.", yell = false},
	{text = "Oh by the gods! What is this... aaaaaargh!", yell = false}
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
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = -25},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -90, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 60, interval = 4000}},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_DEATHDAMAGE, minDamage = -50, maxDamage = -60, length = 3, spread = 1, effect = CONST_ME_POISONAREA},
	{name ="condition", interval = 2000, chance = 20, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -190, maxDamage = -280, length = 1, spread = 0, effect = CONST_ME_HITBYPOISON},
	{name ="speed", interval = 2000, chance = 10, speed = {min = -600, max = -600}, duration = 30000, range = 7, effect = CONST_ME_STUN}
}

monster.defenses = {
	defense = 15,
	armor = 15,
	{name ="speed", interval = 2000, chance = 15, speed = {min = 220, max = 220}, duration = 5000, effect = CONST_ME_GREEN_RINGS}
}

monster.loot = {
	{id = "gold coin", chance = 44000, maxCount = 80},
	{id = "gold coin", chance = 44000, maxCount = 50},
	{id = "strange talisman", chance = 4980},
	{id = "silver amulet", chance = 70},
	{id = "fishbone", chance = 10050},
	{id = 2376, chance = 5030},
	{id = "scale armor", chance = 8080},
	{id = 2696, chance = 8000},
	{id = "fern", chance = 420},
	{id = "worm", chance = 7110, maxCount = 2},
	{id = "peanut", chance = 580},
	{id = 9808, chance = 190},
	{id = "mutated flesh", chance = 19940}
}

mType:register(monster)
