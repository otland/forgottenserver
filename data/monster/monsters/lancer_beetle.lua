local mType = Game.createMonsterType("lancer beetle")
local monster = {}

monster.name = "Lancer Beetle"
monster.description = "a lancer beetle"
monster.experience = 275
monster.outfit = {
	lookType = 348,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 400
monster.maxHealth = 400
monster.runHealth = 30
monster.race = "venom"
monster.corpse = 11375
monster.speed = 200
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
	canPushCreatures = false,
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
	{text = "Crump!", yell = true}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 50}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -115, effect = CONST_ME_DRAWBLOOD},
	{name ="poisonfield", interval = 2000, chance = 10, radius = 4, target = false, effect = CONST_ME_POISONAREA},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_LIFEDRAIN, minDamage = 0, maxDamage = -90, length = 7, spread = 0, effect = CONST_ME_HITBYPOISON},
	{name ="condition", interval = 2000, chance = 10, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -40, maxDamage = -80, range = 7, ShootEffect = CONST_ANI_POISON},
	{name ="lancer beetle curse", interval = 2000, chance = 5, range = 5}
}

monster.defenses = {
	defense = 20,
	armor = 20,
	{name ="invisible", interval = 2000, chance = 10, duration = 3000, effect = CONST_ME_GROUNDSHAKER}
}

monster.loot = {
	{id = "gold coin", chance = 60000, maxCount = 61},
	{id = "gold coin", chance = 60000, maxCount = 79},
	{id = "small amethyst", chance = 247},
	{id = "poisonous slime", chance = 8333},
	{id = "lump of dirt", chance = 4166},
	{id = "lancer beetle shell", chance = 16666},
	{id = "beetle necklace", chance = 1123}
}

mType:register(monster)
