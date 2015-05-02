local mType = Game.createMonsterType("Kreebosh the Exile")
local monster = {}
monster.description = "kreebosh the exile"
monster.experience = 350
monster.outfit = {
	lookType = 103
}

monster.health = 880
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7349
monster.speed = 270
monster.maxSummons = 2

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

monster.summons = {
    {name = "Green Djinn", chance = 20, interval = 5000}
}

monster.voices = {
	interval = 5000,
	chance = 5,
	{text = "I bet you wish you weren't here.", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 30, minDamage = 0, maxDamage = -100, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 80, interval = 6000, minDamage = 0, maxDamage = -120, radius = 3, effect = 12},
	{name = "speed",  chance = 35, interval = 1, minDamage = 0, maxDamage = 0, target = true, duration = 20000, range = 5, radius = 1, effect = 14},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 40, interval = 6000, minDamage = -20, maxDamage = -100, target = true, range = 5, radius = 1, shootEffect = 32, effect = 18},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 20, interval = 5000, minDamage = -40, maxDamage = -200, target = true, range = 5, radius = 1, shootEffect = 5, effect = 7},
	{name = "drunk",  chance = 20, interval = 1000, minDamage = 0, maxDamage = 0, target = true, range = 5, radius = 1, shootEffect = 5, effect = 7},
	{name = "outfit",  chance = 50, interval = 2000, minDamage = 0, maxDamage = 0, target = true, range = 5, radius = 1, shootEffect = 5, effect = 15}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 55},
	{type = COMBAT_DEATHDAMAGE, percent = -10}
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
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)