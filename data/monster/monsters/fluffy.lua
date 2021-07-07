local mType = Game.createMonsterType("fluffy")
local monster = {}

monster.name = "Fluffy"
monster.description = "Fluffy"
monster.experience = 3550
monster.outfit = {
	lookType = 240,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 4500
monster.maxHealth = 4500
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 6332
monster.speed = 310
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 5000,
	chance = 8
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
	{text = "Wooof!", yell = false}
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
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -648, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 1500, chance = 30, type = COMBAT_ENERGYDAMAGE, minDamage = -100, maxDamage = -200, length = 8, spread = 3, effect = CONST_ME_LOSEENERGY},
	{name ="combat", interval = 1000, chance = 15, type = COMBAT_EARTHDAMAGE, minDamage = -120, maxDamage = -300, length = 8, spread = 3, effect = CONST_ME_POISONAREA},
	{name ="combat", interval = 1000, chance = 25, type = COMBAT_PHYSICALDAMAGE, minDamage = -105, maxDamage = -235, range = 7, ShootEffect = CONST_ANI_SUDDENDEATH},
	{name ="combat", interval = 1000, chance = 20, type = COMBAT_EARTHDAMAGE, minDamage = -135, maxDamage = -255, range = 7, radius = 6, target = false, effect = CONST_ME_LOSEENERGY}
}

monster.defenses = {
	defense = 35,
	armor = 25
}

monster.loot = {
	{id = "gold coin", chance = 100000, maxCount = 100},
	{id = "gold coin", chance = 100000, maxCount = 20},
	{id = 2230, chance = 25000},
	{id = "spike sword", chance = 2500},
	{id = "knight axe", chance = 2857},
	{id = "ham", chance = 50000, maxCount = 8},
	{id = "soul orb", chance = 5555},
	{id = "demonic essence", chance = 7200},
	{id = "concentrated demonic blood", chance = 8888},
	{id = 6570, chance = 5538, maxCount = 4},
	{id = 6571, chance = 1538}
}

mType:register(monster)
