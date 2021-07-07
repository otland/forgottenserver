local mType = Game.createMonsterType("weeper")
local monster = {}

monster.name = "Weeper"
monster.description = "a weeper"
monster.experience = 4800
monster.outfit = {
	lookType = 489,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 6800
monster.maxHealth = 6800
monster.runHealth = 570
monster.race = "fire"
monster.corpse = 17252
monster.speed = 260
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
	illusionable = false,
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
	{text = "Moooaaan!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -450, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_FIREDAMAGE, minDamage = -400, maxDamage = -1000, length = 8, spread = 0, effect = CONST_ME_FIREATTACK},
	{name ="combat", interval = 3000, chance = 100, type = COMBAT_FIREDAMAGE, minDamage = -80, maxDamage = -250, radius = 3, target = false, effect = CONST_ME_HITBYFIRE},
	{name ="speed", interval = 2000, chance = 10, speed = {min = -800, max = -800}, duration = 30000, length = 5, spread = 3, effect = CONST_ME_BLOCKHIT}
}

monster.defenses = {
	defense = 50,
	armor = 50,
	{name ="invisible", interval = 2000, chance = 5, duration = 3000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = "small ruby", chance = 15690, maxCount = 3},
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "gold coin", chance = 50000, maxCount = 99},
	{id = "bronze amulet", chance = 100000, maxCount = 7},
	{id = "fire sword", chance = 3280},
	{id = "fire axe", chance = 2150},
	{id = "great mana potion", chance = 14800, maxCount = 2},
	{id = "magma legs", chance = 790},
	{id = "magma coat", chance = 790},
	{id = "ultimate health potion", chance = 15440, maxCount = 2},
	{id = "fiery heart", chance = 13570},
	{id = 13757, chance = 1320},
	{id = "wand of everblazing", chance = 1480},
	{id = "violet crystal shard", chance = 5430},
	{id = "brown crystal splinter", chance = 12100, maxCount = 2},
	{id = "red crystal fragment", chance = 8880},
	{id = "magma clump", chance = 13240},
	{id = "blazing bone", chance = 14890},
	{id = "eye of a weeper", chance = 15870},
	{id = "prismatic bolt", chance = 10900, maxCount = 5}
}

mType:register(monster)
