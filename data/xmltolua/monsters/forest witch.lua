local mtype = Game.createMonsterType("forest witch")
local monster = {}
monster.description = "a forest witch"
monster.experience = 100
monster.outfit = {
	lookType = 54
}

monster.health = 100
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20535
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
    canPushCreatures = false,
    targetDistance = 4,
    staticAttack = 95
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Horax pokti!", yell = false},
	{text = "Herba budinia ex!", yell = false},
	{text = "Hihihihi!", yell = false}
}

monster.loot = {
    {id = 18419 --[[Tharian gem]], maxCount = 30, chance = 25000},
	{id = 10569 --[[witch broom]], chance = 10000}
}

monster.attacks = {
	{name = "",  chance = 100, interval = 1000, minDamage = 0, maxDamage = -15, range = 1},
	{name = "",  chance = 10, interval = 1000, minDamage = -5, maxDamage = -15, range = 7},
	{name = "",  chance = 5, interval = 1000, minDamage = 0, maxDamage = 0, range = 7},
	{name = "",  chance = 1, interval = 1000, minDamage = 0, maxDamage = 0, range = 5}
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