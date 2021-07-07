local mType = Game.createMonsterType("haunted treeling")
local monster = {}

monster.name = "Haunted Treeling"
monster.description = "a haunted treeling"
monster.experience = 310
monster.outfit = {
	lookType = 310,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 450
monster.maxHealth = 450
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 9867
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
	canPushCreatures = true,
	staticAttackChance = 85,
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
	{text = "Knarrrz", yell = false},
	{text = "Huuhuuhuuuhuuaarrr", yell = false},
	{text = "Knorrrrrr", yell = false}
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
	{type = COMBAT_FIREDAMAGE, percent = -5},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 15},
	{type = COMBAT_HOLYDAMAGE , percent = 20},
	{type = COMBAT_DEATHDAMAGE , percent = 1}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -150, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 5, type = COMBAT_MANADRAIN, minDamage = -30, maxDamage = -100, radius = 4, target = false, effect = CONST_ME_GREEN_RINGS},
	{name ="speed", interval = 2000, chance = 15, speed = {min = -700, max = -700}, duration = 15000, length = 5, spread = 0, effect = CONST_ME_SMALLPLANTS},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -100, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_SMALLEARTH, effect = CONST_ME_CARNIPHILA},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_EARTHDAMAGE, minDamage = -55, maxDamage = -100, radius = 4, target = true, effect = CONST_ME_HITBYPOISON},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_EARTHDAMAGE, radius = 1, target = false, effect = CONST_ME_POISONAREA}
}

monster.defenses = {
	defense = 20,
	armor = 20
}

monster.loot = {
	{id = "small sapphire", chance = 585},
	{id = "gold coin", chance = 60000, maxCount = 95},
	{id = "dwarven ring", chance = 621},
	{id = "white mushroom", chance = 5263, maxCount = 2},
	{id = "red mushroom", chance = 7142},
	{id = "orange mushroom", chance = 1851},
	{id = 4860, chance = 25000},
	{id = "bullseye potion", chance = 80},
	{id = "strong health potion", chance = 900},
	{id = "health potion", chance = 5000},
	{id = "haunted piece of wood", chance = 5000}
}

mType:register(monster)
