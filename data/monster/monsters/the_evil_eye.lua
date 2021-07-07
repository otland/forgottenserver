local mType = Game.createMonsterType("the evil eye")
local monster = {}

monster.name = "The Evil Eye"
monster.description = "The Evil Eye"
monster.experience = 750
monster.outfit = {
	lookType = 210,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1200
monster.maxHealth = 1200
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 6037
monster.speed = 240
monster.summonCost = 0
monster.maxSummons = 5

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
	{text = "Inferior creatures, bow before my power!", yell = false},
	{text = "653768764!", yell = false}
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
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = -20},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -90, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 1000, chance = 15, type = COMBAT_ENERGYDAMAGE, minDamage = -60, maxDamage = -130, range = 7, ShootEffect = CONST_ANI_ENERGY},
	{name ="combat", interval = 1000, chance = 13, type = COMBAT_FIREDAMAGE, minDamage = -85, maxDamage = -115, range = 7, ShootEffect = CONST_ANI_FIRE},
	{name ="combat", interval = 1000, chance = 17, type = COMBAT_PHYSICALDAMAGE, minDamage = -135, maxDamage = -175, range = 7, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_MORTAREA},
	{name ="combat", interval = 1000, chance = 15, type = COMBAT_EARTHDAMAGE, minDamage = -40, maxDamage = -120, range = 7, ShootEffect = CONST_ANI_POISON},
	{name ="combat", interval = 1000, chance = 12, type = COMBAT_LIFEDRAIN, minDamage = -110, maxDamage = -130, range = 7, effect = CONST_ME_MAGIC_RED},
	{name ="speed", interval = 1000, chance = 10, speed = {min = -850, max = -850}, duration = 20000, range = 7, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 1000, chance = 8, type = COMBAT_EARTHDAMAGE, minDamage = -35, maxDamage = -85, length = 8, spread = 3, effect = CONST_ME_GREEN_RINGS},
	{name ="combat", interval = 1000, chance = 6, type = COMBAT_LIFEDRAIN, minDamage = -75, maxDamage = -85, length = 8, spread = 3, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 1000, chance = 9, type = COMBAT_MANADRAIN, minDamage = -150, maxDamage = -250, length = 8, spread = 3, effect = CONST_ME_LOSEENERGY}
}

monster.defenses = {
	defense = 23,
	armor = 19,
	{name ="combat", interval = 1000, chance = 9, type = COMBAT_HEALING, minDamage = 1, maxDamage = 219, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "demon skeleton", chance = 13, interval = 1000, max = 5},
	{name = "ghost", chance = 12, interval = 1000, max = 4}
}

monster.loot = {
	{id = "bonelord eye", chance = 100000},
	{id = "gold coin", chance = 33000, maxCount = 29},
	{id = "gold coin", chance = 33000, maxCount = 100},
	{id = "gold coin", chance = 33000, maxCount = 100},
	{id = "small flask of eyedrops", chance = 100000},
	{id = "morning star", chance = 50000},
	{id = "steel shield", chance = 50000},
	{id = "two handed sword", chance = 34000},
	{id = "longsword", chance = 23000},
	{id = 2175, chance = 23000},
	{id = "great mana potion", chance = 15000},
	{id = "bonelord shield", chance = 11000},
	{id = "terra mantle", chance = 11000},
	{id = "terra rod", chance = 500},
	{id = "bonelord helmet", chance = 100}
}

mType:register(monster)
