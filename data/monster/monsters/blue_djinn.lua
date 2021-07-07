local mType = Game.createMonsterType("blue djinn")
local monster = {}

monster.name = "Blue Djinn"
monster.description = "a blue djinn"
monster.experience = 215
monster.outfit = {
	lookType = 80,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 330
monster.maxHealth = 330
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 6020
monster.speed = 220
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
	canPushCreatures = false,
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
	{text = "Simsalabim", yell = false},
	{text = "Feel the power of my magic, tiny mortal!", yell = false},
	{text = "Be careful what you wish for.", yell = false},
	{text = "Wishes can come true.", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 50},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 80},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE , percent = 20},
	{type = COMBAT_DEATHDAMAGE , percent = -13}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -110, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_FIREDAMAGE, minDamage = -45, maxDamage = -80, range = 7, ShootEffect = CONST_ANI_FIRE},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_DEATHDAMAGE, minDamage = -60, maxDamage = -105, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_SMALLCLOUDS},
	{name ="drunk", interval = 2000, chance = 10, drunkenness = 25, duration = 5000, range = 7, ShootEffect = CONST_ANI_ENERGY},
	{name ="outfit", interval = 2000, chance = 1, monster = "rabbit", duration = 4000, range = 7, effect = CONST_ME_MAGIC_BLUE},
	{name ="djinn electrify", interval = 2000, chance = 15, range = 5},
	{name ="djinn cancel invisibility", interval = 2000, chance = 10}
}

monster.defenses = {
	defense = 20,
	armor = 20
}

monster.loot = {
	{id = "gold coin", chance = 43000, maxCount = 100},
	{id = "gold coin", chance = 43000, maxCount = 15},
	{id = 2684, chance = 23480},
	{id = "royal spear", chance = 4500, maxCount = 2},
	{id = "small sapphire", chance = 2560, maxCount = 4},
	{id = 1963, chance = 2450},
	{id = "blue piece of cloth", chance = 1920},
	{id = "dirty turban", chance = 1890},
	{id = "mana potion", chance = 860},
	{id = "small oil lamp", chance = 730},
	{id = "blue rose", chance = 440},
	{id = "mystic turban", chance = 70}
}

mType:register(monster)
