local mType = Game.createMonsterType("koshei the deathless")
local monster = {}

monster.name = "Koshei The Deathless"
monster.description = "Koshei the Deathless"
monster.experience = 0
monster.outfit = {
	lookType = 99,
	lookHead = 95,
	lookBody = 116,
	lookLegs = 119,
	lookFeet = 115,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 3000
monster.maxHealth = 3000
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 6028
monster.speed = 390
monster.summonCost = 0
monster.maxSummons = 1

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
	{text = "Your pain will be beyond imagination!", yell = false},
	{text = "You can't defeat me! I will ressurect and take your soul!", yell = false},
	{text = "Death is my ally!", yell = false},
	{text = "Welcome to my domain, visitor!", yell = false},
	{text = "You will be my toy on the other side!", yell = false},
	{text = "What a disgusting smell of life!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 80},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = -15},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -170, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 300, interval = 4000}},
	{name ="combat", interval = 3000, chance = 9, type = COMBAT_LIFEDRAIN, minDamage = -60, maxDamage = -250, range = 1, effect = CONST_ME_MAGIC_BLUE},
	{name ="combat", interval = 1000, chance = 11, type = COMBAT_LIFEDRAIN, minDamage = -70, maxDamage = -135, radius = 3, target = false, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 2000, chance = 9, type = COMBAT_DEATHDAMAGE, minDamage = -50, maxDamage = -140, length = 8, spread = 0, effect = CONST_ME_MORTAREA},
	{name ="condition", interval = 3000, chance = 15, type = CONDITION_CURSED, startDamage = 0, tick = 4000, minDamage = -54, maxDamage = -54, range = 1},
	{name ="speed", interval = 2000, chance = 15, speed = {min = -900, max = -900}, duration = 30000, range = 7, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 20,
	armor = 20,
	{name ="combat", interval = 1000, chance = 30, type = COMBAT_HEALING, minDamage = 150, maxDamage = 300, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "bonebeast", chance = 16, interval = 1000}
}

monster.loot = {
	{id = "gold coin", chance = 100000, maxCount = 100},
	{id = "gold coin", chance = 80000, maxCount = 12},
	{id = "stealth ring", chance = 888},
	{id = "platinum amulet", chance = 1666},
	{id = 2175, chance = 10000},
	{id = "gold ring", chance = 900},
	{id = 2237, chance = 10000},
	{id = 2401, chance = 10000},
	{id = "castle shield", chance = 588},
	{id = "blue robe", chance = 709},
	{id = "lich staff", chance = 1000},
	{id = "lightning boots", chance = 688}
}

mType:register(monster)
