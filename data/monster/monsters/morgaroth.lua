local mType = Game.createMonsterType("morgaroth")
local monster = {}

monster.name = "Morgaroth"
monster.description = "Morgaroth"
monster.experience = 15000
monster.outfit = {
	lookType = 12,
	lookHead = 1,
	lookBody = 75,
	lookLegs = 59,
	lookFeet = 1,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 55000
monster.maxHealth = 55000
monster.runHealth = 0
monster.race = "fire"
monster.corpse = 6068
monster.speed = 400
monster.summonCost = 0
monster.maxSummons = 6

monster.changeTarget = {
	interval = 10000,
	chance = 20
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
	staticAttackChance = 98,
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
	{text = "I AM MORGAROTH, LORD OF THE TRIANGLE... AND YOU ARE LOST!", yell = true},
	{text = "MY SEED IS FEAR AND MY HARVEST ARE YOUR SOULS!", yell = true},
	{text = "ZATHROTH! LOOK AT THE DESTRUCTION I AM CAUSING IN YOUR NAME!", yell = true},
	{text = "THE TRIANGLE OF TERROR WILL RISE!", yell = true}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 1},
	{type = COMBAT_ENERGYDAMAGE, percent = 1},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 80}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -2000, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 3000, chance = 35, type = COMBAT_FIREDAMAGE, minDamage = -700, maxDamage = -1200, range = 7, radius = 7, target = true, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_FIREAREA},
	{name ="melee", interval = 1800, chance = 40, minDamage = -250, maxDamage = -400, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 3000, chance = 30, type = COMBAT_ENERGYDAMAGE, minDamage = -300, maxDamage = -1000, length = 8, spread = 0, effect = CONST_ME_ENERGYHIT},
	{name ="combat", interval = 2500, chance = 20, type = COMBAT_PHYSICALDAMAGE, minDamage = -200, maxDamage = -480, range = 7, radius = 5, target = false, effect = CONST_ME_MAGIC_GREEN},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_PHYSICALDAMAGE, minDamage = -250, maxDamage = -500, range = 7, radius = 13, target = false, effect = CONST_ME_SOUND_RED},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_PHYSICALDAMAGE, minDamage = -200, maxDamage = -450, radius = 14, target = false, effect = CONST_ME_LOSEENERGY},
	{name ="melee", interval = 3000, chance = 15, minDamage = -100, maxDamage = -200, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 65,
	armor = 55,
	{name ="combat", interval = 3000, chance = 35, type = COMBAT_HEALING, minDamage = 300, maxDamage = 800, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 4000, chance = 80, speed = {min = 440, max = 440}, duration = 6000, effect = CONST_ME_MAGIC_RED}
}

monster.summons = {
	{name = "Demon", chance = 33, interval = 4000}
}

monster.loot = {
	{id = "red tome", chance = 20000},
	{id = "golden mug", chance = 16666},
	{id = "teddy bear", chance = 11111},
	{id = 2124, chance = 5882},
	{id = "white pearl", chance = 11111, maxCount = 13},
	{id = "black pearl", chance = 16666, maxCount = 15},
	{id = "small sapphire", chance = 25000, maxCount = 8},
	{id = "small emerald", chance = 20000, maxCount = 6},
	{id = "small amethyst", chance = 33333, maxCount = 18},
	{id = "talon", chance = 16666, maxCount = 7},
	{id = "platinum coin", chance = 50000, maxCount = 74},
	{id = "green gem", chance = 33333},
	{id = "blue gem", chance = 11111},
	{id = "might ring", chance = 20000},
	{id = "stealth ring", chance = 11111},
	{id = "energy ring", chance = 5882},
	{id = "strange symbol", chance = 11111},
	{id = "life crystal", chance = 11111},
	{id = "life crystal", chance = 11111},
	{id = "mind stone", chance = 16666},
	{id = "gold ring", chance = 5882},
	{id = "ring of healing", chance = 25000},
	{id = "double axe", chance = 16666},
	{id = "giant sword", chance = 11111},
	{id = "golden legs", chance = 11111},
	{id = "magic plate armor", chance = 20000},
	{id = "steel boots", chance = 11111},
	{id = "Morgaroth's heart", chance = 16666},
	{id = "demon horn", chance = 20000, maxCount = 2},
	{id = 6300, chance = 16666},
	{id = "demonic essence", chance = 50000},
	{id = "assassin star", chance = 16666, maxCount = 28},
	{id = "demonbone", chance = 16666},
	{id = "scrying crystal ball", chance = 11111},
	{id = "great mana potion", chance = 50000},
	{id = "great health potion", chance = 5882},
	{id = "great spirit potion", chance = 5882},
	{id = "ultimate health potion", chance = 20000},
	{id = "chain bolter", chance = 20000},
	{id = "royal crossbow", chance = 11111},
	{id = "the devileye", chance = 33333},
	{id = "the ironworker", chance = 25000},
	{id = "dark lord's cape", chance = 20000},
	{id = "dragon robe", chance = 5882},
	{id = "fireborn giant armor", chance = 16666},
	{id = "obsidian truncheon", chance = 16666}
}

mType:register(monster)
