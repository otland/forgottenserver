local mType = Game.createMonsterType("ghazbaran")
local monster = {}

monster.name = "Ghazbaran"
monster.description = "Ghazbaran"
monster.experience = 15000
monster.outfit = {
	lookType = 12,
	lookHead = 0,
	lookBody = 66,
	lookLegs = 98,
	lookFeet = 75,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 60000
monster.maxHealth = 60000
monster.runHealth = 3500
monster.race = "fire"
monster.corpse = 6068
monster.speed = 400
monster.summonCost = 0
monster.maxSummons = 4

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
	{text = "COME AND GIVE ME SOME AMUSEMENT", yell = false},
	{text = "IS THAT THE BEST YOU HAVE TO OFFER?", yell = true},
	{text = "I AM GHAZBARAN OF THE TRIANGLE... AND I AM HERE TO CHALLENGE YOU ALL.", yell = true},
	{text = "FLAWLESS VICTORY!", yell = true}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 1},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = -1},
	{type = COMBAT_DEATHDAMAGE , percent = 1}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -2191, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 40, type = COMBAT_PHYSICALDAMAGE, minDamage = -250, maxDamage = -500, range = 7, radius = 6, target = false, effect = CONST_ME_HITAREA},
	{name ="combat", interval = 3000, chance = 34, type = COMBAT_PHYSICALDAMAGE, minDamage = -120, maxDamage = -500, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_WHIRLWINDSWORD, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 4000, chance = 30, type = COMBAT_ENERGYDAMAGE, minDamage = -100, maxDamage = -800, length = 8, spread = 0, effect = CONST_ME_MORTAREA},
	{name ="combat", interval = 3000, chance = 20, type = COMBAT_PHYSICALDAMAGE, minDamage = -200, maxDamage = -480, range = 14, radius = 5, target = false, effect = CONST_ME_POFF},
	{name ="combat", interval = 4000, chance = 15, type = COMBAT_PHYSICALDAMAGE, minDamage = -100, maxDamage = -650, range = 7, radius = 13, target = false, effect = CONST_ME_BLOCKHIT},
	{name ="combat", interval = 4000, chance = 18, type = COMBAT_PHYSICALDAMAGE, minDamage = -200, maxDamage = -600, radius = 14, target = false, effect = CONST_ME_LOSEENERGY},
	{name ="combat", interval = 3000, chance = 15, type = COMBAT_PHYSICALDAMAGE, minDamage = -200, maxDamage = -750, range = 7, radius = 4, target = false, effect = CONST_ME_ENERGYAREA}
}

monster.defenses = {
	defense = 65,
	armor = 55,
	{name ="combat", interval = 3000, chance = 35, type = COMBAT_HEALING, minDamage = 300, maxDamage = 800, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 4000, chance = 80, speed = {min = 440, max = 440}, duration = 6000, effect = CONST_ME_MAGIC_RED}
}

monster.summons = {
	{name = "Deathslicer", chance = 20, interval = 4000}
}

monster.loot = {
	{id = "blue tome", chance = 20000},
	{id = "teddy bear", chance = 12500},
	{id = 2124, chance = 8333},
	{id = "white pearl", chance = 25000, maxCount = 15},
	{id = "black pearl", chance = 11111, maxCount = 14},
	{id = "small diamond", chance = 25000, maxCount = 5},
	{id = "small sapphire", chance = 25000, maxCount = 10},
	{id = "small emerald", chance = 25000, maxCount = 10},
	{id = "small amethyst", chance = 25000, maxCount = 17},
	{id = "talon", chance = 12500, maxCount = 7},
	{id = "platinum coin", chance = 100000, maxCount = 69},
	{id = "green gem", chance = 20000},
	{id = "blue gem", chance = 14285},
	{id = "might ring", chance = 12500},
	{id = "stealth ring", chance = 12500},
	{id = "strange symbol", chance = 11111},
	{id = "life crystal", chance = 12500},
	{id = "mind stone", chance = 20000},
	{id = "gold ring", chance = 20000},
	{id = "ring of healing", chance = 20000},
	{id = "twin axe", chance = 11111},
	{id = "golden armor", chance = 8333},
	{id = "magic plate armor", chance = 8333},
	{id = "demon shield", chance = 12500},
	{id = "golden boots", chance = 8333},
	{id = "demon horn", chance = 33333, maxCount = 2},
	{id = 6300, chance = 25000},
	{id = "demonic essence", chance = 100000},
	{id = "ruthless axe", chance = 14285},
	{id = "assassin star", chance = 12500, maxCount = 44},
	{id = "havoc blade", chance = 16666},
	{id = "ravenwing", chance = 14285},
	{id = "scrying crystal ball", chance = 20000},
	{id = "great mana potion", chance = 20000},
	{id = "great health potion", chance = 20000},
	{id = "glacier kilt", chance = 8333},
	{id = "great spirit potion", chance = 25000},
	{id = "ultimate health potion", chance = 25000},
	{id = "oceanborn leviathan armor", chance = 16666},
	{id = "frozen plate", chance = 8333},
	{id = "spellbook of warding", chance = 20000},
	{id = "spellbook of mind control", chance = 11111},
	{id = "spellbook of lost souls", chance = 16666},
	{id = "spellscroll of prophecies", chance = 25000},
	{id = "spellbook of dark mysteries", chance = 20000}
}

mType:register(monster)
