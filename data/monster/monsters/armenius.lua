local mType = Game.createMonsterType("armenius")
local monster = {}

monster.name = "Armenius"
monster.description = "Armenius"
monster.experience = 500
monster.outfit = {
	lookType = 79,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 550
monster.maxHealth = 550
monster.runHealth = 0
monster.race = "venom"
monster.corpse = 6021
monster.speed = 330
monster.summonCost = 0
monster.maxSummons = 3

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
	{type = COMBAT_FIREDAMAGE, percent = -15},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -210, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 100, interval = 4000}},
	{name ="combat", interval = 1000, chance = 12, type = COMBAT_EARTHDAMAGE, minDamage = -15, maxDamage = -135, range = 7, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_POISONAREA},
	{name ="speed", interval = 1000, chance = 13, speed = {min = -700, max = -700}, duration = 25000, range = 7, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_POISONAREA},
	{name ="condition", interval = 3000, chance = 17, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -30, maxDamage = -100, radius = 5, target = false, effect = CONST_ME_POISONAREA}
}

monster.defenses = {
	defense = 20,
	armor = 25,
	{name ="speed", interval = 1000, chance = 8, speed = {min = 350, max = 350}, duration = 9000, effect = CONST_ME_MAGIC_RED}
}

monster.summons = {
	{name = "Larva", chance = 15, interval = 1000, max = 3}
}

monster.loot = {
	{id = "scarab amulet", chance = 2063},
	{id = 2142, chance = 2564},
	{id = "gold coin", chance = 100000, maxCount = 90},
	{id = "gold coin", chance = 100000, maxCount = 90},
	{id = "small emerald", chance = 4347, maxCount = 3},
	{id = "small amethyst", chance = 4166, maxCount = 4},
	{id = "scarab coin", chance = 7692, maxCount = 2},
	{id = 2162, chance = 11111},
	{id = "daramian waraxe", chance = 781},
	{id = "plate armor", chance = 10000},
	{id = "scarab shield", chance = 518},
	{id = "strong health potion", chance = 1041},
	{id = "terra hood", chance = 1298},
	{id = "scarab pincers", chance = 3571}
}

mType:register(monster)
