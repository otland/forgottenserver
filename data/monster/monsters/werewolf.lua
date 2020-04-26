local mType = Game.createMonsterType("werewolf")
local monster = {}

monster.name = "Werewolf"
monster.description = "a werewolf"
monster.experience = 1900
monster.outfit = {
	lookType = 308,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1955
monster.maxHealth = 1955
monster.runHealth = 300
monster.race = "blood"
monster.corpse = 20380
monster.speed = 200
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
	boss = false,
	illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	staticAttackChance = 80,
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
	{text = "BLOOD!", yell = true},
	{text = "HRAAAAAAAAAARRRRRR!", yell = true}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 5},
	{type = COMBAT_EARTHDAMAGE, percent = 65},
	{type = COMBAT_FIREDAMAGE, percent = -5},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE , percent = -5},
	{type = COMBAT_DEATHDAMAGE , percent = 50}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -350, effect = CONST_ME_DRAWBLOOD},
	{name ="outfit", interval = 2000, chance = 1, monster = "werewolf", duration = 2000, radius = 1, target = true, effect = CONST_ME_SOUND_BLUE},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_LIFEDRAIN, minDamage = -80, maxDamage = -200, length = 4, spread = 2, effect = CONST_ME_SOUND_RED},
	{name ="combat", interval = 2000, chance = 40, type = COMBAT_PHYSICALDAMAGE, radius = 3, target = false, effect = CONST_ME_SOUND_WHITE},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_PHYSICALDAMAGE, radius = 1, target = false, effect = CONST_ME_SOUND_GREEN},
	{name ="werewolf skill reducer", interval = 2000, chance = 15, range = 1}
}

monster.defenses = {
	defense = 40,
	armor = 40,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 120, maxDamage = 225, effect = CONST_ME_MAGIC_GREEN},
	{name ="speed", interval = 2000, chance = 15, speed = {min = 400, max = 400}, duration = 5000, range = 7, effect = CONST_ME_SOUND_PURPLE}
}

monster.summons = {
	{name = "war wolf", chance = 40, interval = 2000}
}

monster.loot = {
	{id = "gold coin", chance = 35000, maxCount = 90},
	{id = "gold coin", chance = 35000, maxCount = 90},
	{id = "gold coin", chance = 35000, maxCount = 45},
	{id = 2169, chance = 800},
	{id = "platinum amulet", chance = 870},
	{id = "stone skin amulet", chance = 1000},
	{id = "halberd", chance = 3000},
	{id = "epee", chance = 560},
	{id = "plate shield", chance = 10340},
	{id = "plate legs", chance = 6500},
	{id = "brown mushroom", chance = 6940},
	{id = "troll green", chance = 1900},
	{id = "wolf paw", chance = 5200},
	{id = "relic sword", chance = 480},
	{id = "dreaded cleaver", chance = 160},
	{id = "bonebreaker", chance = 400},
	{id = "berserk potion", chance = 1200},
	{id = "strong health potion", chance = 5000},
	{id = "ultimate health potion", chance = 2400},
	{id = 9809, chance = 210},
	{id = "werewolf fur", chance = 10650}
}

mType:register(monster)
