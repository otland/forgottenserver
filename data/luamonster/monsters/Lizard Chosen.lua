local mType = Game.createMonsterType("Lizard Chosen")
local monster = {}
monster.description = "a lizard chosen"
monster.experience = 2200
monster.outfit = {
	lookType = 344
}

monster.health = 3050
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 11288
monster.speed = 260
monster.runHealth = 50
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
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Grzzzzzzz!", yell = false},
	{text = "Garrrblarrrrzzzz!", yell = false},
	{text = "Kzzzzzz!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 33000},
	{id = "gold coin", maxCount = 100, chance = 32000},
	{id = "gold coin", maxCount = 36, chance = 32000},
	{id = "lizard leather", chance = 2000},
	{id = "spiked iron ball", chance = 9890},
	{id = "scale of corruption", chance = 2870},
	{id = "platinum coin", maxCount = 5, chance = 2920},
	{id = "cursed shoulder spikes", chance = 5800},
	{id = "great health potion", maxCount = 3, chance = 5350},
	{id = "Zaoan shoes", chance = 810},
	{id = "small diamond", maxCount = 5, chance = 2550},
	{id = "corrupted flag", chance = 3350},
	{id = "lizard scale", maxCount = 3, chance = 980},
	{id = "tower shield", chance = 1100},
	{id = "Zaoan legs", chance = 940},
	{id = "Zaoan helmet", chance = 140},
	{id = "Zaoan armor", chance = 980}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 80, minDamage = 0, maxDamage = -360, interval = 2000, effect = 0},
	{name = "condition", type = CONDITION_POISON,  chance = 15, interval = 2000, minDamage = -240, maxDamage = -320, length = 3, spread = 2, effect = 21},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -190, maxDamage = -340, radius = 3, effect = 17},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = -90, maxDamage = -180, length = 8, effect = 9}
}

monster.defenses = {
	defense = 45,
    armor = 45,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 75, maxDamage = 125, effect = 15}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = 10},
	{type = COMBAT_FIREDAMAGE, percent = 10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)