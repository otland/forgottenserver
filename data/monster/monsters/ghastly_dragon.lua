local mType = Game.createMonsterType("ghastly dragon")
local monster = {}

monster.name = "Ghastly Dragon"
monster.description = "a ghastly dragon"
monster.experience = 4600
monster.outfit = {
	lookType = 351,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 7800
monster.maxHealth = 7800
monster.runHealth = 366
monster.race = "undead"
monster.corpse = 11362
monster.speed = 320
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 5
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
	staticAttackChance = 70,
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
	{text = "EMBRACE MY GIFTS!", yell = true},
	{text = "I WILL FEAST ON YOUR SOUL!", yell = true}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 10},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 50},
	{type = COMBAT_HOLYDAMAGE , percent = -15},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -603, effect = CONST_ME_DRAWBLOOD},
	{name ="ghastly dragon curse", interval = 2000, chance = 5, range = 5},
	{name ="condition", interval = 2000, chance = 10, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -920, maxDamage = -1280, range = 5, effect = CONST_ME_SMALLCLOUDS},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_LIFEDRAIN, minDamage = -80, maxDamage = -230, range = 7, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_DEATHDAMAGE, minDamage = -120, maxDamage = -250, length = 8, spread = 3, effect = CONST_ME_LOSEENERGY},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_DEATHDAMAGE, minDamage = -110, maxDamage = -180, radius = 4, target = false, effect = CONST_ME_MORTAREA},
	{name ="speed", interval = 2000, chance = 20, speed = {min = -800, max = -800}, duration = 30000, range = 7, effect = CONST_ME_SMALLCLOUDS}
}

monster.defenses = {
	defense = 35,
	armor = 35
}

monster.loot = {
	{id = "gold coin", chance = 33725, maxCount = 100},
	{id = "gold coin", chance = 33725, maxCount = 100},
	{id = "gold coin", chance = 33725, maxCount = 66},
	{id = "platinum coin", chance = 29840, maxCount = 2},
	{id = "soul orb", chance = 12170},
	{id = "demonic essence", chance = 8920},
	{id = "great mana potion", chance = 30560, maxCount = 2},
	{id = "terra legs", chance = 3130},
	{id = "terra boots", chance = 9510},
	{id = "great spirit potion", chance = 29460, maxCount = 2},
	{id = "ultimate health potion", chance = 24700},
	{id = 9810, chance = 180},
	{id = "shiny stone", chance = 860},
	{id = "guardian boots", chance = 200},
	{id = "Zaoan armor", chance = 870},
	{id = "Zaoan helmet", chance = 150},
	{id = "Zaoan shoes", chance = 870},
	{id = "Zaoan legs", chance = 1400},
	{id = "drakinata", chance = 1470},
	{id = "Zaoan sword", chance = 100},
	{id = "twin hooks", chance = 15100},
	{id = "Zaoan halberd", chance = 15020},
	{id = "spellweaver's robe", chance = 690},
	{id = "ghastly dragon head", chance = 6650},
	{id = "undead heart", chance = 19830},
	{id = "jade hat", chance = 810}
}

mType:register(monster)
