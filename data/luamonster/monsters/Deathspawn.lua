local mType = Game.createMonsterType("Deathspawn")
local monster = {}
monster.description = "a deathspawn"
monster.experience = 20
monster.outfit = {
	lookFeet = 114,
	lookLegs = 97,
	lookHead = 114,
	lookType = 226,
	lookBody = 98
}

monster.health = 225
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 2220
monster.speed = 230
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 8
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Ribbit!", yell = false},
	{text = "Ribbit! Ribbit!", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 10, minDamage = 0, maxDamage = -40, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 1000, minDamage = -400, maxDamage = -700, length = 7, effect = 6},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 11, interval = 1000, minDamage = -250, maxDamage = -450, length = 7, effect = 48}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -15},
	{type = COMBAT_ICEDAMAGE, percent = -15},
	{type = COMBAT_HOLYDAMAGE, percent = 10},
	{type = COMBAT_FIREDAMAGE, percent = -15}
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)