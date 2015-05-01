local mType = Game.createMonsterType("Son of Verminor")
local monster = {}
monster.description = "a son of verminor"
monster.experience = 5900
monster.outfit = {
	lookType = 19
}

monster.health = 8500
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 1490
monster.speed = 240
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 70
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Blubb", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 90, skill = 95, minDamage = 0, maxDamage = -473, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = -150, maxDamage = -200, range = 7, shootEffect = 15},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -350, maxDamage = -390, radius = 3, shootEffect = 15, effect = 21},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 15, interval = 2000, minDamage = -220, maxDamage = -270, radius = 3, shootEffect = 15, effect = 39}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 2000, minDamage = 250, maxDamage = 350, shootEffect = 15, effect = 13},
	{name = "outfit",  chance = 10, interval = 5000, minDamage = 0, maxDamage = 0, shootEffect = 15, effect = 13},
	{name = "outfit",  chance = 10, interval = 5000, minDamage = 0, maxDamage = 0, shootEffect = 15, effect = 13},
	{name = "outfit",  chance = 10, interval = 5000, minDamage = 0, maxDamage = 0, shootEffect = 15, effect = 13},
	{name = "outfit",  chance = 10, interval = 5000, minDamage = 0, maxDamage = 0, shootEffect = 15, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 10}
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
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)