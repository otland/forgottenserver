local mType = Game.createMonsterType("chizzoron the distorter")
local monster = {}

monster.name = "Chizzoron The Distorter"
monster.description = "Chizzoron the Distorter"
monster.experience = 4000
monster.outfit = {
	lookType = 340,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 16000
monster.maxHealth = 16000
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 11316
monster.speed = 260
monster.summonCost = 0
monster.maxSummons = 2

monster.changeTarget = {
	interval = 2000,
	chance = 10
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
	{text = "Humanzzz! Leave Zzaion at onzzzze!", yell = false},
	{text = "I pray to my mazzterzz, the mighty dragonzzz!", yell = false},
	{text = "You are not worzzy to touch zzizz zzacred ground!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = 10},
	{type = COMBAT_DEATHDAMAGE , percent = 30}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -455, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_EARTHDAMAGE, minDamage = 0, maxDamage = -430, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_POISONAREA},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_FIREDAMAGE, minDamage = 0, maxDamage = -874, length = 8, spread = 3, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_FIREAREA},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_PHYSICALDAMAGE, minDamage = -300, maxDamage = -646, radius = 3, target = false, effect = CONST_ME_POFF},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_LIFEDRAIN, minDamage = -148, maxDamage = -250, range = 7, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 85,
	armor = 70
}

monster.summons = {
	{name = "Lizard Dragon Priest", chance = 10, interval = 2000, max = 2}
}

monster.loot = {
	{id = "gold coin", chance = 69825, maxCount = 100},
	{id = "gold coin", chance = 69825, maxCount = 10},
	{id = "small emerald", chance = 5750},
	{id = "green gem", chance = 16300},
	{id = 2169, chance = 11025},
	{id = "dragon scale mail", chance = 5750},
	{id = "lizard scale", chance = 100000},
	{id = "great health potion", chance = 5750},
	{id = "gold ingot", chance = 71550, maxCount = 2}
}

mType:register(monster)
