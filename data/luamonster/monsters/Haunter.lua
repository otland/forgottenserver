local mType = Game.createMonsterType("Haunter")
local monster = {}
monster.description = "haunter"
monster.experience = 4000
monster.outfit = {
	lookType = 320
}

monster.health = 8500
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 9915
monster.speed = 270
monster.maxSummons = 0

monster.changeTarget = {
    interval = 2000,
	chance = 9
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
	chance = 9,
	{text = "Surrender and I'll end it quick!", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 50, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 16, interval = 2000, minDamage = 0, maxDamage = -130, radius = 3, effect = 12},
	{name = "condition", type = CONDITION_POISON,  chance = 16, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, radius = 3, shootEffect = 15, effect = 21}
}

monster.defenses = {
	defense = 20,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 16, interval = 2000, minDamage = 100, maxDamage = 155, radius = 3, shootEffect = 15, effect = 13},
	{name = "speed",  chance = 16, interval = 2000, minDamage = 0, maxDamage = 0, duration = 80000, speedChange = 360, radius = 3, shootEffect = 15, effect = 14}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
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