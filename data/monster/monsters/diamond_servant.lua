local mType = Game.createMonsterType("diamond servant")
local monster = {}

monster.name = "Diamond Servant"
monster.description = "a diamond servant"
monster.experience = 700
monster.outfit = {
	lookType = 397,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1000
monster.maxHealth = 1000
monster.runHealth = 100
monster.race = "venom"
monster.corpse = 13485
monster.speed = 210
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
	{text = "Error. LOAD 'PROGRAM',8,1", yell = false},
	{text = "Remain. Obedient.", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 100},
	{type = COMBAT_EARTHDAMAGE, percent = 80},
	{type = COMBAT_FIREDAMAGE, percent = 10},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = -5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -100, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_ENERGYDAMAGE, minDamage = -80, maxDamage = -120, radius = 3, target = false, effect = CONST_ME_YELLOWENERGY},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_ENERGYDAMAGE, minDamage = -125, maxDamage = -170, length = 5, spread = 2, ShootEffect = CONST_ANI_ENERGY, effect = CONST_ME_ENERGYHIT},
	{name ="drunk", interval = 2000, chance = 10, drunkenness = 25, duration = 3000, range = 7, ShootEffect = CONST_ANI_DEATH, effect = CONST_ME_STUN}
}

monster.defenses = {
	defense = 25,
	armor = 25
}

monster.loot = {
	{id = "gold coin", chance = 44000, maxCount = 100},
	{id = "gold coin", chance = 50000, maxCount = 79},
	{id = "yellow gem", chance = 550},
	{id = "might ring", chance = 940},
	{id = "life crystal", chance = 9150},
	{id = "wand of cosmic energy", chance = 530},
	{id = "soul orb", chance = 45000},
	{id = "bonebreaker", chance = 10},
	{id = "mastermind potion", chance = 400},
	{id = "strong health potion", chance = 5790},
	{id = "strong mana potion", chance = 5980},
	{id = "lightning pendant", chance = 710},
	{id = "crystalline armor", chance = 20},
	{id = "gear wheel", chance = 5000},
	{id = 9976, chance = 5320},
	{id = "shockwave amulet", chance = 110},
	{id = "gear crystal", chance = 5000},
	{id = "slime mould", chance = 480}
}

mType:register(monster)
