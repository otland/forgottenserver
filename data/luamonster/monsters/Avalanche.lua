local mType = Game.createMonsterType("Avalanche")
local monster = {}
monster.description = "avalanche"
monster.experience = 305
monster.outfit = {
	lookType = 261
}

monster.health = 550
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 7349
monster.speed = 195
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
	interval = 5000,
	chance = 5,
	{text = "You will pay for imprisoning me here.", yell = false},
	{text = "Puny warmblood.", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 60, skill = 50, minDamage = 0, maxDamage = -180, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DROWNDAMAGE,  chance = 100, interval = 1000, minDamage = -10, maxDamage = -50, length = 5, spread = 6, effect = 38},
	{name = "speed",  chance = 100, interval = 4000, minDamage = 0, maxDamage = 0, duration = 10000, speedChange = -400, radius = 3, effect = 3},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 100, interval = 6000, minDamage = 0, maxDamage = -40, target = true, range = 5, radius = 1, shootEffect = 12, effect = 3}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 30}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)