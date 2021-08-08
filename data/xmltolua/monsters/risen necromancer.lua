local mtype = Game.createMonsterType("risen necromancer")
local monster = {}
monster.description = "a risen necromancer"
monster.experience = 40
monster.outfit = {
	lookType = 300
}

monster.health = 70
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 5972
monster.speed = 160
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
    chance = 10
}

monster.flags = {
    healthHidden = false,
    summonable = false,
    attackable = true,
    hostile = true,
    convinceable = false,
    illusionable = false,
    canPushItems = true,
    canPushCreatures = true,
    targetDistance = 1,
    staticAttack = 95
}

monster.loot = {
    {id = 5925 --[[bone]], chance = 7500},
	{id = 18419 --[[Tharian gem]], maxCount = 20, chance = 25000},
	{id = 2388 --[[hatchet]], chance = 3500},
	{id = 2398 --[[mace]], chance = 2500}
}

monster.attacks = {
	{name = "",  chance = 100, interval = 1000, minDamage = 0, maxDamage = -12, range = 1},
	{name = "",  chance = 10, interval = 1000, minDamage = 0, maxDamage = -13, range = 7},
	{name = "",  chance = 10, interval = 1000, minDamage = 0, maxDamage = -13, range = 0},
	{name = "",  chance = 10, interval = 1000, minDamage = 0, maxDamage = -13, range = 0},
	{name = "",  chance = 10, interval = 1000, minDamage = 0, maxDamage = -13, range = 0}
}

monster.defenses = {
	defense = 4,
    armor = 4,
    {name = "",  chance = 10, interval = 1000, minDamage = 10, maxDamage = 20, range = 0}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -20},
	{type = COMBAT_HOLYDAMAGE, percent = -20},
	{type = COMBAT_DEATHDAMAGE, percent = 60},
	{type = COMBAT_ICEDAMAGE, percent = 10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "poison", condition = false, combat = false},
	{type = "cursed", condition = false, combat = false},
	{type = "invisible", condition = true, combat = false},
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