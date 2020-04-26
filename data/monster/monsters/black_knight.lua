local mType = Game.createMonsterType("black knight")
local monster = {}

monster.name = "Black Knight"
monster.description = "a black knight"
monster.experience = 1600
monster.outfit = {
	lookType = 131,
	lookHead = 95,
	lookBody = 95,
	lookLegs = 95,
	lookFeet = 95,
	lookAddons = 3,
	lookMount = 0
}

monster.health = 1800
monster.maxHealth = 1800
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 20355
monster.speed = 250
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
	{text = "MINE!", yell = false},
	{text = "NO PRISONERS!", yell = false},
	{text = "NO MERCY!", yell = false},
	{text = "By Bolg's blood!", yell = false},
	{text = "You're no match for me!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 80},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 95},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = -8},
	{type = COMBAT_DEATHDAMAGE , percent = 20}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -300, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -200, range = 7, ShootEffect = CONST_ANI_SPEAR}
}

monster.defenses = {
	defense = 42,
	armor = 42
}

monster.loot = {
	{id = "gold coin", chance = 24000, maxCount = 100},
	{id = "gold coin", chance = 24000, maxCount = 36},
	{id = "spear", chance = 30800, maxCount = 3},
	{id = "brown bread", chance = 21600, maxCount = 2},
	{id = 2120, chance = 14020},
	{id = "brass legs", chance = 13200},
	{id = "halberd", chance = 12050},
	{id = "plate armor", chance = 11000},
	{id = "steel helmet", chance = 10500},
	{id = "two handed sword", chance = 9600},
	{id = "battle hammer", chance = 6980},
	{id = "warrior helmet", chance = 4700},
	{id = "knight axe", chance = 2600},
	{id = "dark helmet", chance = 2330},
	{id = "dark armor", chance = 1900},
	{id = "knight legs", chance = 1050},
	{id = "knight armor", chance = 750},
	{id = "ruby necklace", chance = 730},
	{id = "lightning legs", chance = 580},
	{id = "boots of haste", chance = 410},
	{id = "dragon lance", chance = 290},
	{id = "piggy bank", chance = 120}
}

mType:register(monster)
