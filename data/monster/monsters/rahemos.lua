local mType = Game.createMonsterType("rahemos")
local monster = {}

monster.name = "Rahemos"
monster.description = "Rahemos"
monster.experience = 3100
monster.outfit = {
	lookType = 87,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 3700
monster.maxHealth = 3700
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 6031
monster.speed = 320
monster.summonCost = 0
monster.maxSummons = 1

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
	{text = "It's a kind of magic.", yell = false},
	{text = "Abrah Kadabrah!", yell = false},
	{text = "Nothing hidden in my wrappings.", yell = false},
	{text = "It's not a trick, it's Rahemos.", yell = false},
	{text = "Meet my friend from hell!", yell = false},
	{text = "I will make you believe in magic.", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -40},
	{type = COMBAT_ENERGYDAMAGE, percent = 95},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 95},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = -1},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -750, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 65, interval = 4000}},
	{name ="combat", interval = 3000, chance = 7, type = COMBAT_LIFEDRAIN, minDamage = -75, maxDamage = -750, range = 1},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_ENERGYDAMAGE, minDamage = -60, maxDamage = -600, range = 7, ShootEffect = CONST_ANI_ENERGY, effect = CONST_ME_ENERGYHIT},
	{name ="combat", interval = 3000, chance = 20, type = COMBAT_PHYSICALDAMAGE, minDamage = -60, maxDamage = -600, range = 7, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_MORTAREA},
	{name ="speed", interval = 1000, chance = 12, speed = {min = -650, max = -650}, duration = 60000, radius = 6, target = false, effect = CONST_ME_POISONAREA},
	{name ="drunk", interval = 1000, chance = 8, drunkenness = 25, duration = 10000, range = 7, ShootEffect = CONST_ANI_ENERGY, effect = CONST_ME_ENERGYAREA},
	{name ="outfit", interval = 1000, chance = 15, monster = "pig", duration = 12000, range = 7, effect = CONST_ME_MAGIC_BLUE}
}

monster.defenses = {
	defense = 40,
	armor = 40,
	{name ="combat", interval = 1000, chance = 20, type = COMBAT_HEALING, minDamage = 200, maxDamage = 500, effect = CONST_ME_MAGIC_BLUE},
	{name ="outfit", interval = 1000, chance = 5, monster = "demon", duration = 4000, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "Demon", chance = 12, interval = 1000}
}

monster.loot = {
	{id = "ancient rune", chance = 100000},
	{id = "gold coin", chance = 30000, maxCount = 100},
	{id = "gold coin", chance = 30000, maxCount = 100},
	{id = "gold coin", chance = 30000, maxCount = 42},
	{id = "great mana potion", chance = 9000},
	{id = "small amethyst", chance = 8000, maxCount = 3},
	{id = "ring of healing", chance = 4000},
	{id = "magician hat", chance = 1600},
	{id = "violet gem", chance = 1100},
	{id = "orb", chance = 300},
	{id = "twin axe", chance = 200},
	{id = "crystal wand", chance = 150},
	{id = "mini mummy", chance = 150}
}

mType:register(monster)
