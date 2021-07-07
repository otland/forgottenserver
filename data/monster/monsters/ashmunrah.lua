local mType = Game.createMonsterType("ashmunrah")
local monster = {}

monster.name = "Ashmunrah"
monster.description = "Ashmunrah"
monster.experience = 3100
monster.outfit = {
	lookType = 87,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 5000
monster.maxHealth = 5000
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 6031
monster.speed = 430
monster.summonCost = 0
monster.maxSummons = 4

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
	{text = "No mortal or undead will steal my secrets!", yell = false},
	{text = "Ahhhh all those long years.", yell = false},
	{text = "My traitorous son has thee.", yell = false},
	{text = "Come to me, my allys and underlings.", yell = false},
	{text = "I might be trapped but not without power.", yell = false},
	{text = "Ages come, ages go. Ashmunrah remains.", yell = false},
	{text = "You will be history soon.", yell = false}
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
	{type = COMBAT_EARTHDAMAGE, percent = 25},
	{type = COMBAT_FIREDAMAGE, percent = -5},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = -10},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -1000, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 55, interval = 4000}},
	{name ="combat", interval = 3000, chance = 7, type = COMBAT_LIFEDRAIN, minDamage = -100, maxDamage = -700, range = 1},
	{name ="combat", interval = 2000, chance = 12, type = COMBAT_EARTHDAMAGE, minDamage = -100, maxDamage = -500, range = 7, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_POISONAREA},
	{name ="combat", interval = 3000, chance = 12, type = COMBAT_PHYSICALDAMAGE, minDamage = -120, maxDamage = -750, range = 7, ShootEffect = CONST_ANI_ENERGY, effect = CONST_ME_MORTAREA},
	{name ="speed", interval = 3000, chance = 25, speed = {min = -650, max = -650}, duration = 50000, range = 7, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 2000, chance = 18, type = COMBAT_LIFEDRAIN, minDamage = -50, maxDamage = -550, length = 8, spread = 3, effect = CONST_ME_YELLOW_RINGS}
}

monster.defenses = {
	defense = 30,
	armor = 25,
	{name ="combat", interval = 1000, chance = 20, type = COMBAT_HEALING, minDamage = 200, maxDamage = 400, effect = CONST_ME_MAGIC_BLUE},
	{name ="invisible", interval = 1000, chance = 7, duration = 2000, effect = CONST_ME_MAGIC_BLUE},
	{name ="outfit", interval = 1000, chance = 3, monster = "ancient scarab", duration = 6000, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "Ancient Scarab", chance = 100, interval = 1000, max = 2},
	{name = "Green Djinn", chance = 100, interval = 1000, max = 4}
}

monster.loot = {
	{id = "gold coin", chance = 33000, maxCount = 100},
	{id = "gold coin", chance = 33000, maxCount = 100},
	{id = "gold coin", chance = 33000, maxCount = 100},
	{id = "gold coin", chance = 50000, maxCount = 4},
	{id = "great mana potion", chance = 12500},
	{id = "might ring", chance = 4600},
	{id = "silver brooch", chance = 4000},
	{id = "crown armor", chance = 470},
	{id = "mini mummy", chance = 180},
	{id = "hammer of wrath", chance = 120},
	{id = "holy scarab", chance = 120}
}

mType:register(monster)
