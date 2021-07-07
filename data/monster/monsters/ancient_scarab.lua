local mType = Game.createMonsterType("ancient scarab")
local monster = {}

monster.name = "Ancient Scarab"
monster.description = "an ancient scarab"
monster.experience = 720
monster.outfit = {
	lookType = 79,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1000
monster.maxHealth = 1000
monster.runHealth = 0
monster.race = "venom"
monster.corpse = 6021
monster.speed = 218
monster.summonCost = 0
monster.maxSummons = 3

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
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = -20},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -210, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 56, interval = 4000}},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_EARTHDAMAGE, minDamage = -15, maxDamage = -145, range = 7, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_POISONAREA},
	{name ="speed", interval = 2000, chance = 15, speed = {min = -700, max = -700}, duration = 25000, range = 7, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_POISONAREA},
	{name ="condition", interval = 2000, chance = 30, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -440, maxDamage = -520, radius = 5, target = false, effect = CONST_ME_POISONAREA}
}

monster.defenses = {
	defense = 36,
	armor = 36,
	{name ="speed", interval = 2000, chance = 15, speed = {min = 260, max = 260}, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.summons = {
	{name = "Larva", chance = 10, interval = 2000}
}

monster.loot = {
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "gold coin", chance = 50000, maxCount = 87},
	{id = 2162, chance = 10300},
	{id = "scarab coin", chance = 8100, maxCount = 2},
	{id = "scarab pincers", chance = 7140},
	{id = "small emerald", chance = 6000, maxCount = 3},
	{id = "small amethyst", chance = 6000, maxCount = 4},
	{id = "plate armor", chance = 5000},
	{id = "scarab amulet", chance = 3510},
	{id = 2142, chance = 2670},
	{id = "strong health potion", chance = 1660},
	{id = "springsprout rod", chance = 1000},
	{id = "scarab shield", chance = 500},
	{id = "terra hood", chance = 400},
	{id = "daramian waraxe", chance = 300}
}

mType:register(monster)
