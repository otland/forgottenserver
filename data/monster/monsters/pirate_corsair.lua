local mType = Game.createMonsterType("pirate corsair")
local monster = {}

monster.name = "Pirate Corsair"
monster.description = "a pirate corsair"
monster.experience = 350
monster.outfit = {
	lookType = 98,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 675
monster.maxHealth = 675
monster.runHealth = 40
monster.race = "blood"
monster.corpse = 20475
monster.speed = 250
monster.summonCost = 775
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 15
}

monster.flags = {
	summonable = true,
	attackable = true,
	hostile = true,
	convinceable = true,
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
	{text = "Give up!", yell = false},
	{text = "Hiyaa!", yell = false},
	{text = "Plundeeeeer!", yell = false}
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
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE , percent = 10},
	{type = COMBAT_DEATHDAMAGE , percent = -5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -170, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -150, range = 7, ShootEffect = CONST_ANI_THROWINGSTAR},
	{name ="pirate corsair skill reducer", interval = 2000, chance = 5}
}

monster.defenses = {
	defense = 35,
	armor = 35
}

monster.loot = {
	{id = "piggy bank", chance = 150},
	{id = "gold coin", chance = 50000, maxCount = 88},
	{id = 2376, chance = 3000},
	{id = "sabre", chance = 10000},
	{id = "throwing star", chance = 8400, maxCount = 12},
	{id = "dark armor", chance = 1650},
	{id = "dark shield", chance = 1000},
	{id = "pirate boots", chance = 220},
	{id = "rum flask", chance = 130},
	{id = 5813, chance = 130},
	{id = "pirate backpack", chance = 930},
	{id = "pirate hat", chance = 1150},
	{id = "hook", chance = 600},
	{id = "eye patch", chance = 500},
	{id = "peg leg", chance = 600},
	{id = "strong health potion", chance = 820},
	{id = "compass", chance = 11050},
	{id = "compass", chance = 11020}
}

mType:register(monster)
