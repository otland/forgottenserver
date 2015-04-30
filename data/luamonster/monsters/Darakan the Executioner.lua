local mType = Game.createMonsterType("Darakan the Executioner")
local monster = {}
monster.description = "darakan the executioner"
monster.experience = 1600
monster.outfit = {
	lookFeet = 114,
	lookLegs = 114,
	lookHead = 78,
	lookType = 255,
	lookBody = 114
}

monster.health = 3500
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7349
monster.speed = 205
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
	{text = "FIGHT LIKE A BARBARIAN!", yell = true},
	{text = "VICTORY IS MINE!", yell = true},
	{text = "I AM your father!", yell = false},
	{text = "To be the man you have to beat the man!", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 70, skill = 50, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 100, interval = 1000, minDamage = -72, maxDamage = -130, shootEffect = 1}
}

monster.defenses = {
	defense = 31,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 65, interval = 6000, minDamage = 20, maxDamage = 50, shootEffect = 1, effect = 13}
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