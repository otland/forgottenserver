local mType = Game.createMonsterType("hideous fungus")
local monster = {}

monster.name = "Hideous Fungus"
monster.description = "a hideous fungus"
monster.experience = 2900
monster.outfit = {
	lookType = 499,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 4600
monster.maxHealth = 4600
monster.runHealth = 275
monster.race = "venom"
monster.corpse = 17428
monster.speed = 260
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
	{text = "Munch munch munch!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 15},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 5},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 15},
	{type = COMBAT_HOLYDAMAGE , percent = 5},
	{type = COMBAT_DEATHDAMAGE , percent = 1}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -450, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_EARTHDAMAGE, minDamage = -250, maxDamage = -430, range = 7, ShootEffect = CONST_ANI_SMALLEARTH, effect = CONST_ME_SMALLPLANTS},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_ICEDAMAGE, minDamage = -250, maxDamage = -550, length = 8, spread = 3, ShootEffect = CONST_ANI_SNOWBALL},
	{name ="speed", interval = 2000, chance = 10, speed = {min = -600, max = -600}, duration = 60000, radius = 1, target = true, effect = CONST_ME_MAGIC_RED},
	{name ="drunk", interval = 2000, chance = 10, drunkenness = 25, duration = 4000, range = 7, radius = 5, target = true, ShootEffect = CONST_ANI_SMALLSTONE, effect = CONST_ME_STUN},
	{name ="condition", interval = 2000, chance = 10, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -400, maxDamage = -640, range = 7, radius = 3, target = false, effect = CONST_ME_HITBYPOISON}
}

monster.defenses = {
	defense = 35,
	armor = 35,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 275, maxDamage = 350, effect = CONST_ME_MAGIC_BLUE},
	{name ="invisible", interval = 2000, chance = 10, duration = 6000, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "humorless fungus", chance = 10, interval = 2000, max = 2}
}

monster.loot = {
	{id = "gold coin", chance = 70000, maxCount = 100},
	{id = "gold coin", chance = 70000, maxCount = 97},
	{id = "platinum coin", chance = 100000, maxCount = 7},
	{id = "war hammer", chance = 4010},
	{id = "white piece of cloth", chance = 6430},
	{id = "green piece of cloth", chance = 3003},
	{id = "red piece of cloth", chance = 1820},
	{id = "blue piece of cloth", chance = 3700},
	{id = "great mana potion", chance = 7692},
	{id = "great health potion", chance = 7220},
	{id = "mana potion", chance = 9090},
	{id = "terra mantle", chance = 800},
	{id = "terra legs", chance = 920},
	{id = "terra boots", chance = 2000},
	{id = "terra amulet", chance = 2000},
	{id = "mushroom backpack", chance = 20},
	{id = "mushroom pie", chance = 14500, maxCount = 3},
	{id = "muck rod", chance = 590},
	{id = "hideous chunk", chance = 14285},
	{id = "envenomed arrow", chance = 14500, maxCount = 15},
	{id = "mycological bow", chance = 60}
}

mType:register(monster)
