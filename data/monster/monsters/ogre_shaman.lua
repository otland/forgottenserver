local mType = Game.createMonsterType("ogre shaman")
local monster = {}

monster.name = "Ogre Shaman"
monster.description = "an ogre shaman"
monster.experience = 625
monster.outfit = {
	lookType = 859,
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
monster.corpse = 24795
monster.speed = 105
monster.summonCost = 0
monster.maxSummons = 1

monster.changeTarget = {
	interval = 2000,
	chance = 20
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
	targetDistance = 5,
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
	{text = "Stay here, soul food!", yell = false},
	{text = "You more delicate than clomp!", yell = false},
	{text = "The spirits hunger!", yell = false},
	{text = "A sacrifice for Great Hunger!", yell = false}
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
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 20},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = -10},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, skill = 50, attack = 50, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 11, type = COMBAT_EARTHDAMAGE, minDamage = -80, maxDamage = -110, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_EARTH, effect = CONST_ME_CARNIPHILA},
	{name ="terra wave", interval = 2000, chance = 13, minDamage = -90, maxDamage = -200, effect = CONST_ME_SMALLPLANTS},
	{name ="outfit", interval = 2000, chance = 11, monster = "chicken", duration = 2000, range = 7, ShootEffect = CONST_ANI_EARTH, effect = CONST_ME_MAGIC_GREEN},
	{name ="condition", interval = 2000, chance = 15, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -20, maxDamage = -40, range = 7, ShootEffect = CONST_ANI_POISON}
}

monster.defenses = {
	defense = 24,
	armor = 30,
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_HEALING, effect = CONST_ME_MORTAREA}
}

monster.summons = {
	{name = "Terror Bird", chance = 10, interval = 2000, max = 1},
	{name = "Dworc Voodoomaster", chance = 10, interval = 2000, max = 1},
	{name = "Dworc Fleshhunter", chance = 10, interval = 2000, max = 1}
}

monster.loot = {
	{id = "gold coin", chance = 100000, maxCount = 90},
	{id = "strong mana potion", chance = 15650},
	{id = "ogre ear stud", chance = 14010},
	{id = "skull fetish", chance = 10920},
	{id = "shamanic talisman", chance = 10350},
	{id = "ogre nose ring", chance = 10040},
	{id = 2229, chance = 9700},
	{id = "prickly pear", chance = 9240, maxCount = 2},
	{id = "broken shamanic staff", chance = 6680},
	{id = "bone fetish", chance = 5080},
	{id = "small ruby", chance = 3130},
	{id = "black pearl", chance = 2600},
	{id = "necrotic rod", chance = 2600},
	{id = "wand of decay", chance = 2330},
	{id = "waterskin", chance = 2060},
	{id = "opal", chance = 1640, maxCount = 2},
	{id = "onyx chip", chance = 1340, maxCount = 2},
	{id = "shamanic mask", chance = 880},
	{id = "ogre scepta", chance = 500},
	{id = "didgeridoo", chance = 460},
	{id = 3955, chance = 40}
}

mType:register(monster)
