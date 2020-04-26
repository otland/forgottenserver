local mType = Game.createMonsterType("mad scientist")
local monster = {}

monster.name = "Mad Scientist"
monster.description = "a mad scientist"
monster.experience = 205
monster.outfit = {
	lookType = 133,
	lookHead = 39,
	lookBody = 0,
	lookLegs = 19,
	lookFeet = 20,
	lookAddons = 1,
	lookMount = 0
}

monster.health = 325
monster.maxHealth = 325
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 20439
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
	{text = "Die in the name of Science!", yell = false},
	{text = "You will regret interrupting my studies!", yell = false},
	{text = "Let me test this!", yell = false},
	{text = "I will study your corpse!", yell = false}
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
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE , percent = 20},
	{type = COMBAT_DEATHDAMAGE , percent = -5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -35, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_DROWNDAMAGE, minDamage = -20, maxDamage = -56, range = 7, radius = 3, target = true, ShootEffect = CONST_ANI_SMALLEARTH, effect = CONST_ME_POFF},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_EARTHDAMAGE, minDamage = -20, maxDamage = -36, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_ENERGYHIT},
	{name ="speed", interval = 2000, chance = 10, speed = {min = -300, max = -300}, duration = 2000, range = 7, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_SMALLPLANTS}
}

monster.defenses = {
	defense = 15,
	armor = 15,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 10, maxDamage = 30, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "gold coin", chance = 30000, maxCount = 65},
	{id = "gold coin", chance = 30000, maxCount = 50},
	{id = 2162, chance = 2000},
	{id = "life crystal", chance = 2000},
	{id = "soulfire rune", chance = 6000},
	{id = "cookie", chance = 1200, maxCount = 5},
	{id = "white mushroom", chance = 8000, maxCount = 3},
	{id = 6324, chance = 820},
	{id = "mastermind potion", chance = 130},
	{id = "health potion", chance = 19000},
	{id = "mana potion", chance = 19000},
	{id = "small enchanted amethyst", chance = 470}
}

mType:register(monster)
