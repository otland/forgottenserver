local mType = Game.createMonsterType("Thornback Tortoise")
local monster = {}
monster.description = "a thornback tortoise"
monster.experience = 150
monster.outfit = {
	lookType = 198
}

monster.health = 300
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6073
monster.speed = 200
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 48, chance = 89500},
	{id = "tortoise egg", maxCount = 3, chance = 790},
	{id = "fish", maxCount = 2, chance = 10800},
	{id = "health potion", chance = 1600},
	{id = "brown mushroom", chance = 700},
	{id = "white mushroom", chance = 1200},
	{id = "war hammer", chance = 260},
	{id = "thorn", chance = 15980},
	{id = "white pearl", chance = 1600},
	{id = "turtle shell", chance = 800},
	{id = "black pearl", chance = 800}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 45, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 45},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_ICEDAMAGE, percent = 20}
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