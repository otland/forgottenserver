local mType = Game.createMonsterType("corym skirmisher")
local monster = {}

monster.name = "Corym Skirmisher"
monster.description = "a corym skirmisher"
monster.experience = 260
monster.outfit = {
	lookType = 533,
	lookHead = 0,
	lookBody = 76,
	lookLegs = 83,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 450
monster.maxHealth = 450
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 19726
monster.speed = 200
monster.summonCost = 695
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = true,
	attackable = true,
	hostile = true,
	convinceable = true,
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
	{text = "Squeak! Squeak!", yell = false},
	{text = "<sniff><sniff><sniff>", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = -20},
	{type = COMBAT_DEATHDAMAGE , percent = 20}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -110, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -90, range = 7, ShootEffect = CONST_ANI_WHIRLWINDCLUB}
}

monster.defenses = {
	defense = 15,
	armor = 15
}

monster.loot = {
	{id = "gold coin", chance = 50000, maxCount = 45},
	{id = 2696, chance = 20000},
	{id = "bola", chance = 7692},
	{id = "spike shield", chance = 645},
	{id = "ratana", chance = 2702},
	{id = "life preserver", chance = 2941},
	{id = "cheese cutter", chance = 8333},
	{id = "cheesy figurine", chance = 854},
	{id = "earflap", chance = 9090},
	{id = "soft cheese", chance = 14285},
	{id = "rat cheese", chance = 16666},
	{id = "rat god doll", chance = 12},
	{id = "leather harness", chance = 561}
}

mType:register(monster)
