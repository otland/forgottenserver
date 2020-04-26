local mType = Game.createMonsterType("the horned fox")
local monster = {}

monster.name = "The Horned Fox"
monster.description = "The Horned Fox"
monster.experience = 300
monster.outfit = {
	lookType = 202,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 265
monster.maxHealth = 265
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 5983
monster.speed = 210
monster.summonCost = 0
monster.maxSummons = 6

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
	{text = "You will never get me!", yell = false},
	{text = "I'll be back!", yell = false},
	{text = "Catch me, if you can!", yell = false},
	{text = "Help me, boys!", yell = false}
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
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -1},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = -1}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -122, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 1000, chance = 25, type = COMBAT_PHYSICALDAMAGE, minDamage = -50, maxDamage = -120, range = 7, ShootEffect = CONST_ANI_BOLT},
	{name ="condition", interval = 1000, chance = 17, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -50, maxDamage = -120, range = 7, ShootEffect = CONST_ANI_BOLT}
}

monster.defenses = {
	defense = 16,
	armor = 16,
	{name ="combat", interval = 1000, chance = 15, type = COMBAT_HEALING, minDamage = 25, maxDamage = 75, effect = CONST_ME_MAGIC_RED},
	{name ="invisible", interval = 1000, chance = 10, duration = 2000, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "minotaur archer", chance = 13, interval = 1000, max = 2},
	{name = "minotaur guard", chance = 13, interval = 1000, max = 2},
	{name = "minotaur mage", chance = 13, interval = 1000, max = 2}
}

monster.loot = {
	{id = "nose ring", chance = 100000},
	{id = "gold coin", chance = 97000, maxCount = 99},
	{id = "minotaur leather", chance = 97000},
	{id = "minotaur horn", chance = 93000, maxCount = 2},
	{id = "piece of warrior armor", chance = 81000},
	{id = "piercing bolt", chance = 46000, maxCount = 14},
	{id = "brass armor", chance = 28000},
	{id = "meat", chance = 15000},
	{id = "battle shield", chance = 12150},
	{id = "dwarven helmet", chance = 9300},
	{id = "strong health potion", chance = 9300},
	{id = "fishing rod", chance = 6250},
	{id = "double axe", chance = 3130}
}

mType:register(monster)
