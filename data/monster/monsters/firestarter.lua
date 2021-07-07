local mType = Game.createMonsterType("firestarter")
local monster = {}

monster.name = "Firestarter"
monster.description = "a firestarter"
monster.experience = 80
monster.outfit = {
	lookType = 159,
	lookHead = 94,
	lookBody = 77,
	lookLegs = 78,
	lookFeet = 79,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 180
monster.maxHealth = 180
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 20599
monster.speed = 180
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = true,
	boss = false,
	illusionable = false,
	canPushItems = false,
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
	{text = "FIRE!", yell = true},
	{text = "BURN!", yell = true},
	{text = "DEATH to the FALSE GOD!!", yell = true},
	{text = "You shall burn in the thornfires!!", yell = false},
	{text = "DOWN with the followers of the bog!!", yell = false}
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
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -20},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = -20}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -15, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_FIREDAMAGE, minDamage = 0, maxDamage = -21, radius = 1, target = true, ShootEffect = CONST_ANI_BURSTARROW, effect = CONST_ME_EXPLOSIONHIT},
	{name ="firefield", interval = 2000, chance = 10, radius = 1, target = true, ShootEffect = CONST_ANI_FIRE}
}

monster.defenses = {
	defense = 15,
	armor = 15
}

monster.loot = {
	{id = "gold coin", chance = 79000, maxCount = 35},
	{id = "longsword", chance = 6000},
	{id = "bow", chance = 4000},
	{id = "grapes", chance = 20000},
	{id = 5921, chance = 930},
	{id = "elvish bow", chance = 100},
	{id = "flaming arrow", chance = 30000, maxCount = 12},
	{id = "elvish talisman", chance = 5000},
	{id = 13757, chance = 15280},
	{id = "flintstone", chance = 340}
}

mType:register(monster)
