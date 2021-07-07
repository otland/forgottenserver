local mType = Game.createMonsterType("barbarian brutetamer")
local monster = {}

monster.name = "Barbarian Brutetamer"
monster.description = "a barbarian brutetamer"
monster.experience = 90
monster.outfit = {
	lookType = 264,
	lookHead = 78,
	lookBody = 97,
	lookLegs = 95,
	lookFeet = 121,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 145
monster.maxHealth = 145
monster.runHealth = 10
monster.race = "blood"
monster.corpse = 20339
monster.speed = 190
monster.summonCost = 0
monster.maxSummons = 2

monster.changeTarget = {
	interval = 60000,
	chance = 0
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
	canPushCreatures = false,
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
	{text = "To me, creatures of the wild!", yell = false},
	{text = "My instincts tell me about your cowardice.", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -20},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 50},
	{type = COMBAT_HOLYDAMAGE , percent = 10},
	{type = COMBAT_DEATHDAMAGE , percent = -1}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -20, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -34, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_SNOWBALL},
	{name ="barbarian brutetamer skill reducer", interval = 2000, chance = 15, range = 5}
}

monster.defenses = {
	defense = 10,
	armor = 10,
	{name ="combat", interval = 2000, chance = 40, type = COMBAT_HEALING, minDamage = 50, maxDamage = 80, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "War Wolf", chance = 10, interval = 2000}
}

monster.loot = {
	{id = 1958, chance = 5000},
	{id = "gold coin", chance = 58000, maxCount = 15},
	{id = 2401, chance = 6500},
	{id = "chain armor", chance = 8900},
	{id = "corncob", chance = 11000, maxCount = 2},
	{id = "hunting spear", chance = 5250},
	{id = "fur bag", chance = 7540},
	{id = "brutetamer's staff", chance = 380},
	{id = "fur boots", chance = 160},
	{id = "mammoth fur cape", chance = 160},
	{id = "mammoth fur shorts", chance = 90},
	{id = "mana potion", chance = 630}
}

mType:register(monster)
