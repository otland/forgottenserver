local mType = Game.createMonsterType("Madareth")
local monster = {}
monster.description = "madareth"
monster.experience = 10000
monster.outfit = {
	lookFeet = 79,
	lookLegs = 82,
	lookHead = 77,
	lookType = 12,
	lookBody = 116
}

monster.health = 55000
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 6068
monster.speed = 380
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 8
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 85
}

monster.voices = {
	interval = 3000,
	chance = 5,
	{text = "I am going to play with yourself!", yell = true}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "mastermind shield", chance = 12650},
	{id = "ring of healing", chance = 8000},
	{id = "demon shield", chance = 7650},
	{id = "small diamond", chance = 5800}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = true, combat = true},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)