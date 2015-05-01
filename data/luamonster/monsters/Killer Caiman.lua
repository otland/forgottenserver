local mType = Game.createMonsterType("Killer Caiman")
local monster = {}
monster.description = "a killer caiman"
monster.experience = 900
monster.outfit = {
	lookType = 358
}

monster.health = 1500
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 11430
monster.speed = 230
monster.runHealth = 10
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
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

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 80, chance = 50000},
	{id = "piece of crocodile leather", chance = 25430},
	{id = "ham", chance = 40100},
	{id = "giant shimmering pearl", chance = 1130},
	{id = "bunch of ripe rice", maxCount = 2, chance = 4800},
	{id = "small emerald", maxCount = 5, chance = 10150},
	{id = "crocodile boots", chance = 510},
	{id = "obsidian lance", chance = 4975}
}

monster.attacks = {
	{name = "melee",  attack = 60, skill = 50, minDamage = 0, maxDamage = -180, interval = 2000, effect = 0}
}

monster.defenses = {
	defense = 35,
    armor = 35,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 700, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 5},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = 10}
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