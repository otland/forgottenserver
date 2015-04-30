local mType = Game.createMonsterType("The Hag")
local monster = {}
monster.description = "the hag"
monster.experience = 510
monster.outfit = {
	lookFeet = 95,
	lookLegs = 95,
	lookHead = 78,
	lookType = 264,
	lookBody = 97
}

monster.health = 935
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7349
monster.speed = 205
monster.maxSummons = 2

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
	targetDistance = 5,
	staticAttack = 95
}

monster.summons = {
    {name = "Ghost", chance = 26, interval = 2000},
	{name = "Crypt Shambler", chance = 26, interval = 2000}
}

monster.voices = {
	interval = 2000,
	chance = 5,
	{text = "If you think I am to old to fight then you're wrong!", yell = false},
	{text = "I've forgotten more things then you have ever learned!", yell = false},
	{text = "Let me teach you a few things youngster!", yell = false},
	{text = "I'll teach you respect for the old!", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 40, interval = 2000, effect = 0},
	{name = "drunk",  chance = 35, interval = 3000, minDamage = 0, maxDamage = 0, target = true, range = 5, radius = 1},
	{name = "speed",  chance = 55, interval = 1, minDamage = 0, maxDamage = 0, target = true, duration = 12000, range = 5, radius = 1, effect = 14}
}

monster.defenses = {
	defense = 25,
    armor = 24,
    {name = "combat", type = COMBAT_HEALING,  chance = 35, interval = 2000, minDamage = 95, maxDamage = 155, radius = 1, effect = 13},
	{name = "invisible",  chance = 50, interval = 3000, minDamage = 0, maxDamage = 0, radius = 1, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_ICEDAMAGE, percent = 20},
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)