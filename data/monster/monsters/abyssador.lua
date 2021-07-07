local mType = Game.createMonsterType("abyssador")
local monster = {}

monster.name = "Abyssador"
monster.description = "Abyssador"
monster.experience = 50000
monster.outfit = {
	lookType = 495,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 300000
monster.maxHealth = 300000
monster.runHealth = 100
monster.race = "blood"
monster.corpse = 17413
monster.speed = 470
monster.summonCost = 0
monster.maxSummons = 0

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
	staticAttackChance = 50,
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
	{text = "*** BRAINS *** SMALL ***", yell = false},
	{text = "*** LIVE *** DRY ***", yell = false},
	{text = "*** IMPORTANT ***", yell = false},
	{text = "*** FIRE *** HOME *** VICTORY ***", yell = false},
	{text = "*** EXISTENCE *** FUTILE ***", yell = false},
	{text = "*** TIME ***", yell = false},
	{text = "*** STEALTH ***", yell = false},
	{text = "*** DEATH ***", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = -20},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = -20}
}

monster.attacks = {
	{name ="melee", interval = 2500, chance = 100, minDamage = 0, maxDamage = -960, effect = CONST_ME_DRAWBLOOD},
	{name ="speed", interval = 2000, chance = 20, speed = {min = -400, max = -400}, duration = 2500, range = 7, ShootEffect = CONST_ANI_EARTH},
	{name ="combat", interval = 800, chance = 10, type = COMBAT_FIREDAMAGE, minDamage = -820, maxDamage = -1250, range = 7, radius = 10, target = false, effect = CONST_ME_HITBYPOISON},
	{name ="combat", interval = 750, chance = 10, type = COMBAT_ENERGYDAMAGE, minDamage = -650, maxDamage = -1300, range = 3, radius = 12, target = true, effect = CONST_ME_HITBYPOISON},
	{name ="combat", interval = 700, chance = 10, type = COMBAT_LIFEDRAIN, minDamage = -600, maxDamage = -1230, range = 3, radius = 4, target = true, ShootEffect = CONST_ANI_POISON},
	{name ="combat", interval = 700, chance = 12, type = COMBAT_EARTHDAMAGE, minDamage = -40, maxDamage = -130, radius = 3, target = false, effect = CONST_ME_POISONAREA}
}

monster.defenses = {
	defense = 20,
	armor = 15,
	{name ="combat", interval = 1000, chance = 1, type = COMBAT_HEALING, minDamage = 0, maxDamage = 300000, effect = CONST_ME_MAGIC_BLUE},
	{name ="combat", interval = 5000, chance = 30, type = COMBAT_HEALING, minDamage = 1000, maxDamage = 1000, effect = CONST_ME_MAGIC_BLUE},
	{name ="invisible", interval = 2000, chance = 25, duration = 3000, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "decorative ribbon", chance = 10000},
	{id = "crystalline sword", chance = 5500},
	{id = "crystalline axe", chance = 5500},
	{id = "mycological mace", chance = 4500},
	{id = "crystal crossbow", chance = 4500},
	{id = "mycological bow", chance = 10000},
	{id = "shiny blade", chance = 8000},
	{id = "Abyssador's lash", chance = 100000}
}

mType:register(monster)
