local mType = Game.createMonsterType("Pythius The Rotten")
local monster = {}
monster.description = "a pythius the rotten"
monster.experience = 7000
monster.outfit = {
	lookType = 231
}

monster.health = 9500
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 7349
monster.speed = 350
monster.maxSummons = 3

monster.changeTarget = {
    interval = 5000,
	chance = 8
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.summons = {
    {name = "Undead Gladiator", chance = 10, interval = 1000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "YOU'LL NEVER GET MY TREASURE!", yell = true},
	{text = "MINIONS, MEET YOUR NEW BROTHER!", yell = true},
	{text = "YOU WILL REGRET THAT YOU ARE BORN!", yell = true},
	{text = "YOU MADE A HUGE WASTE!", yell = true},
	{text = "I SENSE LIFE", yell = true}
}

monster.attacks = {
	{name = "melee",  attack = 60, skill = 60, minDamage = 0, maxDamage = -210, interval = 1950, effect = 0},
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -795, interval = 2000, effect = 1},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2800, minDamage = 0, maxDamage = -400, range = 7, shootEffect = 32, effect = 18},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 17, interval = 2000, minDamage = -55, maxDamage = -155, target = true, range = 7, radius = 4, shootEffect = 15, effect = 21},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 14, interval = 2500, minDamage = -333, maxDamage = -413, length = 8, spread = 3, shootEffect = 15, effect = 21},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 18, interval = 2000, minDamage = -165, maxDamage = -200, length = 7, shootEffect = 15, effect = 1},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 22, interval = 2500, minDamage = -85, maxDamage = -110, range = 8, shootEffect = 29, effect = 1},
	{name = "condition", type = CONDITION_DROWN,  chance = 45, interval = 1000, minDamage = 0, maxDamage = 0, length = 8, spread = 3, shootEffect = 29, effect = 3}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = true, combat = true},
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