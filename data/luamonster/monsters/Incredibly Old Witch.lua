local mType = Game.createMonsterType("Incredibly Old Witch")
local monster = {}
monster.description = "a incredibly old witch"
monster.experience = 0
monster.outfit = {
	lookFeet = 50,
	lookLegs = 40,
	lookHead = 20,
	lookType = 54,
	lookBody = 30
}

monster.health = 1000000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6081
monster.speed = 180
monster.runHealth = 1000000
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
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Feel the wrath of the witch!", yell = false},
	{text = "Oh how you will regret to have distubed me!", yell = false},
	{text = "Everyone is so stupid!", yell = false},
	{text = "Stupid people!", yell = false}
}

monster.attacks = {
	{name = "outfit",  chance = 12, interval = 1000, minDamage = 0, maxDamage = 0, range = 7, radius = 4, effect = 13},
	{name = "outfit",  chance = 12, interval = 1000, minDamage = 0, maxDamage = 0, range = 7, radius = 4, effect = 13},
	{name = "outfit",  chance = 12, interval = 1000, minDamage = 0, maxDamage = 0, range = 7, radius = 4, effect = 13},
	{name = "outfit",  chance = 12, interval = 1000, minDamage = 0, maxDamage = 0, range = 7, radius = 4, effect = 13},
	{name = "outfit",  chance = 12, interval = 1000, minDamage = 0, maxDamage = 0, range = 7, radius = 4, effect = 13},
	{name = "outfit",  chance = 12, interval = 1000, minDamage = 0, maxDamage = 0, range = 7, radius = 4, effect = 13},
	{name = "outfit",  chance = 12, interval = 1000, minDamage = 0, maxDamage = 0, range = 7, radius = 4, effect = 13},
	{name = "outfit",  chance = 12, interval = 1000, minDamage = 0, maxDamage = 0, range = 7, radius = 4, effect = 13},
	{name = "outfit",  chance = 12, interval = 1000, minDamage = 0, maxDamage = 0, range = 7, radius = 4, effect = 13}
}

monster.defenses = {
	defense = 0,
    armor = 0,
    {name = "outfit",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, effect = 13},
	{name = "outfit",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, effect = 13},
	{name = "outfit",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, effect = 13},
	{name = "outfit",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, effect = 13},
	{name = "outfit",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, effect = 13},
	{name = "outfit",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, effect = 13},
	{name = "outfit",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, effect = 13},
	{name = "outfit",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, effect = 13},
	{name = "outfit",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, effect = 13}
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