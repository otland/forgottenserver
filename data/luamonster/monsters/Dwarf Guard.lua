local mType = Game.createMonsterType("Dwarf Guard")
local monster = {}
monster.description = "a dwarf guard"
monster.experience = 165
monster.outfit = {
	lookType = 70
}

monster.health = 245
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6013
monster.speed = 205
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Hail Durin!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 30, chance = 40000},
	{id = "health potion", chance = 380},
	{id = "iron ore", chance = 510},
	{id = "leather boots", chance = 40000},
	{id = "small amethyst", chance = 140},
	{id = "steel helmet", chance = 1600},
	{id = "battle shield", chance = 6000},
	{id = "battle hammer", chance = 4000},
	{id = "white mushroom", maxCount = 2, chance = 55000},
	{id = "scale armor", chance = 9200},
	{id = "double axe", chance = 600},
	{id = "iron helmet", chance = 250},
	{id = "axe ring", chance = 190},
	{id = "coal", chance = 280}
}

monster.attacks = {
	{name = "melee",  attack = 70, skill = 30, minDamage = 0, maxDamage = -140, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -5},
	{type = COMBAT_DEATHDAMAGE, percent = -5}
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)