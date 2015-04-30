local mType = Game.createMonsterType("Heoni")
local monster = {}
monster.description = "heoni"
monster.experience = 515
monster.outfit = {
	lookType = 239
}

monster.health = 795
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6302
monster.speed = 300
monster.maxSummons = 0

monster.changeTarget = {
    interval = 2000,
	chance = 10
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

monster.voices = {
	interval = 2000,
	chance = 10,
	{text = "Shriiiek", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 50, interval = 2000, effect = 0},
	{name = "condition", type = CONDITION_POISON,  chance = 15, interval = 2000, minDamage = -20, maxDamage = -240, length = 8, spread = 3, effect = 21},
	{name = "drunk",  chance = 13, interval = 2000, minDamage = 0, maxDamage = 0, length = 8, spread = 3, effect = 25}
}

monster.defenses = {
	defense = 18,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 11, interval = 2000, minDamage = 76, maxDamage = 84, effect = 13},
	{name = "speed",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 290, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = 10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
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