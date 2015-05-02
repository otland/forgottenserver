local mType = Game.createMonsterType("Cave Rat")
local monster = {}
monster.description = "a cave rat"
monster.experience = 10
monster.outfit = {
	lookType = 56
}

monster.health = 30
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5964
monster.speed = 150
monster.runHealth = 3
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
    illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Meeeeep!", yell = false},
	{text = "Meep!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 2, chance = 85000},
	{id = "worm", maxCount = 2, chance = 9700},
	{id = "cheese", chance = 30000},
	{id = "cookie", chance = 750}
}

monster.attacks = {
	{name = "melee",  attack = 10, skill = 10, minDamage = 0, maxDamage = -10, interval = 2000, effect = 0}
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