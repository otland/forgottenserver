local mType = Game.createMonsterType("dark apprentice")
local monster = {}

monster.name = "Dark Apprentice"
monster.description = "a dark apprentice"
monster.experience = 100
monster.outfit = {
	lookType = 133,
	lookHead = 78,
	lookBody = 57,
	lookLegs = 95,
	lookFeet = 115,
	lookAddons = 1,
	lookMount = 0
}

monster.health = 225
monster.maxHealth = 225
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 20363
monster.speed = 190
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
	{text = "Outch!", yell = false},
	{text = "Oops, I did it again.", yell = false},
	{text = "From the spirits that I called Sir, deliver me!", yell = false},
	{text = "I must dispose of my masters enemies!", yell = false}
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
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = -5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -60, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_FIREDAMAGE, minDamage = -2, maxDamage = -26, range = 7, ShootEffect = CONST_ANI_FIRE},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_ENERGYDAMAGE, minDamage = -10, maxDamage = -20, range = 7, ShootEffect = CONST_ANI_ENERGY},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -24, range = 7, ShootEffect = CONST_ANI_SMALLSTONE},
	{name ="outfit", interval = 2000, chance = 1, monster = "cyclops", duration = 2000, range = 3, ShootEffect = CONST_ANI_EXPLOSION},
	{name ="outfit", interval = 2000, chance = 1, item = 2324, duration = 2000, radius = 4, target = false, effect = CONST_ME_MAGIC_BLUE}
}

monster.defenses = {
	defense = 15,
	armor = 15,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 30, maxDamage = 40, effect = CONST_ME_MAGIC_GREEN},
	{name ="outfit", interval = 2000, chance = 5, monster = "green frog", duration = 3000}
}

monster.loot = {
	{id = "gold coin", chance = 7500, maxCount = 45},
	{id = "wand of decay", chance = 110},
	{id = "wand of dragonbreath", chance = 1980},
	{id = "blank rune", chance = 8125, maxCount = 3},
	{id = 6079, chance = 12100},
	{id = "health potion", chance = 2900},
	{id = "mana potion", chance = 2980},
	{id = "reins", chance = 10}
}

mType:register(monster)
