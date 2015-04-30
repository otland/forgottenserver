local mType = Game.createMonsterType("Spirit of Water")
local monster = {}
monster.description = "the spirit of water"
monster.experience = 850
monster.outfit = {
	lookType = 11
}

monster.health = 1517
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
	interval = 5000,
	chance = 5,
	{text = "Blubb", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 70, interval = 2000, effect = 0},
	{name = "poisonfield",  chance = 50, interval = 1000, minDamage = 0, maxDamage = 0, shootEffect = 15},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 40, interval = 2000, minDamage = -1, maxDamage = -120, shootEffect = 15, effect = 2}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10}
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)