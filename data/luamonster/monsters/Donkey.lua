local mType = Game.createMonsterType("Donkey")
local monster = {}
monster.description = "a donkey"
monster.experience = 0
monster.outfit = {
	lookType = 387
}

monster.health = 45
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 13509
monster.speed = 150
monster.runHealth = 25
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

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Grunt!", yell = false}
}

monster.loot = {
    {id = "meat", maxCount = 2, chance = 20000}
}

monster.attacks = {
	{name = "melee",  attack = 5, skill = 10, minDamage = 0, maxDamage = -5, interval = 2000, effect = 0}
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