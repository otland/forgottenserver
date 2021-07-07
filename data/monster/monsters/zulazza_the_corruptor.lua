local mType = Game.createMonsterType("zulazza the corruptor")
local monster = {}

monster.name = "Zulazza the Corruptor"
monster.description = "Zulazza The Corruptor"
monster.experience = 9800
monster.outfit = {
	lookType = 334,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 28000
monster.maxHealth = 28000
monster.runHealth = 1500
monster.race = "blood"
monster.corpse = 11107
monster.speed = 290
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 2000,
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
	{text = "I'm Zulazza, and you won't forget me that fazzt.", yell = false},
	{text = "Oh, HE will take revenge on zzizz azzault when you zztep in front of HIZZ fazze!", yell = false}
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
	{type = COMBAT_EARTHDAMAGE, percent = 70},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = 20},
	{type = COMBAT_DEATHDAMAGE , percent = 30}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -2100, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 40, type = COMBAT_PHYSICALDAMAGE, minDamage = -500, maxDamage = -800, length = 8, spread = 0, effect = CONST_ME_MORTAREA},
	{name ="combat", interval = 2000, chance = 30, type = COMBAT_EARTHDAMAGE, minDamage = -300, maxDamage = -800, radius = 3, target = false, effect = CONST_ME_POISONAREA},
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_MANADRAIN, minDamage = -50, maxDamage = -130, range = 7, effect = CONST_ME_MAGIC_GREEN},
	{name ="speed", interval = 2000, chance = 20, speed = {min = -500, max = -500}, duration = 20000, range = 7, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 119,
	armor = 96,
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_HEALING, minDamage = 2000, maxDamage = 3000, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "emerald bangle", chance = 10500},
	{id = "gold coin", chance = 49650, maxCount = 100},
	{id = "platinum coin", chance = 41325, maxCount = 30},
	{id = "violet gem", chance = 25500},
	{id = "yellow gem", chance = 15500},
	{id = "green gem", chance = 20500},
	{id = "red gem", chance = 10500},
	{id = "blue gem", chance = 30500},
	{id = "mastermind shield", chance = 5500},
	{id = "guardian shield", chance = 5500},
	{id = "tower shield", chance = 15500},
	{id = "soul orb", chance = 19250, maxCount = 4},
	{id = "viper star", chance = 8100, maxCount = 67},
	{id = "mastermind potion", chance = 10500},
	{id = "great mana potion", chance = 20500},
	{id = "great health potion", chance = 30500},
	{id = 7632, chance = 28000, maxCount = 2},
	{id = "ultimate health potion", chance = 10500},
	{id = "earthborn titan armor", chance = 5500},
	{id = "paladin armor", chance = 5500},
	{id = 9808, chance = 50500},
	{id = "gold ingot", chance = 33000, maxCount = 4},
	{id = "dragon scale boots", chance = 5500}
}

mType:register(monster)
