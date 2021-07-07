local mType = Game.createMonsterType("marsh stalker")
local monster = {}

monster.name = "Marsh Stalker"
monster.description = "a marsh stalker"
monster.experience = 50
monster.outfit = {
	lookType = 530,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 100
monster.maxHealth = 100
monster.runHealth = 10
monster.race = "blood"
monster.corpse = 19708
monster.speed = 210
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 5000,
	chance = 0
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
	{text = "Rrrawk! Rrrrawk!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = -5},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 5},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -10, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -8, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_EXPLOSION, effect = CONST_ME_EXPLOSIONAREA}
}

monster.defenses = {
	defense = 5,
	armor = 5
}

monster.loot = {
	{id = 2120, chance = 4530},
	{id = "gold coin", chance = 65000, maxCount = 23},
	{id = "longsword", chance = 7180},
	{id = "meat", chance = 20360, maxCount = 2},
	{id = 2667, chance = 20290, maxCount = 2},
	{id = "worm", chance = 15000, maxCount = 2},
	{id = "seeds", chance = 120},
	{id = 19741, chance = 7180},
	{id = "marsh stalker beak", chance = 9300}
}

mType:register(monster)
