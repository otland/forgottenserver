local mType = Game.createMonsterType("Mercury Blob")
local monster = {}
monster.description = "a mercury blob"
monster.experience = 180
monster.outfit = {
	lookType = 316
}

monster.health = 150
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 9961
monster.speed = 180
monster.maxSummons = 3

monster.changeTarget = {
    interval = 5000,
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
	staticAttack = 85
}

monster.summons = {
    {name = "Mercury Blob", chance = 10, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Crackle", yell = false}
}

monster.loot = {
    {id = "glob of mercury", chance = 17966}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 40, minDamage = 0, maxDamage = -75, interval = 2000, effect = 0},
	{name = "drunk",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, shootEffect = 31, effect = 32},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 20, interval = 2000, minDamage = -10, maxDamage = -30, range = 7, shootEffect = 5, effect = 12}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 5, interval = 2000, minDamage = 20, maxDamage = 30, shootEffect = 5, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 5},
	{type = COMBAT_EARTHDAMAGE, percent = 65},
	{type = COMBAT_FIREDAMAGE, percent = 10},
	{type = COMBAT_ICEDAMAGE, percent = 15},
	{type = COMBAT_HOLYDAMAGE, percent = 65},
	{type = COMBAT_LIFEDRAIN, percent = 80}
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)