local mType = Game.createMonsterType("demon outcast")
local monster = {}

monster.name = "Demon Outcast"
monster.description = "a demon outcast"
monster.experience = 6200
monster.outfit = {
	lookType = 590,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 6900
monster.maxHealth = 6900
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 22549
monster.speed = 280
monster.summonCost = 0
monster.maxSummons = 2

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
	{text = "Back in the evil business!", yell = false},
	{text = "This prison break will have casualties!", yell = false},
	{text = "At last someone to hurt", yell = false},
	{text = "No one will imprison me again!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 20},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 40},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 5},
	{type = COMBAT_HOLYDAMAGE , percent = -5},
	{type = COMBAT_DEATHDAMAGE , percent = 5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -400, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_ENERGYDAMAGE, minDamage = -250, maxDamage = -450, length = 6, spread = 0, effect = CONST_ME_PURPLEENERGY},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_ENERGYDAMAGE, minDamage = -350, maxDamage = -550, length = 8, spread = 0, effect = CONST_ME_YELLOWENERGY},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_ENERGYDAMAGE, minDamage = -100, maxDamage = -250, radius = 3, target = true, effect = CONST_ME_ENERGYHIT},
	{name ="demon outcast skill reducer", interval = 2000, chance = 10, range = 5},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_MANADRAIN, minDamage = -80, maxDamage = -150, radius = 4, target = false, effect = CONST_ME_MAGIC_GREEN}
}

monster.defenses = {
	defense = 40,
	armor = 40,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 250, maxDamage = 425, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "energy elemental", chance = 10, interval = 2000}
}

monster.loot = {
	{id = "small diamond", chance = 10000, maxCount = 5},
	{id = "small sapphire", chance = 10000, maxCount = 5},
	{id = "small ruby", chance = 12000, maxCount = 5},
	{id = "gold coin", chance = 100000, maxCount = 100},
	{id = "small emerald", chance = 10000, maxCount = 5},
	{id = "platinum coin", chance = 100000, maxCount = 6},
	{id = "might ring", chance = 910},
	{id = "stealth ring", chance = 1300},
	{id = "platinum amulet", chance = 1000},
	{id = "ring of healing", chance = 3000},
	{id = "giant sword", chance = 2000},
	{id = "ice rapier", chance = 660},
	{id = "devil helmet", chance = 910},
	{id = "crusader helmet", chance = 740},
	{id = "crown shield", chance = 740},
	{id = "demon shield", chance = 170},
	{id = "fire mushroom", chance = 20600, maxCount = 6},
	{id = "red powder", chance = 1000},
	{id = "assassin star", chance = 8340, maxCount = 10},
	{id = "demonrage sword", chance = 350},
	{id = "great mana potion", chance = 18000, maxCount = 2},
	{id = "ultimate health potion", chance = 20500, maxCount = 3},
	{id = "small topaz", chance = 9300, maxCount = 5},
	{id = "cluster of solace", chance = 410}
}

mType:register(monster)
