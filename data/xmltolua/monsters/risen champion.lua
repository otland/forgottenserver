local mtype = Game.createMonsterType("risen champion")
local monster = {}
monster.description = "a risen champion"
monster.experience = 50
monster.outfit = {
	lookType = 306
}

monster.health = 80
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 9823
monster.speed = 170
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
    canPushItems = true,
    canPushCreatures = true,
    targetDistance = 1,
    staticAttack = 95
}

monster.loot = {
    {id = 5925 --[[bone]], chance = 10000},
	{id = 18419 --[[Tharian gem]], maxCount = 20, chance = 25000},
	{id = 2388 --[[hatchet]], chance = 4000},
	{id = 2398 --[[mace]], chance = 3000}
}

monster.attacks = {
	{name = "",  chance = 100, interval = 1000, minDamage = 0, maxDamage = -25, range = 1},
	{name = "",  chance = 20, interval = 1000, minDamage = 0, maxDamage = -15, range = 7}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 80},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE, percent = 10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
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