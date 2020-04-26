local mType = Game.createMonsterType("orewalker")
local monster = {}

monster.name = "Orewalker"
monster.description = "an orewalker"
monster.experience = 4800
monster.outfit = {
	lookType = 490,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 7200
monster.maxHealth = 7200
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 17256
monster.speed = 250
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
	{text = "Sizzle!", yell = false},
	{text = "CLONK!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 5},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 65},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 5},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 15}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -300, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_MANADRAIN, minDamage = -296, maxDamage = -700, length = 4, spread = 3, effect = CONST_ME_MORTAREA},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -1500, length = 6, spread = 0, effect = CONST_ME_GROUNDSHAKER},
	{name ="condition", interval = 2000, chance = 10, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -800, maxDamage = -1080, radius = 3, target = true, ShootEffect = CONST_ANI_SMALLEARTH, effect = CONST_ME_SMALLPLANTS},
	{name ="drunk", interval = 2000, chance = 15, drunkenness = 25, duration = 6000, radius = 4, target = false, effect = CONST_ME_SOUND_PURPLE},
	{name ="speed", interval = 2000, chance = 15, speed = {min = -800, max = -800}, duration = 20000, radius = 2, target = false, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 45,
	armor = 45
}

monster.loot = {
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "gold coin", chance = 50000, maxCount = 98},
	{id = "platinum coin", chance = 100000, maxCount = 10},
	{id = "yellow gem", chance = 1030},
	{id = "dwarven ring", chance = 4660},
	{id = "knight legs", chance = 1910},
	{id = "crown armor", chance = 370},
	{id = "crown helmet", chance = 890},
	{id = "iron ore", chance = 15000},
	{id = "magic sulphur", chance = 3000},
	{id = "titan axe", chance = 2600},
	{id = "glorious axe", chance = 1870},
	{id = "strong health potion", chance = 15600, maxCount = 2},
	{id = "strong mana potion", chance = 14000, maxCount = 2},
	{id = "great mana potion", chance = 14000, maxCount = 2},
	{id = "mana potion", chance = 14000, maxCount = 4},
	{id = "ultimate health potion", chance = 9500, maxCount = 2},
	{id = "crystalline armor", chance = 560},
	{id = "small topaz", chance = 16500, maxCount = 3},
	{id = "shiny stone", chance = 13700},
	{id = "sulphurous stone", chance = 20700},
	{id = "wand of defiance", chance = 1300},
	{id = "green crystal shard", chance = 8000},
	{id = "blue crystal splinter", chance = 16000, maxCount = 2},
	{id = "cyan crystal fragment", chance = 13000},
	{id = "pulverized ore", chance = 20500},
	{id = "vein of ore", chance = 15000},
	{id = "prismatic bolt", chance = 15500, maxCount = 5},
	{id = "crystal crossbow", chance = 300}
}

mType:register(monster)
