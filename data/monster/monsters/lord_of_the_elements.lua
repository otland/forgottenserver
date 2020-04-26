local mType = Game.createMonsterType("lord of the elements")
local monster = {}

monster.name = "Lord of the Elements"
monster.description = "Lord of the Elements"
monster.experience = 8000
monster.outfit = {
	lookType = 290,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 8000
monster.maxHealth = 8000
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 9009
monster.speed = 370
monster.summonCost = 0
monster.maxSummons = 2

monster.changeTarget = {
	interval = 5000,
	chance = 10
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
	{text = "WHO DARES CALLING ME?", yell = true},
	{text = "I'LL FREEZE YOU THEN I CRUSH YOU!", yell = true}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 1},
	{type = COMBAT_ENERGYDAMAGE, percent = 30},
	{type = COMBAT_EARTHDAMAGE, percent = 45},
	{type = COMBAT_FIREDAMAGE, percent = 30},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 30},
	{type = COMBAT_HOLYDAMAGE , percent = 100},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -690, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 30,
	armor = 30,
	{name ="combat", interval = 1000, chance = 25, type = COMBAT_HEALING, minDamage = 100, maxDamage = 195, effect = CONST_ME_MAGIC_BLUE},
	{name ="outfit", interval = 1500, chance = 40, monster = "Energy Overlord", duration = 3000, effect = CONST_ME_MAGIC_BLUE},
	{name ="outfit", interval = 1500, chance = 40, monster = "Fire Overlord", duration = 3000, effect = CONST_ME_MAGIC_BLUE},
	{name ="outfit", interval = 1500, chance = 40, monster = "Earth Overlord", duration = 3000, effect = CONST_ME_MAGIC_BLUE},
	{name ="outfit", interval = 1500, chance = 40, monster = "Ice Overlord", duration = 3000, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "Blistering Fire Elemental", chance = 50, interval = 4000},
	{name = "Jagged Earth Elemental", chance = 50, interval = 4000},
	{name = "Roaring Water Elemental", chance = 50, interval = 4000},
	{name = "Overcharged Energy Elemental", chance = 50, interval = 4000}
}

monster.loot = {
	{id = "small sapphire", chance = 7142, maxCount = 4},
	{id = "small ruby", chance = 11111, maxCount = 4},
	{id = "small emerald", chance = 11111, maxCount = 4},
	{id = "small amethyst", chance = 11111, maxCount = 3},
	{id = "platinum coin", chance = 50000, maxCount = 9},
	{id = "earthborn titan armor", chance = 2063},
	{id = "gold ingot", chance = 25000}
}

mType:register(monster)
