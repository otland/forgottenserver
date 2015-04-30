local mType = Game.createMonsterType("Hellhound")
local monster = {}
monster.description = "a hellhound"
monster.experience = 6800
monster.outfit = {
	lookType = 240
}

monster.health = 7500
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6332
monster.speed = 280
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
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 70
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "GROOOWL!", yell = false},
	{text = "GRRRRR!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "platinum coin", maxCount = 7, chance = 100000},
	{id = "ham", maxCount = 14, chance = 30930},
	{id = "assassin star", maxCount = 10, chance = 38000},
	{id = "black pearl", maxCount = 4, chance = 9400},
	{id = "big bone", chance = 1000},
	{id = "concentrated demonic blood", maxCount = 2, chance = 31000},
	{id = "demonic essence", chance = 17000},
	{id = "gold ingot", chance = 1500},
	{id = "rusty armor", chance = 4500},
	{id = "giant sword", chance = 950},
	{id = "soul orb", chance = 53000},
	{id = "knight axe", chance = 10810},
	{id = "spike sword", chance = 16310},
	{id = "ruthless axe", chance = 400},
	{id = "hellhound slobber", chance = 18000},
	{id = "magma legs", chance = 1270},
	{id = "fire sword", chance = 6900},
	{id = "explorer brooch", chance = 130},
	{id = "wand of inferno", chance = 9000},
	{id = "great health potion", maxCount = 2, chance = 15150},
	{id = "great mana potion", maxCount = 3, chance = 15150}
}

monster.attacks = {
	{name = "melee",  attack = 130, skill = 70, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 5, interval = 2000, minDamage = -300, maxDamage = -700, length = 8, spread = 3, effect = 47},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 10, interval = 2000, minDamage = -395, maxDamage = -498, range = 7, shootEffect = 32, effect = 39},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 2000, minDamage = -350, maxDamage = -660, length = 8, spread = 3, shootEffect = 32, effect = 7},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -350, maxDamage = -976, length = 8, spread = 3, shootEffect = 32, effect = 14},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 2000, minDamage = -200, maxDamage = -403, target = true, radius = 1, shootEffect = 32, effect = 16},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 5, interval = 2000, minDamage = -300, maxDamage = -549, range = 7, radius = 1, shootEffect = 15, effect = 21}
}

monster.defenses = {
	defense = 40,
    armor = 40,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 320, radius = 1, shootEffect = 15, effect = 14},
	{name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 2000, minDamage = 220, maxDamage = 425, radius = 1, shootEffect = 15, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE, percent = -25},
	{type = COMBAT_ICEDAMAGE, percent = -5}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = true, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)