local mType = Game.createMonsterType("fernfang")
local monster = {}

monster.name = "Fernfang"
monster.description = "Fernfang"
monster.experience = 600
monster.outfit = {
	lookType = 206,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 400
monster.maxHealth = 400
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 20566
monster.speed = 240
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
	{text = "You desecrated this place!", yell = false},
	{text = "I will cleanse this isle!", yell = false},
	{text = "Grrrrrrr", yell = false},
	{text = "Yoooohhuuuu!", yell = true}
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
	{type = COMBAT_EARTHDAMAGE, percent = 40},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 70},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -50, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 1000, chance = 25, type = COMBAT_MANADRAIN, minDamage = -25, maxDamage = -45, range = 7, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 2000, chance = 80, type = COMBAT_HOLYDAMAGE, minDamage = -65, maxDamage = -180, range = 7, ShootEffect = CONST_ANI_HOLY, effect = CONST_ME_HOLYAREA}
}

monster.defenses = {
	defense = 10,
	armor = 15,
	{name ="combat", interval = 1000, chance = 15, type = COMBAT_HEALING, minDamage = 90, maxDamage = 120, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 1000, chance = 7, speed = {min = 310, max = 310}, duration = 10000, effect = CONST_ME_MAGIC_RED},
	{name ="effect", interval = 1000, chance = 5, effect = CONST_ME_MAGIC_RED},
	{name ="outfit", interval = 1000, chance = 5, monster = "war wolf", duration = 14000, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "War Wolf", chance = 13, interval = 1000}
}

monster.loot = {
	{id = "gold coin", chance = 100000, maxCount = 94},
	{id = "book of prayers", chance = 100000},
	{id = "platinum coin", chance = 97000, maxCount = 3},
	{id = "star herb", chance = 92000},
	{id = "rope belt", chance = 56000},
	{id = "yellow gem", chance = 43000},
	{id = "safety pin", chance = 41000},
	{id = "power ring", chance = 41000},
	{id = "blank rune", chance = 17000},
	{id = "brown flask", chance = 12000},
	{id = "bread", chance = 12000},
	{id = 2044, chance = 12000},
	{id = 2401, chance = 12000},
	{id = "green tunic", chance = 12000},
	{id = "strong mana potion", chance = 10200},
	{id = "wooden whistle", chance = 7600},
	{id = "sling herb", chance = 5100},
	{id = "wolf tooth chain", chance = 5100},
	{id = "life crystal", chance = 2500}
}

mType:register(monster)
