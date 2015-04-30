local mType = Game.createMonsterType("Morguthis")
local monster = {}
monster.description = "a morguthis"
monster.experience = 3000
monster.outfit = {
	lookType = 90
}

monster.health = 4800
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6025
monster.speed = 320
monster.maxSummons = 3

monster.changeTarget = {
    interval = 5000,
	chance = 8
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
	staticAttack = 90
}

monster.summons = {
    {name = "Hero", chance = 1000, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Vengeance!", yell = false},
	{text = "You will make a fine trophy.", yell = false},
	{text = "Come and fight me, cowards!", yell = false},
	{text = "I am the supreme warrior!", yell = false},
	{text = "Let me hear the music of battle.", yell = false},
	{text = "Anotherone to bite the dust!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 80, chance = 50000},
	{id = "gold coin", maxCount = 73, chance = 50000},
	{id = "stone skin amulet", subType = 5, chance = 7000},
	{id = "knight axe", chance = 7000},
	{id = "black pearl", chance = 7000},
	{id = "great health potion", chance = 1500},
	{id = "demonbone amulet", chance = 500},
	{id = "ravager's axe", chance = 300},
	{id = "assassin star", maxCount = 3, chance = 500},
	{id = "steel boots", chance = 500},
	{id = "sword hilt", chance = 100000}
}

monster.attacks = {
	{name = "melee",  attack = 200, skill = 90, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 7, interval = 3000, minDamage = -55, maxDamage = -550, range = 1},
	{name = "speed",  chance = 25, interval = 1000, minDamage = 0, maxDamage = 0, duration = 50000, speedChange = -650, range = 7, effect = 14},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 1000, minDamage = -40, maxDamage = -400, radius = 3, effect = 10},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 7, interval = 3000, minDamage = -50, maxDamage = -500, radius = 3, effect = 18}
}

monster.defenses = {
	defense = 25,
    armor = 35,
    {name = "combat", type = COMBAT_HEALING,  chance = 13, interval = 1000, minDamage = 200, maxDamage = 300, radius = 3, effect = 13},
	{name = "speed",  chance = 7, interval = 1000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 1201, radius = 3, effect = 14},
	{name = "invisible",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 52},
	{type = COMBAT_EARTHDAMAGE, percent = -15},
	{type = COMBAT_FIREDAMAGE, percent = 60},
	{type = COMBAT_HOLYDAMAGE, percent = -22},
	{type = COMBAT_ICEDAMAGE, percent = 62}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)