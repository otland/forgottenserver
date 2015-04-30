local mType = Game.createMonsterType("Terror Bird")
local monster = {}
monster.description = "a terror bird"
monster.experience = 150
monster.outfit = {
	lookType = 218
}

monster.health = 300
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6057
monster.speed = 220
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "CRAAAHHH!", yell = false},
	{text = "Gruuuh Gruuuh.", yell = false},
	{text = "Carrah Carrah!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 30, chance = 81500},
	{id = "worm", maxCount = 3, chance = 10000},
	{id = "gold coin", maxCount = 3, chance = 48000},
	{id = "terrorbird beak", chance = 11000},
	{id = "colourful feather", chance = 3000},
	{id = "feather headdress", chance = 630},
	{id = "feather headdress", chance = 150},
	{id = "feather headdress", chance = 260}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 35, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = 20}
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