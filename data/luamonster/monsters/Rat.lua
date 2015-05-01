local mType = Game.createMonsterType("Rat")
local monster = {}
monster.description = "a rat"
monster.experience = 5
monster.outfit = {
	lookType = 21
}

monster.health = 20
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5964
monster.speed = 135
monsters.runHealth = 5
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
	{text = "Meep!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 4, chance = 100000},
	{id = "cheese", chance = 39410}
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