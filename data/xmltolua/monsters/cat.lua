local mtype = Game.createMonsterType("cat")
local monster = {}
monster.description = "a cat"
monster.experience = 0
monster.outfit = {
	lookType = 276
}

monster.health = 20
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7637
monster.speed = 190
monster.runHealth = 8
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
    chance = 0
}

monster.flags = {
    healthHidden = false,
    summonable = false,
    attackable = true,
    hostile = false,
    convinceable = true,
    illusionable = true,
    canPushItems = false,
    canPushCreatures = false,
    targetDistance = 1,
    staticAttack = 95
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Mew!", yell = false},
	{text = "Meow!", yell = false},
	{text = "Meow meow!", yell = false}
}

monster.attacks = {
	{name = "",  chance = 100, interval = 2000, minDamage = 0, maxDamage = 0, range = 1}
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