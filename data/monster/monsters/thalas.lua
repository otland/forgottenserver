local mType = Game.createMonsterType("thalas")
local monster = {}

monster.name = "Thalas"
monster.description = "Thalas"
monster.experience = 2950
monster.outfit = {
	lookType = 90,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 4100
monster.maxHealth = 4100
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 6025
monster.speed = 320
monster.summonCost = 0
monster.maxSummons = 8

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
	{text = "You will become a feast for my maggots!", yell = false}
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
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE , percent = -20},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -900, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_EARTHDAMAGE, minDamage = -150, maxDamage = -650, range = 7, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_POISONAREA},
	{name ="melee", interval = 3000, chance = 20, minDamage = -150, maxDamage = -650, effect = CONST_ME_DRAWBLOOD},
	{name ="speed", interval = 1000, chance = 6, speed = {min = -800, max = -800}, duration = 20000, range = 7, effect = CONST_ME_MAGIC_RED},
	{name ="condition", interval = 1000, chance = 15, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -34, maxDamage = -35, radius = 5, target = false, effect = CONST_ME_POISONAREA},
	{name ="combat", interval = 3000, chance = 17, type = COMBAT_EARTHDAMAGE, minDamage = -55, maxDamage = -550, length = 8, spread = 3, effect = CONST_ME_POISONAREA}
}

monster.defenses = {
	defense = 40,
	armor = 40,
	{name ="combat", interval = 1000, chance = 20, type = COMBAT_HEALING, minDamage = 150, maxDamage = 450, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "Slime", chance = 100, interval = 2000}
}

monster.loot = {
	{id = "cobrafang dagger", chance = 100000},
	{id = "gold coin", chance = 30000, maxCount = 100},
	{id = "gold coin", chance = 30000, maxCount = 100},
	{id = "gold coin", chance = 30000, maxCount = 38},
	{id = "poison dagger", chance = 21000},
	{id = "small emerald", chance = 9000, maxCount = 3},
	{id = "great health potion", chance = 8200},
	{id = 2169, chance = 5200},
	{id = "serpent sword", chance = 2700},
	{id = "djinn blade", chance = 1000},
	{id = "green gem", chance = 100}
}

mType:register(monster)
