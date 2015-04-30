local mType = Game.createMonsterType("Modified Gnarlhound")
local monster = {}
monster.description = "a modified gnarlhound"
monster.experience = 0
monster.outfit = {
	lookType = 515
}

monster.health = 1500
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 13528
monster.speed = 150
monster.maxSummons = 0

monster.changeTarget = {
    interval = 2000,
	chance = 20
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = false,
	convinceable = true,
    illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 90
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
	{type = "physical", condition = false, combat = true},
	{type = "bleed", condition = true, combat = false},
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = true, combat = true},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)