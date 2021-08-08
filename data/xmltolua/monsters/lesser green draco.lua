local mtype = Game.createMonsterType("lesser green draco")
local monster = {}
monster.description = "a lesser green draco"
monster.experience = 70
monster.outfit = {
	lookType = 34
}

monster.health = 100
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5973
monster.speed = 160
monster.runHealth = 10
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

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "GROOAAARRR", yell = true},
	{text = "FCHHHHH", yell = true}
}

monster.loot = {
    {id = 18419 --[[Tharian gem]], maxCount = 25, chance = 25000}
}

monster.attacks = {
	{name = "",  chance = 100, interval = 1000, minDamage = 0, maxDamage = -20, range = 1},
	{name = "",  chance = 15, interval = 1000, minDamage = -1, maxDamage = -15, range = 7},
	{name = "",  chance = 10, interval = 1000, minDamage = -1, maxDamage = -15, range = 0}
}

monster.defenses = {
	defense = 3,
    armor = 3,
    {name = "",  chance = 15, interval = 1000, minDamage = 5, maxDamage = 25, range = 0}
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