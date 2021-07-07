local mType = Game.createMonsterType("demodras")
local monster = {}

monster.name = "Demodras"
monster.description = "Demodras"
monster.experience = 6000
monster.outfit = {
	lookType = 204,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 4500
monster.maxHealth = 4500
monster.runHealth = 300
monster.race = "blood"
monster.corpse = 5984
monster.speed = 230
monster.summonCost = 0
monster.maxSummons = 2

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
	{text = "I WILL SET THE WORLD ON FIRE!", yell = true},
	{text = "I WILL PROTECT MY BROOD!", yell = true}
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
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -235, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 3000, chance = 20, type = COMBAT_FIREDAMAGE, minDamage = -350, maxDamage = -400, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_FIREAREA},
	{name ="firefield", interval = 1000, chance = 10, range = 7, radius = 6, target = true, ShootEffect = CONST_ANI_FIRE},
	{name ="combat", interval = 4000, chance = 20, type = COMBAT_FIREDAMAGE, minDamage = -300, maxDamage = -550, length = 8, spread = 3, effect = CONST_ME_FIREAREA}
}

monster.defenses = {
	defense = 45,
	armor = 45,
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_HEALING, minDamage = 400, maxDamage = 600, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "Dragon", chance = 17, interval = 1000}
}

monster.loot = {
	{id = 5919, chance = 100000},
	{id = "platinum coin", chance = 99000, maxCount = 10},
	{id = "dragon ham", chance = 75000, maxCount = 10},
	{id = "green mushroom", chance = 25666, maxCount = 7},
	{id = "power bolt", chance = 19000, maxCount = 10},
	{id = "red dragon leather", chance = 15000},
	{id = "small sapphire", chance = 12222},
	{id = 1976, chance = 10000},
	{id = "energy ring", chance = 10000},
	{id = "great mana potion", chance = 8800},
	{id = "great health potion", chance = 8800},
	{id = "golden mug", chance = 6100},
	{id = "onyx arrow", chance = 4500, maxCount = 5},
	{id = "dragon scale mail", chance = 2000},
	{id = "fire sword", chance = 2000},
	{id = "life crystal", chance = 1000}
}

mType:register(monster)
