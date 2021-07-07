local mType = Game.createMonsterType("dark magician")
local monster = {}

monster.name = "Dark Magician"
monster.description = "a dark magician"
monster.experience = 185
monster.outfit = {
	lookType = 133,
	lookHead = 58,
	lookBody = 95,
	lookLegs = 51,
	lookFeet = 131,
	lookAddons = 2,
	lookMount = 0
}

monster.health = 325
monster.maxHealth = 325
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 20367
monster.speed = 210
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
	{text = "Feel the power of my runes!", yell = false},
	{text = "Killing you is getting expensive!", yell = false},
	{text = "My secrets are mine alone!", yell = false},
	{text = "Stand Still!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 10},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE , percent = 20},
	{type = COMBAT_DEATHDAMAGE , percent = -5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -40, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_FIREDAMAGE, minDamage = -5, maxDamage = -40, range = 7, ShootEffect = CONST_ANI_FIRE},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_ENERGYDAMAGE, minDamage = -20, maxDamage = -30, range = 7, ShootEffect = CONST_ANI_ENERGY}
}

monster.defenses = {
	defense = 15,
	armor = 15,
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_HEALING, minDamage = 60, maxDamage = 80, effect = CONST_ME_MAGIC_GREEN},
	{name ="invisible", interval = 2000, chance = 10, duration = 5000, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "gold coin", chance = 75100, maxCount = 55},
	{id = "wand of decay", chance = 280},
	{id = "blank rune", chance = 10000},
	{id = "strong health potion", chance = 3000},
	{id = "strong mana potion", chance = 2860},
	{id = "health potion", chance = 12000},
	{id = "mana potion", chance = 11900},
	{id = "small enchanted emerald", chance = 510},
	{id = "reins", chance = 20}
}

mType:register(monster)
