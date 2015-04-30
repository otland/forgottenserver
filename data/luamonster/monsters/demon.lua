local mType = Game.createMonsterType("Demon")
local monster = {}
monster.description = "a demon"
monster.experience = 6000
monster.outfit = {
    lookType = 35
}

monster.health = 8200
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 5995
monster.speed = 280
monster.maxSummons = 1

monster.changeTarget = {
	interval = 4*1000,
	chance = 20
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
    convinceable = false,
	illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 70
}

monster.summons = {
	{name = "fire elemental", chance = 10, interval = 2*1000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Your soul will be mine!", yell = false},
	{text = "MUHAHAHAHA!", yell = false},
	{text = "CHAMEK ATH UTHUL ARAK!", yell = false},
	{text = "I SMELL FEEEEAAAAAR!", yell = false},
	{text = "Your resistance is futile!", yell = false}
}

monster.loot = {
	{id = "gold coin", chance = 60000, maxCount = 100},
	{id = "gold coin", chance = 60000, maxCount = 99},
	{id = "platinum coin", chance = 100000, maxCount = 6},
	{id = "fire mushroom", chance = 20740, maxCount = 6},
	{id = 8473, chance = 20000, maxCount = 3},
	{id = 7590, chance = 14285, maxCount = 3},
	{id = "double axe", chance = 14285},
	{id = "small emerald", chance = 10000},
	{id = "assassin star", chance = 5263, maxCount = 5},
	{id = "fire axe", chance = 3703},
	{id = "talon", chance = 3571},
	{id = "orb", chance = 2854},
	{id = "giant sword", chance = 2000},
	{id = "golden sickle", chance = 1428},
	{id = "stealth ring", chance = 1333},
	{id = "devil helmet", chance = 1204},
	{id = "purple tome", chance = 1190},
	{id = "gold ring", chance = 1010},
	{id = "platinum amulet", chance = 813},
	{id = "ice rapier", chance = 666},
	{id = "demon shield", chance = 649},
	{id = "ring of healing", chance = 473},
	{id = "demon horn", chance = 467},
	{id = "golden legs", chance = 413},
	{id = "mastermind shield", chance = 389},
	{id = "might ring", chance = 170},
	{id = "demon trophy", chance = 100},
	{id = "demonrage sword", chance = 80},
	{id = "magic plate armor", chance = 70}
}

monster.attacks = {
	{name = "melee", attack = 130, skill = 70, effect = CONST_ME_DRAWBLOOD, interval = 2*1000},
	{name = "energy strike", range = 1, chance = 10, interval = 2*1000, minDamage = -210, maxDamage = -300, target = true},
	{name = "combat", type = COMBAT_MANADRAIN, chance = 10, interval = 2*1000, minDamage = 0, maxDamage = -120, target = true, range = 7, effect = CONST_ME_MAGIC_BLUE},
	{name = "combat", type = COMBAT_FIREDAMAGE, chance = 20, interval = 2*1000, minDamage = -150, maxDamage = -250, radius = 1, target = true, effect = CONST_ME_FIREAREA, shootEffect = CONST_ANI_FIRE},
	{name = "speed", chance = 15, interval = 2*1000, speed = -700, radius = 1, target = true, duration = 30*1000, effect = CONST_ME_MAGIC_RED},
	{name = "firefield", chance = 10, interval = 2*1000, range = 7, radius = 1, target = true, shootEffect = CONST_ANI_FIRE},
	{name = "combat", type = COMBAT_LIFEDRAIN, chance = 10, interval = 2*1000, length = 8, spread = 0, minDamage = -300, maxDamage = -490, effect = CONST_ME_PURPLEENERGY}
}

monster.defenses = {
	defense = 55,
	armor = 55,
	{name = "combat", type = COMBAT_HEALING, chance = 15, interval = 2*1000, minDamage = 180, maxDamage = 250, effect = CONST_ME_MAGIC_BLUE},
	{name = "speed", chance = 15, interval = 2*1000, speed = 320, effect = CONST_ME_MAGIC_RED}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 30},
	{type = COMBAT_DEATHDAMAGE, percent = 30},
	{type = COMBAT_ENERGYDAMAGE, percent = 50},
	{type = COMBAT_EARTHDAMAGE, percent = 40},
	{type = COMBAT_ICEDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = -10}
}

monster.immunities = {
	{type = "fire", combat = true, condition = true},
	{type = "drown", condition = true},
	{type = "lifedrain", combat = true},
	{type = "paralyze", condition = true},
	{type = "invisible", condition = true}
}

mType:register(monster)








