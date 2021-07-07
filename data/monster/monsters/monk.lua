local mType = Game.createMonsterType("monk")
local monster = {}

monster.name = "Monk"
monster.description = "a monk"
monster.experience = 200
monster.outfit = {
	lookType = 57,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 240
monster.maxHealth = 240
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 20371
monster.speed = 240
monster.summonCost = 600
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = true,
	attackable = true,
	hostile = true,
	convinceable = true,
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
	{text = "Repent Heretic!", yell = false},
	{text = "A prayer to the almighty one!", yell = false},
	{text = "I will punish the sinners!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 50},
	{type = COMBAT_DEATHDAMAGE , percent = 50}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -130, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 30,
	armor = 30,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 30, maxDamage = 50, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 2000, chance = 15, speed = {min = 300, max = 300}, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = "bread", chance = 20000},
	{id = "gold coin", chance = 15000, maxCount = 18},
	{id = "book of prayers", chance = 4930},
	{id = "rope belt", chance = 3150},
	{id = "ankh", chance = 2240},
	{id = 1949, chance = 1700},
	{id = "life crystal", chance = 1002},
	{id = "safety pin", chance = 1001},
	{id = "brown flask", chance = 890},
	{id = 2044, chance = 840},
	{id = "sandals", chance = 660},
	{id = 2401, chance = 440},
	{id = "power ring", chance = 100}
}

mType:register(monster)
