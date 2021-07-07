local mType = Game.createMonsterType("grim reaper")
local monster = {}

monster.name = "Grim Reaper"
monster.description = "a grim reaper"
monster.experience = 5500
monster.outfit = {
	lookType = 300,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 3900
monster.maxHealth = 3900
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 8955
monster.speed = 260
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 5000,
	chance = 20
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
	{text = "Death!", yell = false},
	{text = "Come a little closer!", yell = false},
	{text = "The end is near!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 20},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 40},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 65},
	{type = COMBAT_HOLYDAMAGE , percent = -10},
	{type = COMBAT_DEATHDAMAGE , percent = 80}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -320, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -165, range = 7, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_MORTAREA},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_DEATHDAMAGE, minDamage = -350, maxDamage = -720, length = 8, spread = 0, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -300, length = 7, spread = 3, effect = CONST_ME_EXPLOSIONAREA},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_DEATHDAMAGE, minDamage = -225, maxDamage = -275, radius = 4, target = false, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 35,
	armor = 35,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 130, maxDamage = 205, effect = CONST_ME_DRAWBLOOD},
	{name ="speed", interval = 2000, chance = 15, speed = {min = 450, max = 450}, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = "gold coin", chance = 33000, maxCount = 100},
	{id = "gold coin", chance = 33000, maxCount = 100},
	{id = "gold coin", chance = 33000, maxCount = 63},
	{id = "platinum coin", chance = 5200, maxCount = 4},
	{id = 2162, chance = 4850},
	{id = "dark shield", chance = 3000},
	{id = "scythe", chance = 9000},
	{id = "orichalcum pearl", chance = 1400, maxCount = 4},
	{id = 6300, chance = 330},
	{id = "demonic essence", chance = 10600},
	{id = "concentrated demonic blood", chance = 35000},
	{id = "nightmare blade", chance = 880},
	{id = "great mana potion", chance = 10000},
	{id = "glacier kilt", chance = 330},
	{id = "ultimate health potion", chance = 9600},
	{id = "skullcracker armor", chance = 270},
	{id = "underworld rod", chance = 910},
	{id = 9810, chance = 2500},
	{id = "mystical hourglass", chance = 5300}
}

mType:register(monster)
