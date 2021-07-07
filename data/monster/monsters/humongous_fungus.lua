local mType = Game.createMonsterType("humongous fungus")
local monster = {}

monster.name = "Humongous Fungus"
monster.description = "a humongous fungus"
monster.experience = 2600
monster.outfit = {
	lookType = 488,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 3400
monster.maxHealth = 3400
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 18382
monster.speed = 270
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 15
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
	staticAttackChance = 80,
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
	{type = COMBAT_DEATHDAMAGE , percent = 15}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -330, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_EARTHDAMAGE, minDamage = -180, maxDamage = -350, range = 7, ShootEffect = CONST_ANI_SMALLEARTH, effect = CONST_ME_SMALLPLANTS},
	{name ="poisonfield", interval = 2000, chance = 20, radius = 4, target = false},
	{name ="condition", interval = 2000, chance = 10, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -500, maxDamage = -1000, length = 8, spread = 0, effect = CONST_ME_GREEN_RINGS},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_LIFEDRAIN, minDamage = -130, maxDamage = -260, length = 5, spread = 0, effect = CONST_ME_MAGIC_RED},
	{name ="condition", interval = 2000, chance = 10, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -400, maxDamage = -640, range = 7, radius = 3, target = false, effect = CONST_ME_HITBYPOISON}
}

monster.defenses = {
	defense = 45,
	armor = 45,
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_HEALING, minDamage = 225, maxDamage = 380, effect = CONST_ME_MAGIC_BLUE},
	{name ="invisible", interval = 2000, chance = 15, duration = 4000, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "gold coin", chance = 60000, maxCount = 100},
	{id = "gold coin", chance = 60000, maxCount = 100},
	{id = "platinum coin", chance = 100000, maxCount = 6},
	{id = "white piece of cloth", chance = 10000},
	{id = "red piece of cloth", chance = 2941},
	{id = "blue piece of cloth", chance = 4545},
	{id = "brown piece of cloth", chance = 15285},
	{id = "angelic axe", chance = 1265},
	{id = "strong health potion", chance = 5000, maxCount = 2},
	{id = "strong mana potion", chance = 5000, maxCount = 2},
	{id = "great mana potion", chance = 5000, maxCount = 2},
	{id = "great health potion", chance = 5000, maxCount = 2},
	{id = "mana potion", chance = 5000, maxCount = 3},
	{id = "terra mantle", chance = 869},
	{id = "terra legs", chance = 1123},
	{id = "terra boots", chance = 1851},
	{id = "terra amulet", chance = 2127},
	{id = "mushroom backpack", chance = 20},
	{id = "mushroom pie", chance = 16666, maxCount = 3},
	{id = "muck rod", chance = 680},
	{id = "humongous chunk", chance = 10000},
	{id = "drill bolt", chance = 14285, maxCount = 15},
	{id = "mycological bow", chance = 150}
}

mType:register(monster)
