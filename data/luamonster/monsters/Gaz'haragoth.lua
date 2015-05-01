local mType = Game.createMonsterType("Gaz'haragoth")
local monster = {}
monster.description = "gaz'haragoth"
monster.experience = 150000
monster.outfit = {
	lookFeet = 79,
	lookLegs = 79,
	lookType = 591,
	lookBody = 94
}

monster.health = 350000
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 22562
monster.speed = 400
monster.maxSummons = 3

monster.changeTarget = {
    interval = 10000,
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
	staticAttack = 98
}

monster.summons = {
    {name = "Minion of Gaz'haragoth", chance = 33, interval = 4000}
}

monster.voices = {
	interval = 5000,
	chance = 30,
	{text = "I AM MORGAROTH, LORD OF THE TRIANGLE... AND YOU ARE LOST!", yell = true},
	{text = "MY SEED IS FEAR AND MY HARVEST ARE YOUR SOULS!", yell = true},
	{text = "THE TRIANGLE OF TERROR WILL RISE!", yell = true},
	{text = "ZATHROTH! LOOK AT THE DESTRUCTION I AM CAUSING IN YOUR NAME!", yell = true}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "gold coin", maxCount = 90, chance = 50000},
	{id = "white pearl", maxCount = 5, chance = 10000},
	{id = "ice rapier", subType = 1, chance = 19429},
	{id = "devil helmet", chance = 20000},
	{id = "ring of healing", chance = 16000},
	{id = "demon legs", chance = 6000},
	{id = "double axe", chance = 20000},
	{id = "giant sword", chance = 60000},
	{id = "boots of haste", chance = 16033},
	{id = "thunder hammer", chance = 7000},
	{id = "great shield", chance = 5600},
	{id = "crystal ball", chance = 24000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = -700, maxDamage = -2000, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 35, interval = 3000, minDamage = -700, maxDamage = -1200, target = true, range = 7, radius = 7, shootEffect = 4, effect = 7},
	{name = "melee",  attack = 0, skill = 0, minDamage = -250, maxDamage = -400, interval = 1800, effect = 10},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 30, interval = 3000, minDamage = -300, maxDamage = -1000, length = 8, shootEffect = 4, effect = 12},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2500, minDamage = -200, maxDamage = -480, range = 7, radius = 5, shootEffect = 4, effect = 15},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = -250, maxDamage = -500, range = 7, radius = 13, shootEffect = 4, effect = 20},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = -200, maxDamage = -450, radius = 14, shootEffect = 4, effect = 2},
	{name = "melee",  attack = 0, skill = 0, minDamage = -100, maxDamage = -200, interval = 3000, effect = 13}
}

monster.defenses = {
	defense = 65,
    armor = 55,
    {name = "combat", type = COMBAT_HEALING,  chance = 35, interval = 3000, minDamage = 300, maxDamage = 800, shootEffect = 4, effect = 13},
	{name = "speed",  chance = 80, interval = 4000, minDamage = 0, maxDamage = 0, duration = 6000, speedChange = 440, shootEffect = 4, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 40},
	{type = COMBAT_DEATHDAMAGE, percent = 60},
	{type = COMBAT_ICEDAMAGE, percent = -25}
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
	{type = "energy", condition = true, combat = true},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)