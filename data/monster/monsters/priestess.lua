local mType = Game.createMonsterType("priestess")
local monster = {}

monster.name = "Priestess"
monster.description = "a priestess"
monster.experience = 420
monster.outfit = {
	lookType = 58,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 390
monster.maxHealth = 390
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 20491
monster.speed = 170
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
	illusionable = true,
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
	{text = "Your energy is mine.", yell = false},
	{text = "Now your life is come to the end, hahahaha!", yell = false},
	{text = "Throw the soul on the altar!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 70},
	{type = COMBAT_FIREDAMAGE, percent = 40},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = -10},
	{type = COMBAT_DEATHDAMAGE , percent = 10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -75, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_DEATHDAMAGE, minDamage = -55, maxDamage = -120, range = 7, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_MORTAREA},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_MANADRAIN, minDamage = -2, maxDamage = -170, range = 7},
	{name ="condition", interval = 2000, chance = 15, type = CONDITION_POISON, startDamage = 0, tick = 4000, range = 7, ShootEffect = CONST_ANI_POISON}
}

monster.defenses = {
	defense = 30,
	armor = 30,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 50, maxDamage = 80, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "ghoul", chance = 10, interval = 2000}
}

monster.loot = {
	{id = "sling herb", chance = 13200},
	{id = "goat grass", chance = 12720},
	{id = "dark rosary", chance = 9840},
	{id = "red apple", chance = 7500, maxCount = 2},
	{id = "powder herb", chance = 6100},
	{id = "black hood", chance = 5000},
	{id = "wood mushroom", chance = 3340},
	{id = "cultish robe", chance = 1700},
	{id = "clerical mace", chance = 1500},
	{id = 2374, chance = 1400},
	{id = 2192, chance = 1250},
	{id = "hailstorm rod", chance = 1100},
	{id = 1962, chance = 910},
	{id = "mana potion", chance = 910},
	{id = "talon", chance = 740},
	{id = "crystal necklace", chance = 640},
	{id = "black shield", chance = 210},
	{id = "piggy bank", chance = 90}
}

mType:register(monster)
