local mType = Game.createMonsterType("medusa")
local monster = {}

monster.name = "Medusa"
monster.description = "a medusa"
monster.experience = 4050
monster.outfit = {
	lookType = 330,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 4500
monster.maxHealth = 4500
monster.runHealth = 600
monster.race = "blood"
monster.corpse = 10524
monster.speed = 280
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
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
	{text = "You will make ssuch a fine ssstatue!", yell = false},
	{text = "There isss no chhhanccce of esscape", yell = false},
	{text = "Jusssst look at me!", yell = false},
	{text = "Are you tired or why are you moving thhat sslow<chuckle>", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = -5},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -450, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 840, interval = 4000}},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_LIFEDRAIN, minDamage = -75, maxDamage = -150, range = 7, ShootEffect = CONST_ANI_EARTH, effect = CONST_ME_CARNIPHILA},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_LIFEDRAIN, minDamage = -75, maxDamage = -150, range = 7, ShootEffect = CONST_ANI_EARTH, effect = CONST_ME_CARNIPHILA},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_LIFEDRAIN, minDamage = -75, maxDamage = -150, range = 7, ShootEffect = CONST_ANI_EARTH, effect = CONST_ME_CARNIPHILA},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_LIFEDRAIN, minDamage = -75, maxDamage = -150, range = 7, ShootEffect = CONST_ANI_EARTH, effect = CONST_ME_CARNIPHILA},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_LIFEDRAIN, minDamage = -75, maxDamage = -150, range = 7, ShootEffect = CONST_ANI_EARTH, effect = CONST_ME_CARNIPHILA},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_EARTHDAMAGE, minDamage = -250, maxDamage = -500, length = 8, spread = 3, effect = CONST_ME_CARNIPHILA},
	{name ="speed", interval = 2000, chance = 25, speed = {min = -900, max = -900}, duration = 10000, radius = 7, target = true, effect = CONST_ME_POFF},
	{name ="outfit", interval = 2000, chance = 1, monster = "clay guardian", duration = 3000, range = 7}
}

monster.defenses = {
	defense = 30,
	armor = 30,
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_HEALING, minDamage = 150, maxDamage = 300, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "gold coin", chance = 50000, maxCount = 90},
	{id = "small emerald", chance = 3770, maxCount = 4},
	{id = "platinum coin", chance = 74810, maxCount = 6},
	{id = "knight armor", chance = 1840},
	{id = "medusa shield", chance = 3040},
	{id = "titan axe", chance = 1160},
	{id = "great mana potion", chance = 10000, maxCount = 2},
	{id = "terra mantle", chance = 870},
	{id = "terra legs", chance = 420},
	{id = "terra amulet", chance = 4060},
	{id = "ultimate health potion", chance = 9290, maxCount = 2},
	{id = 9810, chance = 500},
	{id = "sacred tree amulet", chance = 850},
	{id = "strand of medusa hair", chance = 9900}
}

mType:register(monster)
