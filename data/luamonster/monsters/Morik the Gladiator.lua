local mType = Game.createMonsterType("Morik the Gladiator")
local monster = {}
monster.description = "a gladiator"
monster.experience = 160
monster.outfit = {
	lookFeet = 57,
	lookLegs = 57,
	lookHead = 57,
	lookType = 131,
	lookAddons = 1,
	lookBody = 57
}

monster.health = 1235
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20531
monster.speed = 200
monster.runHealth = 10
monster.maxSummons = 2

monster.changeTarget = {
    interval = 2000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = false,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.summons = {
    {name = "gladiator", chance = 10, interval = 2000}
}

monster.voices = {
	interval = 4000,
	chance = 10,
	{text = "To be the one you'll have to beat the one!", yell = false},
	{text = "Where did I put my ultimate health potion again?", yell = false},
	{text = "I am the best!", yell = false},
	{text = "I'll take your ears as a trophy!", yell = false}
}

monster.loot = {
    {id = "Morik's helmet", chance = 100000}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 90, minDamage = 0, maxDamage = -250, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -110, radius = 3, effect = 10},
	{name = "drunk",  chance = 10, interval = 1000, minDamage = 0, maxDamage = 0, effect = 10}
}

monster.defenses = {
	defense = 22,
    armor = 20,
    {name = "speed",  chance = 9, interval = 1000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 215, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 15},
	{type = COMBAT_DEATHDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = 10}
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