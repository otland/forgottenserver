local mType = Game.createMonsterType("Necromancer Servant")
local monster = {}
monster.description = "a necromancer servant"
monster.experience = 580
monster.outfit = {
	lookType = 9
}

monster.health = 580
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20455
monster.speed = 210
monster.runHealth = 10
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 8
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Blib!", yell = false},
	{text = "Blub!", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 20, minDamage = 0, maxDamage = -45, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 1000, minDamage = -5, maxDamage = -40, range = 7, shootEffect = 9}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -8},
	{type = COMBAT_DEATHDAMAGE, percent = -7}
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