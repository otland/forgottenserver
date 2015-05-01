local mType = Game.createMonsterType("White Deer")
local monster = {}
monster.description = "a white deer"
monster.experience = 0
monster.outfit = {
	lookType = 400
}

monster.health = 195
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 13513
monster.speed = 190
monsters.runHealth = 195
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = false,
	convinceable = false,
    illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "*wheeze*", yell = false},
	{text = "ROOOAAARR!!", yell = false},
	{text = "*sniff*", yell = false},
	{text = "*bell*", yell = false}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = false, combat = false},
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