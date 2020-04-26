local mType = Game.createMonsterType("necropharus")
local monster = {}

monster.name = "Necropharus"
monster.description = "Necropharus"
monster.experience = 1050
monster.outfit = {
	lookType = 209,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 750
monster.maxHealth = 750
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 20574
monster.speed = 240
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
	{text = "You will rise as my servant!", yell = false},
	{text = "Praise to my master Urgith!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -102, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 95, interval = 4000}},
	{name ="combat", interval = 1000, chance = 20, type = COMBAT_LIFEDRAIN, minDamage = -80, maxDamage = -120, range = 1, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 1000, chance = 17, type = COMBAT_EARTHDAMAGE, minDamage = -50, maxDamage = -140, range = 7, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_POISONAREA}
}

monster.defenses = {
	defense = 25,
	armor = 25,
	{name ="combat", interval = 1000, chance = 25, type = COMBAT_HEALING, minDamage = 60, maxDamage = 90, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "ghoul", chance = 20, interval = 1000, max = 2},
	{name = "ghost", chance = 17, interval = 1000, max = 1},
	{name = "mummy", chance = 15, interval = 1000, max = 1}
}

monster.loot = {
	{id = "gold coin", chance = 100000, maxCount = 100},
	{id = "book of necromantic rituals", chance = 100000},
	{id = "necromantic robe", chance = 100000},
	{id = "soul stone", chance = 100000},
	{id = "clerical mace", chance = 55000},
	{id = "skull staff", chance = 44000},
	{id = "bone club", chance = 36000},
	{id = "green mushroom", chance = 25000},
	{id = 2229, chance = 13000},
	{id = "mystic turban", chance = 11000},
	{id = "moonlight rod", chance = 8300},
	{id = 2231, chance = 5500},
	{id = "bone shield", chance = 5500},
	{id = "boots of haste", chance = 2700},
	{id = "strong mana potion", chance = 2700}
}

mType:register(monster)
