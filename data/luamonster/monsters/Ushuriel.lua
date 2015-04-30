local mType = Game.createMonsterType("Ushuriel")
local monster = {}
monster.description = "ushuriel"
monster.experience = 10500
monster.outfit = {
	lookFeet = 112,
	lookLegs = 19,
	lookType = 12,
	lookBody = 95
}

monster.health = 31500
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 6068
monster.speed = 350
monster.maxSummons = 0

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
	staticAttack = 85
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "You can't run or hide forever!", yell = false},
	{text = "I'm the executioner of the Seven!", yell = false},
	{text = "The final punishment awaits you!", yell = false},
	{text = "The judgement is guilty! The sentence is death!", yell = false}
}

monster.loot = {
    {id = "demonic essence", chance = 100000},
	{id = "brown mushroom", maxCount = 30, chance = 50000},
	{id = "gold coin", maxCount = 190, chance = 50000},
	{id = "iron ore", chance = 33333},
	{id = "hardened bone", maxCount = 20, chance = 25000},
	{id = "thaian sword", chance = 25000},
	{id = "great health potion", chance = 20000},
	{id = "mind stone", chance = 20000},
	{id = "great mana potion", chance = 20000},
	{id = "royal helmet", chance = 20000},
	{id = "ultimate health potion", chance = 20000},
	{id = "great spirit potion", chance = 20000},
	{id = "skull helmet", chance = 20000},
	{id = "platinum coin", maxCount = 26, chance = 20000},
	{id = "rusty armor", chance = 20000},
	{id = "warrior helmet", chance = 20000},
	{id = "gold ingot", chance = 16666},
	{id = "life crystal", chance = 16666},
	{id = "orb", chance = 16666},
	{id = "fire sword", chance = 14285},
	{id = "huge chunk of crude iron", chance = 14285},
	{id = "mysterious voodoo skull", chance = 12500},
	{id = "scimitar", chance = 11111},
	{id = "crimson sword", chance = 10000},
	{id = "spike sword", chance = 9090},
	{id = "demon horn", maxCount = 2, chance = 8333},
	{id = "dragon slayer", chance = 8333},
	{id = "strange helmet", chance = 8333},
	{id = "enchanted chicken wing", chance = 7692},
	{id = "giant sword", chance = 7692},
	{id = "runed sword", chance = 6666},
	{id = "crown helmet", chance = 6250},
	{id = "flask of warrior's sweat", chance = 5555},
	{id = "spirit container", chance = 4761},
	{id = "unholy book", chance = 2063}
}

monster.attacks = {
	{name = "melee",  attack = 145, skill = 140, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 1000, minDamage = -250, maxDamage = -500, length = 10, effect = 18},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 8, interval = 1000, minDamage = -30, maxDamage = -760, radius = 5, shootEffect = 11, effect = 18},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 9, interval = 2000, minDamage = -200, maxDamage = -585, length = 8, radius = 5, shootEffect = 11, effect = 46},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 8, interval = 1000, minDamage = 0, maxDamage = -430, radius = 6, shootEffect = 11, effect = 43},
	{name = "drunk",  chance = 11, interval = 3000, minDamage = 0, maxDamage = 0, radius = 6, shootEffect = 11, effect = 23},
	{name = "condition", type = CONDITION_ENERGY,  chance = 15, interval = 2000, minDamage = -250, maxDamage = -250, radius = 4, shootEffect = 11, effect = 12}
}

monster.defenses = {
	defense = 45,
    armor = 50,
    {name = "combat", type = COMBAT_HEALING,  chance = 12, interval = 1000, minDamage = 400, maxDamage = 600, radius = 4, shootEffect = 11, effect = 15},
	{name = "speed",  chance = 4, interval = 1000, minDamage = 0, maxDamage = 0, duration = 7000, speedChange = 400, radius = 4, shootEffect = 11, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 50},
	{type = COMBAT_ENERGYDAMAGE, percent = 30},
	{type = COMBAT_EARTHDAMAGE, percent = 30},
	{type = COMBAT_FIREDAMAGE, percent = 30},
	{type = COMBAT_HOLYDAMAGE, percent = 25},
	{type = COMBAT_ICEDAMAGE, percent = 30}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
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