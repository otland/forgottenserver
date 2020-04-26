local mType = Game.createMonsterType("ogre savage")
local monster = {}

monster.name = "Ogre Savage"
monster.description = "an ogre savage"
monster.experience = 625
monster.outfit = {
	lookType = 858,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 800
monster.maxHealth = 800
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 24803
monster.speed = 105
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 2000,
	chance = 5
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
	{text = "You tasty!", yell = false},
	{text = "Must! Chop! Food! Raahh!", yell = false},
	{text = "UGGA UGGA!!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 20},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = 50},
	{type = COMBAT_DEATHDAMAGE , percent = 10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, skill = 70, attack = 60, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -140, range = 7, ShootEffect = CONST_ANI_WHIRLWINDCLUB},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_PHYSICALDAMAGE, minDamage = -60, maxDamage = -200, range = 1, radius = 5, target = false, effect = CONST_ME_EXPLOSIONAREA},
	{name ="condition", interval = 2000, chance = 15, type = CONDITION_BLEEDING, startDamage = 0, tick = 4000, minDamage = -20, maxDamage = -25, range = 1, radius = 4, target = false, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 24,
	armor = 30,
	{name ="speed", interval = 2000, chance = 15, speed = {min = 210, max = 210}, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = "gold coin", chance = 100000, maxCount = 100},
	{id = "gold coin", chance = 50000, maxCount = 54},
	{id = "ogre ear stud", chance = 19720},
	{id = "ogre nose ring", chance = 17360},
	{id = "strong health potion", chance = 15330},
	{id = "skull fetish", chance = 9300},
	{id = "cookie", chance = 8350, maxCount = 5},
	{id = "mysterious fetish", chance = 6120},
	{id = "jalapeno pepper", chance = 4130, maxCount = 2},
	{id = "onyx chip", chance = 3060, maxCount = 2},
	{id = "small ruby", chance = 2840, maxCount = 2},
	{id = "small topaz", chance = 2500, maxCount = 2},
	{id = "opal", chance = 2240, maxCount = 2},
	{id = "war hammer", chance = 1510},
	{id = "ogre choppa", chance = 1160},
	{id = "berserk potion", chance = 520},
	{id = "shamanic mask", chance = 340},
	{id = "dreaded cleaver", chance = 40}
}

mType:register(monster)
