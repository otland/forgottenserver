local mType = Game.createMonsterType("kollos")
local monster = {}

monster.name = "Kollos"
monster.description = "Kollos"
monster.experience = 2400
monster.outfit = {
	lookType = 458,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 3800
monster.maxHealth = 3800
monster.runHealth = 0
monster.race = "venom"
monster.corpse = 15354
monster.speed = 200
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
	pushable = true,
	boss = false,
	illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	staticAttackChance = 95,
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
	{text = "Zopp!", yell = false},
	{text = "Flzlzlzlzlzlz!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 5},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 30},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = -5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -315, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -500, range = 7, radius = 3, target = true, ShootEffect = CONST_ANI_EXPLOSION, effect = CONST_ME_EXPLOSIONHIT}
}

monster.defenses = {
	defense = 35,
	armor = 35
}

monster.loot = {
	{id = "small ruby", chance = 8180, maxCount = 2},
	{id = "gold coin", chance = 49000, maxCount = 100},
	{id = "gold coin", chance = 50000, maxCount = 97},
	{id = "platinum coin", chance = 66000, maxCount = 3},
	{id = "ring of healing", chance = 3270},
	{id = "steel boots", chance = 130},
	{id = "great mana potion", chance = 8950, maxCount = 4},
	{id = 7632, chance = 2600},
	{id = "ultimate health potion", chance = 4000, maxCount = 3},
	{id = "gold ingot", chance = 5160},
	{id = "kollos shell", chance = 15390},
	{id = "compound eye", chance = 15720},
	{id = "calopteryx cape", chance = 360},
	{id = "carapace shield", chance = 310},
	{id = "hive scythe", chance = 700},
	{id = "buggy backpack", chance = 460},
	{id = "tarsal arrow", chance = 10210, maxCount = 5}
}

mType:register(monster)
