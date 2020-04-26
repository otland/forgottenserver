local mType = Game.createMonsterType("the many")
local monster = {}

monster.name = "The Many"
monster.description = "The Many"
monster.experience = 4000
monster.outfit = {
	lookType = 121,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 5000
monster.maxHealth = 5000
monster.runHealth = 300
monster.race = "blood"
monster.corpse = 6048
monster.speed = 260
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
	{text = "FCHHHHH", yell = false},
	{text = "HISSSS", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 50},
	{type = COMBAT_HOLYDAMAGE , percent = 30},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -270, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 7, type = COMBAT_EARTHDAMAGE, minDamage = -65, maxDamage = -320, length = 8, spread = 3, effect = CONST_ME_SMALLPLANTS},
	{name ="speed", interval = 2000, chance = 8, speed = {min = -360, max = -360}, duration = 15000, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_GREEN_RINGS},
	{name ="combat", interval = 2000, chance = 9, type = COMBAT_ICEDAMAGE, minDamage = -100, maxDamage = -250, length = 8, spread = 3, effect = CONST_ME_LOSEENERGY},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_ICEDAMAGE, minDamage = -70, maxDamage = -155, range = 7, ShootEffect = CONST_ANI_ICE, effect = CONST_ME_ICEATTACK}
}

monster.defenses = {
	defense = 35,
	armor = 25,
	{name ="combat", interval = 2000, chance = 30, type = COMBAT_HEALING, minDamage = 420, maxDamage = 500, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "small sapphire", chance = 56325, maxCount = 5},
	{id = "stone skin amulet", chance = 100000},
	{id = "warrior helmet", chance = 75500},
	{id = "royal helmet", chance = 25500},
	{id = "medusa shield", chance = 75500},
	{id = "strong mana potion", chance = 100000, maxCount = 2},
	{id = "gold ingot", chance = 47725, maxCount = 3},
	{id = "sacred tree amulet", chance = 75500},
	{id = "egg of The Many", chance = 100000}
}

mType:register(monster)
