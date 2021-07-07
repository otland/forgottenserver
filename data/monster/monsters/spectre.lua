local mType = Game.createMonsterType("spectre")
local monster = {}

monster.name = "Spectre"
monster.description = "a spectre"
monster.experience = 2100
monster.outfit = {
	lookType = 235,
	lookHead = 20,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1350
monster.maxHealth = 1350
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 6348
monster.speed = 230
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
	{text = "Revenge ... is so ... sweet!", yell = false},
	{text = "Life...force! Feed me your... lifeforce!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 90},
	{type = COMBAT_ENERGYDAMAGE, percent = -8},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = -8},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 1},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -308, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 300, interval = 4000}},
	{name ="drunk", interval = 2000, chance = 15, drunkenness = 25, duration = 6000, radius = 4, target = false, effect = CONST_ME_SOUND_PURPLE},
	{name ="spectre drown", interval = 2000, chance = 15},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_MANADRAIN, minDamage = -100, maxDamage = -400, range = 7},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_LIFEDRAIN, minDamage = -300, maxDamage = -550, range = 7, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 35,
	armor = 35,
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_HEALING, minDamage = 100, maxDamage = 250, effect = CONST_ME_MAGIC_BLUE},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_HEALING, minDamage = 370, maxDamage = 700, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 2000, chance = 15, speed = {min = 290, max = 290}, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = 2071, chance = 9620},
	{id = "silver brooch", chance = 850},
	{id = "silver brooch", chance = 110},
	{id = 2136, chance = 110},
	{id = "gold coin", chance = 33000, maxCount = 100},
	{id = "gold coin", chance = 33000, maxCount = 100},
	{id = "gold coin", chance = 33000, maxCount = 97},
	{id = "platinum coin", chance = 3850, maxCount = 7},
	{id = "stealth ring", chance = 190},
	{id = "wand of cosmic energy", chance = 9800},
	{id = "blank rune", chance = 30310, maxCount = 2},
	{id = "white piece of cloth", chance = 3800},
	{id = "soul orb", chance = 6005},
	{id = 6300, chance = 280},
	{id = "demonic essence", chance = 6270},
	{id = "relic sword", chance = 700},
	{id = "great mana potion", chance = 920},
	{id = "shiny stone", chance = 1000}
}

mType:register(monster)
