local mType = Game.createMonsterType("Slim")
local monster = {}
monster.description = "slim"
monster.experience = 580
monster.outfit = {
	lookType = 101
}

monster.health = 1025
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 7349
monster.speed = 200
monster.maxSummons = 0

monster.changeTarget = {
    interval = 0,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 95
}

monster.voices = {
	interval = 2000,
	chance = 5,
	{text = "Zhroozzzzs.", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 50, minDamage = 0, maxDamage = -150, interval = 2000, effect = 0},
	{name = "poisonfield",  chance = 50, interval = 1000, minDamage = 0, maxDamage = 0, shootEffect = 15},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 40, interval = 3049, minDamage = -10, maxDamage = -50, shootEffect = 15, effect = 10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = true, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)