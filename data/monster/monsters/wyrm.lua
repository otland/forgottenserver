local mType = Game.createMonsterType("wyrm")
local monster = {}

monster.name = "Wyrm"
monster.description = "a wyrm"
monster.experience = 1550
monster.outfit = {
	lookType = 291,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1825
monster.maxHealth = 1825
monster.runHealth = 150
monster.race = "blood"
monster.corpse = 8941
monster.speed = 280
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
	canPushCreatures = false,
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
	{text = "GRRR", yell = false},
	{text = "GRROARR", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 100},
	{type = COMBAT_EARTHDAMAGE, percent = 75},
	{type = COMBAT_FIREDAMAGE, percent = 20},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = -5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -235, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_ENERGYDAMAGE, minDamage = -100, maxDamage = -220, radius = 3, target = false, effect = CONST_ME_YELLOWENERGY},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_ENERGYDAMAGE, minDamage = -130, maxDamage = -200, length = 5, spread = 2, effect = CONST_ME_PURPLEENERGY},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_ENERGYDAMAGE, minDamage = -100, maxDamage = -125, range = 7, ShootEffect = CONST_ANI_ENERGY, effect = CONST_ME_ENERGYHIT},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_LIFEDRAIN, minDamage = -98, maxDamage = -145, length = 4, spread = 0, effect = CONST_ME_POFF}
}

monster.defenses = {
	defense = 34,
	armor = 34,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 100, maxDamage = 150, effect = CONST_ME_MAGIC_BLUE},
	{name ="effect", interval = 2000, chance = 10, radius = 1, target = false, effect = CONST_ME_SOUND_YELLOW}
}

monster.loot = {
	{id = "gold coin", chance = 32000, maxCount = 100},
	{id = "gold coin", chance = 32000, maxCount = 100},
	{id = "gold coin", chance = 32000, maxCount = 30},
	{id = "dragon ham", chance = 34800, maxCount = 3},
	{id = "strong health potion", chance = 20000},
	{id = "strong mana potion", chance = 15310},
	{id = "wyrm scale", chance = 15360},
	{id = "burst arrow", chance = 8000, maxCount = 10},
	{id = "crossbow", chance = 6000},
	{id = "focus cape", chance = 1250},
	{id = "wand of draconia", chance = 1000},
	{id = "small diamond", chance = 910, maxCount = 3},
	{id = "lightning pendant", chance = 760},
	{id = "wand of starstorm", chance = 460},
	{id = "hibiscus dress", chance = 320},
	{id = "composite hornbow", chance = 110},
	{id = "dragonbone staff", chance = 110},
	{id = "shockwave amulet", chance = 110}
}

mType:register(monster)
