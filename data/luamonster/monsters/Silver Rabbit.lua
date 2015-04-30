local mType = Game.createMonsterType("Silver Rabbit")
local monster = {}
monster.description = "a silver rabbit"
monster.experience = 0
monster.outfit = {
	lookFeet = 66,
	lookLegs = 69,
	lookHead = 69,
	lookType = 262,
	lookBody = 66
}

monster.health = 15
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7338
monster.speed = 195
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = false,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 70
}

monster.loot = {
    {id = "meat", maxCount = 2, chance = 86170},
	{id = "carrot", chance = 11150},
	{id = "silky fur", chance = 28670}
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