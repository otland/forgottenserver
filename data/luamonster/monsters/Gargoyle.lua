local mType = Game.createMonsterType("Gargoyle")
local monster = {}
monster.description = "a gargoyle"
monster.experience = 150
monster.outfit = {
	lookType = 95
}

monster.health = 250
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6027
monster.speed = 200
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
	{text = "Harrrr Harrrr!", yell = false},
	{text = "Stone sweet stone.", yell = false},
	{text = "Feel my claws, softskin.", yell = false},
	{text = "Chhhhhrrrrk!", yell = false},
	{text = "There is a stone in your shoe!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 30, chance = 88000},
	{id = "battle shield", chance = 1000},
	{id = "potato", maxCount = 2, chance = 9220},
	{id = "strawberry", maxCount = 5, chance = 1810},
	{id = "morning star", chance = 2150},
	{id = "piece of marble rock", chance = 630},
	{id = "mysterious fetish", maxCount = 10, chance = 15800},
	{id = "steel helmet", chance = 850},
	{id = "dark armor", chance = 300},
	{id = "wolf tooth chain", chance = 1480},
	{id = "club ring", chance = 260},
	{id = "shiny stone", chance = 190},
	{id = "stone wing", chance = 11730}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 55, interval = 2000, effect = 0}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 2000, minDamage = 5, maxDamage = 15, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 40},
	{type = COMBAT_DEATHDAMAGE, percent = 1},
	{type = COMBAT_FIREDAMAGE, percent = -10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = false, combat = false},
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