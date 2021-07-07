local mType = Game.createMonsterType("dharalion")
local monster = {}

monster.name = "Dharalion"
monster.description = "Dharalion"
monster.experience = 570
monster.outfit = {
	lookType = 203,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 380
monster.maxHealth = 380
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 6011
monster.speed = 240
monster.summonCost = 0
monster.maxSummons = 2

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
	targetDistance = 4,
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
	{text = "Feel my wrath!", yell = false},
	{text = "Noone will stop my ascension!", yell = false},
	{text = "My powers are divine!", yell = false},
	{text = "You desecrated this temple!", yell = false},
	{text = "Muahahaha!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 10},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -30, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 1000, chance = 15, type = COMBAT_MANADRAIN, minDamage = -30, maxDamage = -60, range = 7},
	{name ="combat", interval = 1000, chance = 13, type = COMBAT_ENERGYDAMAGE, minDamage = -80, maxDamage = -100, range = 7, ShootEffect = CONST_ANI_ENERGY, effect = CONST_ME_ENERGYHIT},
	{name ="combat", interval = 1000, chance = 10, type = COMBAT_DEATHDAMAGE, minDamage = -80, maxDamage = -151, range = 7, ShootEffect = CONST_ANI_SUDDENDEATH},
	{name ="effect", interval = 1000, chance = 13, range = 7, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_POISONAREA}
}

monster.defenses = {
	defense = 25,
	armor = 15,
	{name ="combat", interval = 1000, chance = 20, type = COMBAT_HEALING, minDamage = 90, maxDamage = 150, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 1000, chance = 7, speed = {min = 300, max = 300}, duration = 10000, effect = CONST_ME_MAGIC_RED}
}

monster.summons = {
	{name = "demon skeleton", chance = 6, interval = 1000}
}

monster.loot = {
	{id = "gold coin", chance = 100000, maxCount = 100},
	{id = "holy orchid", chance = 100000},
	{id = "elvish talisman", chance = 87880},
	{id = "elven astral observer", chance = 83330},
	{id = "yellow gem", chance = 42420},
	{id = "blank rune", chance = 25760},
	{id = "melon", chance = 22000},
	{id = "bread", chance = 16000},
	{id = "elven amulet", chance = 15150},
	{id = "great mana potion", chance = 13000},
	{id = "life crystal", chance = 13000},
	{id = "sling herb", chance = 10000},
	{id = 3957, chance = 7500},
	{id = "green tunic", chance = 3000},
	{id = "royal spear", chance = 1520, maxCount = 2}
}

mType:register(monster)
