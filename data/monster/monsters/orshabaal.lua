local mType = Game.createMonsterType("orshabaal")
local monster = {}

monster.name = "Orshabaal"
monster.description = "Orshabaal"
monster.experience = 10000
monster.outfit = {
	lookType = 201,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 22500
monster.maxHealth = 22500
monster.runHealth = 2500
monster.race = "fire"
monster.corpse = 2916
monster.speed = 380
monster.summonCost = 0
monster.maxSummons = 4

monster.changeTarget = {
	interval = 2000,
	chance = 10
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
	{text = "PRAISED BE MY MASTERS, THE RUTHLESS SEVEN!", yell = false},
	{text = "YOU ARE DOOMED!", yell = false},
	{text = "ORSHABAAL IS BACK!", yell = false},
	{text = "Be prepared for the day my masters will come for you!", yell = false},
	{text = "SOULS FOR ORSHABAAL!", yell = false}
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
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -1},
	{type = COMBAT_HOLYDAMAGE , percent = -1},
	{type = COMBAT_DEATHDAMAGE , percent = 50}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -1990, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 1000, chance = 13, type = COMBAT_MANADRAIN, minDamage = -300, maxDamage = -600, range = 7},
	{name ="combat", interval = 1000, chance = 6, type = COMBAT_MANADRAIN, minDamage = -150, maxDamage = -350, radius = 5, target = false, effect = CONST_ME_POISONAREA},
	{name ="effect", interval = 1000, chance = 6, radius = 5, target = false, effect = CONST_ME_HITAREA},
	{name ="combat", interval = 1000, chance = 34, type = COMBAT_FIREDAMAGE, minDamage = -310, maxDamage = -600, range = 7, radius = 7, target = true, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_FIREAREA},
	{name ="firefield", interval = 1000, chance = 10, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_FIRE},
	{name ="combat", interval = 1000, chance = 15, type = COMBAT_ENERGYDAMAGE, minDamage = -500, maxDamage = -850, length = 8, spread = 0, effect = CONST_ME_ENERGYHIT}
}

monster.defenses = {
	defense = 90,
	armor = 90,
	{name ="combat", interval = 1000, chance = 9, type = COMBAT_HEALING, minDamage = 1500, maxDamage = 2500, effect = CONST_ME_MAGIC_BLUE},
	{name ="combat", interval = 1000, chance = 17, type = COMBAT_HEALING, minDamage = 600, maxDamage = 1000, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 1000, chance = 5, speed = {min = 1140, max = 1140}, duration = 7000, effect = CONST_ME_MAGIC_RED}
}

monster.summons = {
	{name = "demon", chance = 10, interval = 1000}
}

monster.loot = {
	{id = "platinum coin", chance = 100000, maxCount = 71},
	{id = "demonic essence", chance = 95000, maxCount = 5},
	{id = 6300, chance = 55000},
	{id = "demon horn", chance = 50000, maxCount = 2},
	{id = "small sapphire", chance = 33333, maxCount = 9},
	{id = "white pearl", chance = 33333, maxCount = 15},
	{id = "great mana potion", chance = 33333},
	{id = "black pearl", chance = 27000, maxCount = 8},
	{id = "ring of healing", chance = 27000},
	{id = "ultimate health potion", chance = 27000},
	{id = "great health potion", chance = 27000},
	{id = "devil helmet", chance = 27000},
	{id = "small emerald", chance = 22000, maxCount = 9},
	{id = "small diamond", chance = 22000, maxCount = 5},
	{id = "blue gem", chance = 22000},
	{id = "mind stone", chance = 22000},
	{id = "giant sword", chance = 22000},
	{id = "demon shield", chance = 22000},
	{id = "small amethyst", chance = 20000, maxCount = 19},
	{id = "talon", chance = 20000, maxCount = 5},
	{id = "assassin star", chance = 16500, maxCount = 42},
	{id = "protection amulet", chance = 16000},
	{id = "purple tome", chance = 16000},
	{id = "crystal necklace", chance = 16000},
	{id = "strange symbol", chance = 16000},
	{id = "two handed sword", chance = 16000},
	{id = "boots of haste", chance = 16000},
	{id = "silver amulet", chance = 16000},
	{id = "golden mug", chance = 12500},
	{id = "orb", chance = 12500},
	{id = "great spirit potion", chance = 12500},
	{id = "life crystal", chance = 12500},
	{id = "fire axe", chance = 12500},
	{id = "golden legs", chance = 12500},
	{id = "stone skin amulet", chance = 12500},
	{id = "platinum amulet", chance = 12500},
	{id = "mastermind shield", chance = 12500},
	{id = "gold ingot", chance = 6666},
	{id = "might ring", chance = 6666},
	{id = "teddy bear", chance = 6666},
	{id = "green gem", chance = 6666},
	{id = 2162, chance = 6666},
	{id = "Orshabaal's brain", chance = 6666},
	{id = "silver dagger", chance = 6666},
	{id = "golden sickle", chance = 6666},
	{id = "thunder hammer", chance = 6666},
	{id = "dragon hammer", chance = 6666},
	{id = "magic plate armor", chance = 6666},
	{id = 3955, chance = 6666},
	{id = 2142, chance = 3500},
	{id = "robe of the underworld", chance = 1000}
}

mType:register(monster)
