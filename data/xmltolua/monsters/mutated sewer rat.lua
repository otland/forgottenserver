local mtype = Game.createMonsterType("mutated sewer rat")
local monster = {}
monster.description = "a mutated sewer rat"
monster.experience = 20
monster.outfit = {
	lookType = 305
}

monster.health = 35
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 9871
monster.speed = 134
monster.runHealth = 5
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
    chance = 10
}

monster.flags = {
    healthHidden = false,
    summonable = false,
    attackable = true,
    hostile = true,
    convinceable = false,
    illusionable = false,
    canPushItems = false,
    canPushCreatures = false,
    targetDistance = 1,
    staticAttack = 95
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Grrrrrrrrrrrrrr!", yell = false},
	{text = "Fcccccchhhhhh", yell = false}
}

monster.loot = {
    {id = 18419 --[[Tharian gem]], maxCount = 7, chance = 25000},
	{id = 2696 --[[cheese]], chance = 40000},
	{id = 10585 --[[rat tail]], chance = 15000}
}

monster.attacks = {
	{name = "",  chance = 100, interval = 1000, minDamage = 0, maxDamage = -8, range = 1},
	{name = "",  chance = 15, interval = 1000, minDamage = -1, maxDamage = -5, range = 3},
	{name = "",  chance = 10, interval = 1000, minDamage = 0, maxDamage = 0, range = 0},
	{name = "",  chance = 10, interval = 1000, minDamage = -1, maxDamage = -5, range = 3},
	{name = "",  chance = 10, interval = 1000, minDamage = 0, maxDamage = 0, range = 3}
}

monster.defenses = {
	defense = 3,
    armor = 3,
    {name = "",  chance = 5, interval = 1000, minDamage = 5, maxDamage = 10, range = 0}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "poison", condition = false, combat = false},
	{type = "cursed", condition = false, combat = false},
	{type = "invisible", condition = false, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "manashield", condition = false, combat = false},
	{type = "freezing", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "haste", condition = false, combat = false},
	{type = "healing", condition = false, combat = false},
	{type = "light", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "ice", condition = false, combat = false},
	{type = "infight", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "holy", condition = false, combat = false}
}

mtype:register(monster)