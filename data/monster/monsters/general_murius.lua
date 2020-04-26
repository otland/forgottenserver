local mType = Game.createMonsterType("general murius")
local monster = {}

monster.name = "General Murius"
monster.description = "General Murius"
monster.experience = 450
monster.outfit = {
	lookType = 611,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 550
monster.maxHealth = 550
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 23462
monster.speed = 240
monster.summonCost = 0
monster.maxSummons = 4

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
	{text = "Feel the power of the Mooh'Tah!", yell = false},
	{text = "You will get what you deserve!", yell = false},
	{text = "For the king!", yell = false},
	{text = "Guards!", yell = false}
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
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -220, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 1000, chance = 12, type = COMBAT_PHYSICALDAMAGE, minDamage = -50, maxDamage = -120, range = 7, ShootEffect = CONST_ANI_BOLT},
	{name ="combat", interval = 1000, chance = 10, type = COMBAT_PHYSICALDAMAGE, minDamage = -50, maxDamage = -80, radius = 3, target = false, effect = CONST_ME_HITAREA}
}

monster.defenses = {
	defense = 22,
	armor = 16,
	{name ="combat", interval = 1000, chance = 15, type = COMBAT_HEALING, minDamage = 50, maxDamage = 100, effect = CONST_ME_MAGIC_RED}
}

monster.summons = {
	{name = "Minotaur Archer", chance = 15, interval = 1000, max = 2},
	{name = "Minotaur Guard", chance = 12, interval = 1000, max = 2}
}

monster.loot = {
	{id = "minotaur horn", chance = 100000, maxCount = 2},
	{id = "minotaur leather", chance = 100000},
	{id = "gold coin", chance = 92000, maxCount = 97},
	{id = "platinum coin", chance = 92000, maxCount = 3},
	{id = "brass armor", chance = 76000},
	{id = "double axe", chance = 76000},
	{id = "piercing bolt", chance = 40000, maxCount = 7},
	{id = "meat", chance = 30000, maxCount = 2},
	{id = "battle shield", chance = 23000},
	{id = "chain legs", chance = 23000},
	{id = 7401, chance = 23000},
	{id = "power bolt", chance = 23000, maxCount = 7},
	{id = "dwarven helmet", chance = 100}
}

mType:register(monster)
