local mType = Game.createMonsterType("Ice Golem")
local monster = {}
monster.description = "an ice golem"
monster.experience = 295
monster.outfit = {
	lookType = 261
}

monster.health = 385
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
	{text = "Chrrr.", yell = false},
	{text = "Crrrrk.", yell = false},
	{text = "Gnarr.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 3, chance = 50000},
	{id = "frosty heart", chance = 11111},
	{id = "ice cube", chance = 5000},
	{id = "black pearl", chance = 1612},
	{id = "small sapphire", chance = 578},
	{id = "strange helmet", chance = 444},
	{id = "strong health potion", chance = 444},
	{id = "ice rapier", subType = 1, chance = 400},
	{id = "shard", chance = 266},
	{id = "crystal sword", chance = 177},
	{id = "glacier mask", chance = 111},
	{id = "small diamond", chance = 66}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 100, interval = 2000, effect = 0},
	{name = "speed",  chance = 13, interval = 1000, minDamage = 0, maxDamage = 0, duration = 20000, speedChange = -800, length = 8, effect = 12},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 15, interval = 1000, minDamage = -50, maxDamage = -85, range = 7, shootEffect = 37, effect = 44},
	{name = "ice golem skill reducer",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 37, effect = 44}
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