local mType = Game.createMonsterType("Webster")
local monster = {}
monster.description = "a webster"
monster.experience = 1200
monster.outfit = {
	lookType = 263
}

monster.health = 1750
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 7349
monster.speed = 290
monster.maxSummons = 0

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
	targetDistance = 1,
	staticAttack = 95
}

monster.voices = {
	interval = 2000,
	chance = 5,
	{text = "You are lost!", yell = false},
	{text = "Come my little morsel.", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 40, interval = 2000, effect = 0},
	{name = "speed",  chance = 65, interval = 3500, minDamage = 0, maxDamage = 0, target = true, duration = 40, speedChange = -500, range = 1, radius = 1, effect = 14},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 75, interval = 3000, minDamage = -13, maxDamage = -80, range = 7, radius = 1, shootEffect = 5, effect = 14}
}

monster.defenses = {
	defense = 34,
    armor = 29,
    {name = "speed",  chance = 100, interval = 5000, minDamage = 0, maxDamage = 0, duration = 2500, speedChange = 500, radius = 1, shootEffect = 5, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -20}
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
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)