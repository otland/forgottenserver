local mType = Game.createMonsterType("Black Sheep")
local monster = {}
monster.description = "a black sheep"
monster.experience = 0
monster.outfit = {
	lookType = 13
}

monster.health = 20
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5994
monster.speed = 130
monster.runHealth = 20
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 20
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = false,
	convinceable = true,
    illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Maeh", yell = false}
}

monster.loot = {
    {id = "meat", maxCount = 5, chance = 70860},
	{id = "black wool", chance = 1000}
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