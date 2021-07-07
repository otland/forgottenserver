local mType = Game.createMonsterType("souleater")
local monster = {}

monster.name = "Souleater"
monster.description = "a souleater"
monster.experience = 1300
monster.outfit = {
	lookType = 355,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1100
monster.maxHealth = 1100
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 12631
monster.speed = 200
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
	staticAttackChance = 80,
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
	{text = "Life is such a fickle thing!", yell = false},
	{text = "I will devour your soul.", yell = false},
	{text = "Souuuls!", yell = false},
	{text = "I will feed on you.", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 70},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 50},
	{type = COMBAT_HOLYDAMAGE , percent = -10},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -210, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 100, type = COMBAT_ICEDAMAGE, minDamage = -50, maxDamage = -100, range = 7, ShootEffect = CONST_ANI_SMALLICE},
	{name ="souleater drown", interval = 2000, chance = 10},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_DEATHDAMAGE, minDamage = -100, maxDamage = -200, length = 4, spread = 3, effect = CONST_ME_SOUND_RED},
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_LIFEDRAIN, minDamage = -30, maxDamage = -60, radius = 4, target = false, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 30,
	armor = 30,
	{name ="invisible", interval = 2000, chance = 5, duration = 3000, effect = CONST_ME_MAGIC_BLUE},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 120, maxDamage = 125, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "gold coin", chance = 33400, maxCount = 100},
	{id = "gold coin", chance = 33400, maxCount = 100},
	{id = "platinum coin", chance = 49610, maxCount = 6},
	{id = "necrotic rod", chance = 980},
	{id = "wand of cosmic energy", chance = 910},
	{id = "spirit container", chance = 140},
	{id = 6300, chance = 300},
	{id = "great mana potion", chance = 8000},
	{id = "ultimate health potion", chance = 9400},
	{id = "souleater trophy", chance = 20},
	{id = "lizard essence", chance = 15250},
	{id = "ectoplasmic sushi", chance = 2000}
}

mType:register(monster)
