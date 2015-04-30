local mType = Game.createMonsterType("Humorless Fungus")
local monster = {}
monster.description = "a humorless fungus"
monster.experience = 0
monster.outfit = {
	lookType = 517
}

monster.health = 8000
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 17428
monster.speed = 230
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
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Munch munch munch!", yell = false},
	{text = "Chatter", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 95, skill = 90, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = -40, maxDamage = -197, range = 7, shootEffect = 39, effect = 46},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -525, range = 7, shootEffect = 13, effect = 42},
	{name = "condition", type = CONDITION_POISON,  chance = 10, interval = 2000, minDamage = -400, maxDamage = -640, range = 7, radius = 3, shootEffect = 13, effect = 17},
	{name = "drunk",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 4, shootEffect = 13, effect = 32}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 5, interval = 2000, minDamage = 0, maxDamage = 230, radius = 4, shootEffect = 13, effect = 13},
	{name = "invisible",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, radius = 4, shootEffect = 13, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 15},
	{type = COMBAT_ICEDAMAGE, percent = 15},
	{type = COMBAT_HOLYDAMAGE, percent = 5},
	{type = COMBAT_FIREDAMAGE, percent = 5}
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