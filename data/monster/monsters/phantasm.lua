local mType = Game.createMonsterType("phantasm")
local monster = {}

monster.name = "Phantasm"
monster.description = "a phantasm"
monster.experience = 4400
monster.outfit = {
	lookType = 241,
	lookHead = 20,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 3950
monster.maxHealth = 3950
monster.runHealth = 350
monster.race = "undead"
monster.corpse = 6344
monster.speed = 240
monster.summonCost = 0
monster.maxSummons = 4

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
	illusionable = true,
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
	{text = "Oh my, you forgot to put your pants on!", yell = false},
	{text = "Weeheeheeheehee!", yell = false},
	{text = "Its nothing but a dream!", yell = false},
	{text = "Give in!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 100},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = -10},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -475, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_DEATHDAMAGE, minDamage = -250, maxDamage = -610, range = 7, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_SMALLCLOUDS},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_MANADRAIN, minDamage = -5, maxDamage = -80, radius = 3, target = false, effect = CONST_ME_YELLOW_RINGS},
	{name ="phantasm drown", interval = 2000, chance = 15},
	{name ="drunk", interval = 2000, chance = 15, drunkenness = 25, duration = 6000, radius = 5, target = false, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 0,
	armor = 0,
	{name ="combat", interval = 2000, chance = 30, type = COMBAT_HEALING, minDamage = 228, maxDamage = 449, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 2000, chance = 10, speed = {min = 500, max = 500}, duration = 6000, effect = CONST_ME_MAGIC_RED},
	{name ="invisible", interval = 2000, chance = 25, duration = 8000, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "Phantasm Summon", chance = 20, interval = 2000}
}

monster.loot = {
	{id = "small ruby", chance = 12160, maxCount = 3},
	{id = "gold coin", chance = 30000, maxCount = 100},
	{id = "gold coin", chance = 30000, maxCount = 100},
	{id = "gold coin", chance = 28000, maxCount = 33},
	{id = "small emerald", chance = 10190, maxCount = 3},
	{id = "small amethyst", chance = 14570, maxCount = 3},
	{id = "platinum coin", chance = 87730, maxCount = 4},
	{id = "stealth ring", chance = 550},
	{id = "blank rune", chance = 22500, maxCount = 2},
	{id = "crown armor", chance = 660},
	{id = "shadow herb", chance = 26930, maxCount = 2},
	{id = 6300, chance = 330},
	{id = "demonic essence", chance = 16320},
	{id = "abyss hammer", chance = 110},
	{id = "shadow sceptre", chance = 550},
	{id = "great mana potion", chance = 32750, maxCount = 2},
	{id = "ultimate health potion", chance = 14680},
	{id = "small topaz", chance = 12810, maxCount = 3}
}

mType:register(monster)
