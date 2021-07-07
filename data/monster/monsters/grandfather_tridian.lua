local mType = Game.createMonsterType("grandfather tridian")
local monster = {}

monster.name = "Grandfather Tridian"
monster.description = "Grandfather Tridian"
monster.experience = 1400
monster.outfit = {
	lookType = 193,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1800
monster.maxHealth = 1800
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 20391
monster.speed = 210
monster.summonCost = 0
monster.maxSummons = 2

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
	boss = true,
	illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	staticAttackChance = 50,
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
	{text = "I will bring peace to your misguided soul!", yell = false},
	{text = "Your intrusion can't be tolerated!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 35},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 5},
	{type = COMBAT_HOLYDAMAGE , percent = 20},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -100, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_LIFEDRAIN, minDamage = -138, maxDamage = -362, range = 1, radius = 1, target = true, ShootEffect = CONST_ANI_HOLY, effect = CONST_ME_HOLYAREA},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_LIFEDRAIN, minDamage = 0, maxDamage = -80, range = 1, radius = 1, target = true, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 25,
	armor = 25,
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_HEALING, minDamage = 60, maxDamage = 90, effect = CONST_ME_MAGIC_BLUE},
	{name ="invisible", interval = 2000, chance = 15, duration = 4000, effect = CONST_ME_YELLOW_RINGS}
}

monster.summons = {
	{name = "crypt shambler", chance = 10, interval = 2000},
	{name = "ghost", chance = 10, interval = 2000}
}

monster.loot = {
	{id = "gold coin", chance = 100000, maxCount = 80},
	{id = "piggy bank", chance = 100000},
	{id = "strong mana potion", chance = 5000},
	{id = "brown mushroom", chance = 5000},
	{id = "wand of inferno", chance = 5000},
	{id = "skull staff", chance = 5000},
	{id = "wand of voodoo", chance = 5000},
	{id = "amber staff", chance = 3000},
	{id = 6087, chance = 3000},
	{id = 6088, chance = 3000},
	{id = 6089, chance = 3000},
	{id = 6090, chance = 3000},
	{id = 3955, chance = 1000},
	{id = "cultish mask", chance = 10250},
	{id = 1962, chance = 910},
	{id = "cultish symbol", chance = 890},
	{id = "protection amulet", chance = 790},
	{id = "small sapphire", chance = 550},
	{id = "energy ring", chance = 450},
	{id = "pirate voodoo doll", chance = 430},
	{id = "platinum amulet", chance = 200},
	{id = "mysterious voodoo skull", chance = 130},
	{id = "broken key ring", chance = 100},
	{id = "jewelled backpack", chance = 100},
	{id = "blue robe", chance = 40}
}

mType:register(monster)
