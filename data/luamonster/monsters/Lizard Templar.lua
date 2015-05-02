local mType = Game.createMonsterType("Lizard Templar")
local monster = {}
monster.description = "a lizard templar"
monster.experience = 155
monster.outfit = {
	lookType = 113
}

monster.health = 410
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 4251
monster.speed = 200
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Hissss!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 60, chance = 84000},
	{id = "short sword", chance = 9500},
	{id = "steel helmet", chance = 2000},
	{id = "sword", chance = 4000},
	{id = "morning star", chance = 1990},
	{id = "plate armor", chance = 1000},
	{id = "lizard leather", chance = 880},
	{id = "templar scytheblade", chance = 500},
	{id = "small emerald", chance = 250},
	{id = "health potion", chance = 890},
	{id = "lizard scale", chance = 990},
	{id = "salamander shield", chance = 110}
}

monster.attacks = {
	{name = "melee",  attack = 35, skill = 30, minDamage = 0, maxDamage = -70, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = 10},
	{type = COMBAT_FIREDAMAGE, percent = -10}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)