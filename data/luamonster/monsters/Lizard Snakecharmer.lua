local mType = Game.createMonsterType("Lizard Snakecharmer")
local monster = {}
monster.description = "a lizard snakecharmer"
monster.experience = 210
monster.outfit = {
	lookType = 115
}

monster.health = 325
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6041
monster.speed = 190
monster.maxSummons = 6

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
	targetDistance = 4,
	staticAttack = 80
}

monster.summons = {
    {name = "cobra", chance = 20, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "I ssssmell warm blood!", yell = false},
	{text = "Shhhhhhhh", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 55, chance = 83740},
	{id = "cape", chance = 8640},
	{id = "dead snake", chance = 30000},
	{id = "lizard leather", chance = 1320},
	{id = "small amethyst", chance = 520},
	{id = "snakebite rod", chance = 230},
	{id = "life crystal", chance = 1430},
	{id = "lizard scale", chance = 860},
	{id = "terra rod", chance = 920},
	{id = "mana potion", chance = 860},
	{id = "yellow gem", chance = 150},
	{id = "life ring", chance = 340},
	{id = "charmer's tiara", chance = 230}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 20, interval = 2000, effect = 0},
	{name = "condition", type = CONDITION_POISON,  chance = 15, interval = 2000, minDamage = -100, maxDamage = -200, range = 7, shootEffect = 15},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -50, maxDamage = -110, target = true, range = 7, radius = 1, shootEffect = 15, effect = 9}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 50, interval = 2000, minDamage = 50, maxDamage = 100, radius = 1, shootEffect = 15, effect = 13},
	{name = "invisible",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, radius = 1, shootEffect = 15, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -20},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_ICEDAMAGE, percent = 20}
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