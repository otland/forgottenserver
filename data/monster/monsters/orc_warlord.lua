local mType = Game.createMonsterType("orc warlord")
local monster = {}

monster.name = "Orc Warlord"
monster.description = "an orc warlord"
monster.experience = 670
monster.outfit = {
	lookType = 2,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 950
monster.maxHealth = 950
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 6008
monster.speed = 234
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
	{text = "Ranat Ulderek!", yell = false},
	{text = "Orc buta bana!", yell = false},
	{text = "Ikem rambo zambo!", yell = false},
	{text = "Futchi maruk buta!", yell = false}
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
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 80},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 10},
	{type = COMBAT_DEATHDAMAGE , percent = -5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -250, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -200, range = 7, ShootEffect = CONST_ANI_THROWINGSTAR}
}

monster.defenses = {
	defense = 28,
	armor = 28,
	{name ="invisible", interval = 2000, chance = 5, duration = 2000, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "broken helmet", chance = 24350},
	{id = "orc leather", chance = 20620},
	{id = "gold coin", chance = 18000, maxCount = 45},
	{id = "throwing star", chance = 14000, maxCount = 18},
	{id = 2667, chance = 10800, maxCount = 2},
	{id = "orc tooth", chance = 9140},
	{id = "orcish axe", chance = 5400},
	{id = "plate armor", chance = 5210},
	{id = "skull belt", chance = 5000},
	{id = "hunting spear", chance = 4700},
	{id = "plate legs", chance = 3800},
	{id = 2419, chance = 3450},
	{id = "two handed sword", chance = 1980},
	{id = "protection amulet", chance = 1800},
	{id = "dark helmet", chance = 1260},
	{id = "brass armor", chance = 860},
	{id = "health potion", chance = 380},
	{id = "crusader helmet", chance = 330},
	{id = "magma boots", chance = 280},
	{id = "dragon hammer", chance = 230},
	{id = 7395, chance = 130},
	{id = "stealth ring", chance = 130}
}

mType:register(monster)
