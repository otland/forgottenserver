local mType = Game.createMonsterType("The Collector")
local monster = {}
monster.description = "an the collector"
monster.experience = 100
monster.outfit = {
	lookType = 261
}

monster.health = 340
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 7282
monster.speed = 195
monster.runHealth = 20
monster.maxSummons = 0

monster.changeTarget = {
    interval = 2000,
	chance = 5
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
	staticAttack = 50
}

monster.voices = {
	interval = 2000,
	chance = 5,
	{text = "Leave as long as you can.", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 100, minDamage = 0, maxDamage = -220, interval = 2000, effect = 0},
	{name = "speed",  chance = 13, interval = 1000, minDamage = 0, maxDamage = 0, duration = 20000, speedChange = -800, length = 8, effect = 12},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 1000, minDamage = 0, maxDamage = -85, range = 7, shootEffect = 12, effect = 12},
	{name = "melee",  attack = 0, skill = 0, minDamage = -10, maxDamage = -80, interval = 2000, effect = 10}
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
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)