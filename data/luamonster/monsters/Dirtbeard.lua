local mType = Game.createMonsterType("Dirtbeard")
local monster = {}
monster.description = "a dirtbeard"
monster.experience = 375
monster.outfit = {
	lookType = 98
}

monster.health = 630
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 10320
monster.speed = 300
monsters.runHealth = 50
monster.maxSummons = 2

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
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.summons = {
    {name = "pirate marauder", chance = 30, interval = 4000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "You are no match for the scourge of the seas!", yell = false},
	{text = "You move like a seasick whale!", yell = false},
	{text = "Yarr, death to all landlubbers!", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -125, interval = 1200, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 30, interval = 2000, minDamage = 0, maxDamage = -100, range = 7, shootEffect = 8},
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = 0, interval = 1200, effect = 20}
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