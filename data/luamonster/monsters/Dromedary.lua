local mType = Game.createMonsterType("Dromedary")
local monster = {}
monster.description = "a dromedary"
monster.experience = 0
monster.outfit = {
	lookType = 404
}

monster.health = 45
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 13528
monster.speed = 170
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 20
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = false,
	convinceable = false,
    illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Snort", yell = false},
	{text = "Grunt!", yell = false}
}

monster.loot = {
    {id = "meat", maxCount = 2, chance = 36000},
	{id = "sugar oat", chance = 1000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0},
	{name = "drunk",  chance = 5, interval = 4000, minDamage = 0, maxDamage = 0, range = 1, shootEffect = 41, effect = 32}
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