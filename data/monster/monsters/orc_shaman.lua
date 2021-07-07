local mType = Game.createMonsterType("orc shaman")
local monster = {}

monster.name = "Orc Shaman"
monster.description = "an orc shaman"
monster.experience = 110
monster.outfit = {
	lookType = 6,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 115
monster.maxHealth = 115
monster.runHealth = 15
monster.race = "blood"
monster.corpse = 5978
monster.speed = 140
monster.summonCost = 0
monster.maxSummons = 4

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
	canPushItems = false,
	canPushCreatures = false,
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
	{text = "Huumans stinkk!", yell = false},
	{text = "Grak brrretz gulu.", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 50},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 10},
	{type = COMBAT_DEATHDAMAGE , percent = -5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -15, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_ENERGYDAMAGE, minDamage = -20, maxDamage = -35, range = 7, ShootEffect = CONST_ANI_ENERGYBALL},
	{name ="combat", interval = 2000, chance = 5, type = COMBAT_FIREDAMAGE, minDamage = -5, maxDamage = -45, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_FIRE}
}

monster.defenses = {
	defense = 8,
	armor = 8,
	{name ="combat", interval = 2000, chance = 60, type = COMBAT_HEALING, minDamage = 27, maxDamage = 43, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "Snake", chance = 20, interval = 2000, max = 4}
}

monster.loot = {
	{id = "gold coin", chance = 90000, maxCount = 5},
	{id = "corncob", chance = 10000, maxCount = 2},
	{id = "broken shamanic staff", chance = 10000},
	{id = "chain armor", chance = 8750},
	{id = "shamanic hood", chance = 7800},
	{id = "spear", chance = 5500},
	{id = "orc leather", chance = 3500},
	{id = "orc tooth", chance = 2100},
	{id = "wand of decay", chance = 1000},
	{id = 1958, chance = 520}
}

mType:register(monster)
