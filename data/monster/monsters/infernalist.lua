local mType = Game.createMonsterType("infernalist")
local monster = {}

monster.name = "Infernalist"
monster.description = "an infernalist"
monster.experience = 4000
monster.outfit = {
	lookType = 130,
	lookHead = 78,
	lookBody = 76,
	lookLegs = 94,
	lookFeet = 39,
	lookAddons = 2,
	lookMount = 0
}

monster.health = 3650
monster.maxHealth = 3650
monster.runHealth = 900
monster.race = "blood"
monster.corpse = 20427
monster.speed = 220
monster.summonCost = 0
monster.maxSummons = 1

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
	staticAttackChance = 95,
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
	{text = "Nothing will remain but your scorched bones!", yell = false},
	{text = "Some like it hot!", yell = false},
	{text = "It's cooking time!", yell = false},
	{text = "Feel the heat of battle!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = 100},
	{type = COMBAT_EARTHDAMAGE, percent = 95},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE , percent = 20},
	{type = COMBAT_DEATHDAMAGE , percent = 5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -100, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 40, type = COMBAT_FIREDAMAGE, minDamage = -65, maxDamage = -180, range = 7, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_HITBYFIRE},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_FIREDAMAGE, minDamage = -90, maxDamage = -180, range = 7, radius = 3, target = true, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_FIREAREA},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_MANADRAIN, minDamage = -53, maxDamage = -120, range = 7, radius = 3, target = true, ShootEffect = CONST_ANI_ENERGYBALL, effect = CONST_ME_TELEPORT},
	{name ="firefield", interval = 2000, chance = 15, range = 7, radius = 3, target = true, ShootEffect = CONST_ANI_FIRE},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_FIREDAMAGE, minDamage = -150, maxDamage = -250, length = 8, spread = 0, effect = CONST_ME_FIREATTACK},
	{name ="combat", interval = 2000, chance = 5, type = COMBAT_PHYSICALDAMAGE, minDamage = -100, maxDamage = -150, radius = 2, target = false, effect = CONST_ME_EXPLOSIONAREA}
}

monster.defenses = {
	defense = 15,
	armor = 15,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 60, maxDamage = 230, effect = CONST_ME_MAGIC_BLUE},
	{name ="invisible", interval = 2000, chance = 15, duration = 8000, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "fire elemental", chance = 20, interval = 2000}
}

monster.loot = {
	{id = "red tome", chance = 300},
	{id = "piggy bank", chance = 220},
	{id = "gold coin", chance = 56500, maxCount = 100},
	{id = "gold coin", chance = 40000, maxCount = 47},
	{id = "energy ring", chance = 1800},
	{id = "skull staff", chance = 6500},
	{id = "magic sulphur", chance = 600},
	{id = "red piece of cloth", chance = 1420},
	{id = "great mana potion", chance = 19700},
	{id = "great health potion", chance = 1900},
	{id = "small enchanted ruby", chance = 4250},
	{id = "magma boots", chance = 300},
	{id = "raspberry", chance = 8500, maxCount = 5},
	{id = "spellbook of mind control", chance = 370},
	{id = "royal tapestry", chance = 520},
	{id = "black skull", chance = 820},
	{id = "gold ingot", chance = 70},
	{id = "crystal of power", chance = 220}
}

mType:register(monster)
