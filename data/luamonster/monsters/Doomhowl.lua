local mType = Game.createMonsterType("Doomhowl")
local monster = {}
monster.description = "doomhowl"
monster.experience = 3750
monster.outfit = {
	lookType = 308
}

monster.health = 8500
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6080
monster.speed = 320
monster.runHealth = 100
monster.maxSummons = 0

monster.changeTarget = {
    interval = 0,
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

monster.attacks = {
	{name = "melee",  attack = 100, skill = 70, minDamage = 0, maxDamage = -400, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 50, interval = 2000, minDamage = 0, maxDamage = -645, radius = 3, effect = 1},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 4000, minDamage = 0, maxDamage = 0, effect = 20}
}

monster.defenses = {
	defense = 55,
    armor = 50,
    {name = "speed",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, duration = 6000, speedChange = 390, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 15}
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)