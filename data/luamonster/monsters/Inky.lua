local mType = Game.createMonsterType("Inky")
local monster = {}
monster.description = "a inky"
monster.experience = 250
monster.outfit = {
	lookType = 46
}

monster.health = 600
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6065
monster.speed = 520
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Tssss!", yell = false},
	{text = "Gaaahhh!", yell = false},
	{text = "Gluh! Gluh!", yell = false},
	{text = "Boohaa!", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 156, skill = 85, interval = 2000, effect = 14},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -87, radius = 3, shootEffect = 4, effect = 10},
	{name = "poisonfield",  chance = 19, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, radius = 3, shootEffect = 15, effect = 10},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 7, interval = 2000, minDamage = -56, maxDamage = -87, range = 7, radius = 3, shootEffect = 13, effect = 10},
	{name = "condition", type = CONDITION_ENERGY,  chance = 10, interval = 2000, minDamage = -75, maxDamage = -75, range = 1, radius = 3, shootEffect = 5, effect = 10}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -15}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)