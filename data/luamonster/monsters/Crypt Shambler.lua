local mType = Game.createMonsterType("Crypt Shambler")
local monster = {}
monster.description = "a crypt shambler"
monster.experience = 195
monster.outfit = {
	lookType = 100
}

monster.health = 330
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6029
monster.speed = 170
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = false,
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
	{text = "Aaaaahhhh!", yell = false},
	{text = "Hoooohhh!", yell = false},
	{text = "Uhhhhhhh!", yell = false},
	{text = "Chhhhhhh!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 55, chance = 57000},
	{id = "worm", maxCount = 10, chance = 9000},
	{id = "half-digested piece of meat", chance = 5000},
	{id = "bone", chance = 5000},
	{id = "iron helmet", chance = 2130},
	{id = "iron helmet", chance = 2000},
	{id = "rotten meat", chance = 1850},
	{id = "bone sword", chance = 1000},
	{id = "bone shield", chance = 1000},
	{id = "throwing star", maxCount = 3, chance = 910},
	{id = "small diamond", chance = 510}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -140, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -28, maxDamage = -55, range = 1}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = false, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)