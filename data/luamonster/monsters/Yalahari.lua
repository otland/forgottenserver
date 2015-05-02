local mType = Game.createMonsterType("Yalahari")
local monster = {}
monster.description = "a yalahari"
monster.experience = 5
monster.outfit = {
	lookType = 309
}

monster.health = 150
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20550
monster.speed = 200
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 8
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = false,
	convinceable = false,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 11,
	{text = "Welcome to Yalahar, outsider.", yell = false},
	{text = "Hail Yalahar.", yell = false},
	{text = "You can learn a lot from us.", yell = false},
	{text = "Our wisdom and knowledge are unequalled in this world.", yell = false},
	{text = "That knowledge would overburden your fragile mind.", yell = false},
	{text = "I wouldn't expect you to understand.", yell = false},
	{text = "One day Yalahar will return to its former glory.", yell = false}
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
	{type = "physical", condition = false, combat = true},
	{type = "bleed", condition = true, combat = false},
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)