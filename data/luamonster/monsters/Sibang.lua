local mType = Game.createMonsterType("Sibang")
local monster = {}
monster.description = "a sibang"
monster.experience = 105
monster.outfit = {
	lookType = 118
}

monster.health = 225
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6045
monster.speed = 200
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
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 70
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Eeeeek! Eeeeek", yell = false},
	{text = "Huh! Huh! Huh!", yell = false},
	{text = "Ahhuuaaa!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 35, chance = 56000},
	{id = "banana", maxCount = 12, chance = 30000},
	{id = "coconut", maxCount = 3, chance = 1960},
	{id = "orange", maxCount = 5, chance = 19840},
	{id = "small stone", maxCount = 3, chance = 30060},
	{id = "melon", chance = 1000},
	{id = "banana sash", chance = 5000},
	{id = "ape fur", chance = 1000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 32, interval = 2000, effect = 13},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 35, interval = 2000, minDamage = 0, maxDamage = -55, range = 7, radius = 1, shootEffect = 10, effect = 13}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 380, radius = 1, shootEffect = 10, effect = 14}
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
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)