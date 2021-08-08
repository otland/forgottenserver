local mtype = Game.createMonsterType("risen")
local monster = {}
monster.description = "a risen"
monster.experience = 30
monster.outfit = {
	lookType = 33
}

monster.health = 40
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 5972
monster.speed = 154
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
    chance = 0
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

monster.loot = {
    {id = 5925 --[[bone]], chance = 3500},
	{id = 18419 --[[Tharian gem]], maxCount = 10, chance = 25000}
}

monster.attacks = {
	{name = "",  chance = 100, interval = 1000, minDamage = 0, maxDamage = -14, range = 1},
	{name = "",  chance = 10, interval = 1000, minDamage = -1, maxDamage = -10, range = 1}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "lifedrain", condition = false, combat = true},
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
	{type = "death", condition = false, combat = true},
	{type = "ice", condition = false, combat = false},
	{type = "infight", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "holy", condition = false, combat = false}
}

mtype:register(monster)