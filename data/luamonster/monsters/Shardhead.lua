local mType = Game.createMonsterType("Shardhead")
local monster = {}
monster.description = "shardhead"
monster.experience = 650
monster.outfit = {
	lookType = 261
}

monster.health = 800
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 7282
monster.speed = 195
monster.maxSummons = 0

monster.changeTarget = {
    interval = 2000,
	chance = 5
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
	staticAttack = 50
}

monster.voices = {
	interval = 2000,
	chance = 5,
	{text = "Chrr.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 87, chance = 99700},
	{id = "shard", chance = 40000},
	{id = "strong health potion", chance = 100000},
	{id = "frosty heart", chance = 40000},
	{id = "ice cube", chance = 80000},
	{id = "crystal sword", chance = 30000}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 100, minDamage = 0, maxDamage = -275, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -90, range = 7, shootEffect = 29, effect = 44},
	{name = "speed",  chance = 12, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = -360, length = 8, shootEffect = 29, effect = 42}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 25},
	{type = COMBAT_ENERGYDAMAGE, percent = -20}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = true, combat = true},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)