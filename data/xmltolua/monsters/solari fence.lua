local mtype = Game.createMonsterType("solari fence")
local monster = {}
monster.description = "a solari fence"
monster.experience = 50
monster.outfit = {
	lookFeet = 23,
	lookLegs = 22,
	lookHead = 96,
	lookType = 150,
	lookAddons = 1,
	lookBody = 39
}

monster.health = 45
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20459
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
    canPushItems = false,
    canPushCreatures = false,
    targetDistance = 1,
    staticAttack = 95
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "I own these things, you can't take them away!", yell = false},
	{text = "For the Solari!", yell = false}
}

monster.loot = {
    {id = 18419 --[[Tharian gem]], maxCount = 10, chance = 25000},
	{id = 26338 --[[sack of stolen grain]], chance = 100000}
}

monster.attacks = {
	{name = "",  chance = 100, interval = 1000, minDamage = 0, maxDamage = -18, range = 1},
	{name = "",  chance = 15, interval = 1000, minDamage = -1, maxDamage = -14, range = 1},
	{name = "",  chance = 15, interval = 1000, minDamage = 0, maxDamage = 0, range = 0},
	{name = "",  chance = 10, interval = 1000, minDamage = -1, maxDamage = -14, range = 1}
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