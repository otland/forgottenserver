local mType = Game.createMonsterType("behemoth")
local monster = {}

monster.name = "Behemoth"
monster.description = "a behemoth"
monster.experience = 2500
monster.outfit = {
	lookType = 55,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 4000
monster.maxHealth = 4000
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 5999
monster.speed = 340
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
	staticAttackChance = 70,
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
	{text = "You're so little!", yell = false},
	{text = "Human flesh - delicious!", yell = false},
	{text = "Crush the intruders!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = 80},
	{type = COMBAT_FIREDAMAGE, percent = 30},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE , percent = 30},
	{type = COMBAT_DEATHDAMAGE , percent = -5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -455, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -200, range = 7, ShootEffect = CONST_ANI_LARGEROCK}
}

monster.defenses = {
	defense = 50,
	armor = 50,
	{name ="speed", interval = 2000, chance = 15, speed = {min = 390, max = 390}, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = "gold coin", chance = 59000, maxCount = 100},
	{id = "gold coin", chance = 40000, maxCount = 100},
	{id = "platinum coin", chance = 60000, maxCount = 5},
	{id = "meat", chance = 30000, maxCount = 6},
	{id = "battle stone", chance = 14000},
	{id = "assassin star", chance = 10000, maxCount = 5},
	{id = "double axe", chance = 9900},
	{id = "small amethyst", chance = 6380, maxCount = 5},
	{id = "two handed sword", chance = 5980},
	{id = "great health potion", chance = 5120},
	{id = "dark armor", chance = 4370},
	{id = "plate armor", chance = 3930},
	{id = "crystal necklace", chance = 2530},
	{id = "perfect behemoth fang", chance = 1090},
	{id = "giant sword", chance = 1006},
	{id = "strange symbol", chance = 820},
	{id = 2231, chance = 720},
	{id = "pick", chance = 650},
	{id = "behemoth claw", chance = 460},
	{id = "steel boots", chance = 440},
	{id = "crowbar", chance = 130},
	{id = 7396, chance = 170},
	{id = 2023, chance = 90},
	{id = "titan axe", chance = 70},
	{id = "war axe", chance = 60}
}

mType:register(monster)
