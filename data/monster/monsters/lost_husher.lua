local mType = Game.createMonsterType("lost husher")
local monster = {}

monster.name = "Lost Husher"
monster.description = "a lost husher"
monster.experience = 1800
monster.outfit = {
	lookType = 537,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1600
monster.maxHealth = 1600
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 19964
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
	{text = "Arr far zwar!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 5},
	{type = COMBAT_ENERGYDAMAGE, percent = 15},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 40},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 5},
	{type = COMBAT_HOLYDAMAGE , percent = -5},
	{type = COMBAT_DEATHDAMAGE , percent = 5}
}

monster.attacks = {
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_DEATHDAMAGE, minDamage = -150, maxDamage = -300, length = 6, spread = 0, effect = CONST_ME_BLACKSMOKE},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_MANADRAIN, minDamage = -150, maxDamage = -250, radius = 5, target = false, effect = CONST_ME_BLACKSMOKE},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_DEATHDAMAGE, minDamage = -150, maxDamage = -200, range = 7, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_MORTAREA},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_EARTHDAMAGE, minDamage = -150, maxDamage = -250, range = 7, radius = 2, target = true, ShootEffect = CONST_ANI_SMALLEARTH, effect = CONST_ME_MAGIC_GREEN},
	{name ="drunk", interval = 2000, chance = 10, drunkenness = 25, duration = 6000, radius = 4, target = false, effect = CONST_ME_SOUND_RED}
}

monster.defenses = {
	defense = 25,
	armor = 25,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 75, maxDamage = 92, effect = CONST_ME_YELLOWENERGY},
	{name ="invisible", interval = 2000, chance = 15, duration = 5000, effect = CONST_ME_TELEPORT}
}

monster.loot = {
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "platinum coin", chance = 58670, maxCount = 2},
	{id = "dwarven ring", chance = 2870},
	{id = "fire axe", chance = 330},
	{id = "skull staff", chance = 280},
	{id = "guardian shield", chance = 830},
	{id = "tower shield", chance = 440},
	{id = "brown mushroom", chance = 14920, maxCount = 2},
	{id = "spiked squelcher", chance = 50},
	{id = "strong health potion", chance = 11050, maxCount = 3},
	{id = "great mana potion", chance = 10770, maxCount = 2},
	{id = "fiery knight axe", chance = 770},
	{id = "terra legs", chance = 50},
	{id = "terra boots", chance = 880},
	{id = "small topaz", chance = 9780},
	{id = "clay lump", chance = 500},
	{id = 13757, chance = 10930},
	{id = "buckle", chance = 8180},
	{id = "buckle", chance = 770},
	{id = "bonecarving knife", chance = 8510},
	{id = "leather harness", chance = 11330},
	{id = "wimp tooth chain", chance = 8950},
	{id = "lost husher's staff", chance = 9280},
	{id = "skull shatterer", chance = 12320},
	{id = "mad froth", chance = 15580},
	{id = "red hair dye", chance = 8510},
	{id = "basalt figurine", chance = 8560}
}

mType:register(monster)
