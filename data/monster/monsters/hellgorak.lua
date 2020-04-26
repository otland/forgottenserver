local mType = Game.createMonsterType("hellgorak")
local monster = {}

monster.name = "Hellgorak"
monster.description = "Hellgorak"
monster.experience = 10000
monster.outfit = {
	lookType = 12,
	lookHead = 0,
	lookBody = 77,
	lookLegs = 1,
	lookFeet = 3,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 25850
monster.maxHealth = 25850
monster.runHealth = 150
monster.race = "fire"
monster.corpse = 6068
monster.speed = 330
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 5000,
	chance = 28
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
	staticAttackChance = 85,
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
	{text = "I'll sacrifice yours souls to seven!", yell = false},
	{text = "I'm bad news for you mortals!", yell = false},
	{text = "No man can defeat me!", yell = false},
	{text = "Your puny skills are no match for me.", yell = false},
	{text = "I smell your fear.", yell = false},
	{text = "Delicious!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 98},
	{type = COMBAT_ENERGYDAMAGE, percent = 98},
	{type = COMBAT_EARTHDAMAGE, percent = 98},
	{type = COMBAT_FIREDAMAGE, percent = 98},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = -305},
	{type = COMBAT_ICEDAMAGE, percent = 98},
	{type = COMBAT_HOLYDAMAGE , percent = 95},
	{type = COMBAT_DEATHDAMAGE , percent = 98}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -910, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 1000, chance = 11, type = COMBAT_ENERGYDAMAGE, minDamage = -250, maxDamage = -850, length = 8, spread = 0, effect = CONST_ME_PURPLEENERGY},
	{name ="combat", interval = 2000, chance = 14, type = COMBAT_MANADRAIN, minDamage = -200, maxDamage = -400, radius = 5, target = false, effect = CONST_ME_STUN},
	{name ="combat", interval = 1000, chance = 11, type = COMBAT_FIREDAMAGE, minDamage = -50, maxDamage = -600, range = 7, radius = 5, target = true, effect = CONST_ME_FIREAREA},
	{name ="combat", interval = 2000, chance = 5, type = COMBAT_LIFEDRAIN, minDamage = 0, maxDamage = -150, radius = 7, target = false, effect = CONST_ME_POFF}
}

monster.defenses = {
	defense = 65,
	armor = 70,
	{name ="combat", interval = 2000, chance = 22, type = COMBAT_HEALING, minDamage = 400, maxDamage = 1700, effect = CONST_ME_MAGIC_GREEN}
}

monster.loot = {
	{id = "demonic essence", chance = 100000},
	{id = "gold coin", chance = 60000, maxCount = 100},
	{id = "gold coin", chance = 60000, maxCount = 98},
	{id = 9813, chance = 51000},
	{id = "ultimate health potion", chance = 42000, maxCount = 2},
	{id = 9810, chance = 32000},
	{id = "spellbook of warding", chance = 30000},
	{id = "beastslayer axe", chance = 29950},
	{id = "great spirit potion", chance = 21000},
	{id = "platinum coin", chance = 20000, maxCount = 30},
	{id = "crown armor", chance = 18000},
	{id = "great health potion", chance = 18000},
	{id = "great mana potion", chance = 17000},
	{id = "crystal necklace", chance = 14000},
	{id = "black pearl", chance = 14000, maxCount = 25},
	{id = "white pearl", chance = 13000, maxCount = 25},
	{id = "small ruby", chance = 13000, maxCount = 25},
	{id = "small diamond", chance = 12000, maxCount = 25},
	{id = "noble axe", chance = 12000},
	{id = "small amethyst", chance = 11000, maxCount = 25},
	{id = "small emerald", chance = 11000, maxCount = 25},
	{id = "small sapphire", chance = 11000, maxCount = 25},
	{id = "small topaz", chance = 11000, maxCount = 25},
	{id = "magma legs", chance = 11000},
	{id = "spirit cloak", chance = 10000},
	{id = "ruby necklace", chance = 10000},
	{id = "crown legs", chance = 10000},
	{id = "spellbook of mind control", chance = 10000},
	{id = "golden amulet", chance = 10000},
	{id = "stone skin amulet", chance = 10000},
	{id = "demon horn", chance = 9500, maxCount = 2},
	{id = "steel boots", chance = 9500},
	{id = "focus cape", chance = 9500},
	{id = "knight legs", chance = 9000},
	{id = "spellbook of lost souls", chance = 9000},
	{id = "blue robe", chance = 8170},
	{id = "butcher's axe", chance = 2320},
	{id = "golden armor", chance = 2870},
	{id = "vile axe", chance = 1970},
	{id = "spellscroll of prophecies", chance = 1360},
	{id = "executioner", chance = 610},
	{id = 2136, chance = 590},
	{id = "demonwing axe", chance = 450},
	{id = "golden legs", chance = 450},
	{id = "voltage armor", chance = 450},
	{id = "spellbook of dark mysteries", chance = 300},
	{id = "great axe", chance = 100}
}

mType:register(monster)
