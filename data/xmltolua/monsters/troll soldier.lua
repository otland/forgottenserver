local mtype = Game.createMonsterType("troll soldier")
local monster = {}
monster.description = "a troll soldier"
monster.experience = 50
monster.outfit = {
	lookType = 281
}

monster.health = 40
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7926
monster.speed = 160
monster.runHealth = 10
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
    chance = 20
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
    {id = 18419 --[[Tharian gem]], maxCount = 12, chance = 25000},
	{id = 2448 --[[studded club]], chance = 6000},
	{id = 2380 --[[hand axe]], chance = 5000},
	{id = 2643 --[[leather boots]], chance = 4000},
	{id = 2666 --[[meat]], chance = 10000},
	{id = 10606 --[[bunch of troll hair]], chance = 5000}
}

monster.attacks = {
	{name = "",  chance = 100, interval = 1000, minDamage = 0, maxDamage = -15, range = 1},
	{name = "",  chance = 20, interval = 1000, minDamage = 0, maxDamage = -12, range = 3}
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