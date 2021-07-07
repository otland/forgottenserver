local mType = Game.createMonsterType("horadron")
local monster = {}

monster.name = "Horadron"
monster.description = "Horadron"
monster.experience = 18000
monster.outfit = {
	lookType = 12,
	lookHead = 78,
	lookBody = 61,
	lookLegs = 115,
	lookFeet = 58,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 30000
monster.maxHealth = 30000
monster.runHealth = 0
monster.race = "fire"
monster.corpse = 6068
monster.speed = 320
monster.summonCost = 0
monster.maxSummons = 2

monster.changeTarget = {
	interval = 2000,
	chance = 25
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
	{text = "Even I fear the wrath of the princes. And the cold touch of those whom they serve! You are not prepared!", yell = false},
	{text = "You foolish mortals with you medding you brought the end upon your world!", yell = false},
	{text = "After all those aeons I smell freedom at last!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 30},
	{type = COMBAT_ENERGYDAMAGE, percent = 30},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 30},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 30},
	{type = COMBAT_HOLYDAMAGE , percent = 30},
	{type = COMBAT_DEATHDAMAGE , percent = 30}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, skill = 110, attack = 100, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_EARTHDAMAGE, minDamage = -600, maxDamage = -1000, length = 8, spread = 0, effect = CONST_ME_HITBYPOISON},
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_EARTHDAMAGE, minDamage = -600, maxDamage = -1000, length = 8, spread = 3, effect = CONST_ME_HITBYPOISON},
	{name ="combat", interval = 2000, chance = 30, type = COMBAT_EARTHDAMAGE, minDamage = -100, maxDamage = -235, range = 7, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_HITBYPOISON},
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_EARTHDAMAGE, minDamage = -100, maxDamage = -250, radius = 3, target = false, effect = CONST_ME_POISONAREA},
	{name ="condition", interval = 2000, chance = 25, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -300, maxDamage = -450, radius = 3, target = false, effect = CONST_ME_HITBYPOISON}
}

monster.defenses = {
	defense = 65,
	armor = 55,
	{name ="combat", interval = 2000, chance = 35, type = COMBAT_HEALING, minDamage = 400, maxDamage = 700, effect = CONST_ME_DRAWBLOOD}
}

monster.summons = {
	{name = "Defiler", chance = 12, interval = 2000}
}

monster.loot = {
	{id = 22396, chance = 100000},
	{id = 5954, chance = 100000},
	{id = 6500, chance = 100000},
	{id = 22397, chance = 13850},
	{id = 22610, chance = 5380},
	{id = 2148, chance = 50000, maxCount = 100},
	{id = 2148, chance = 50000, maxCount = 100},
	{id = 2152, chance = 100000, maxCount = 50},
	{id = 22598, chance = 100000},
	{id = 8903, chance = 5130},
	{id = 8901, chance = 35900},
	{id = 8902, chance = 10260},
	{id = 3962, chance = 33330},
	{id = 7456, chance = 12820},
	{id = 7453, chance = 2560},
	{id = 7388, chance = 2560},
	{id = 2466, chance = 1220},
	{id = 2656, chance = 1220},
	{id = 2487, chance = 17950},
	{id = 2470, chance = 250},
	{id = 2415, chance = 90},
	{id = 8926, chance = 90},
	{id = 7895, chance = 1750},
	{id = 2477, chance = 1750},
	{id = 2488, chance = 20510},
	{id = 2645, chance = 15380},
	{id = 2145, chance = 17950, maxCount = 25},
	{id = 2150, chance = 5130, maxCount = 25},
	{id = 2149, chance = 10260, maxCount = 25},
	{id = 2146, chance = 28210, maxCount = 25},
	{id = 9970, chance = 15380, maxCount = 25},
	{id = 2147, chance = 20510, maxCount = 25}
}

mType:register(monster)
