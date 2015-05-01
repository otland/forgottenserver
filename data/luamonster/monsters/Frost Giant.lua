local mType = Game.createMonsterType("Frost Giant")
local monster = {}
monster.description = "a frost giant"
monster.experience = 150
monster.outfit = {
	lookType = 257
}

monster.health = 270
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7330
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
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Hmm Humansoup", yell = false},
	{text = "Stand still ya tasy snack!", yell = false},
	{text = "Joh Thun!", yell = false},
	{text = "Brore Smode!", yell = false},
	{text = "Horre Sjan Flan!", yell = false},
	{text = "Forle Bramma!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 40, chance = 82000},
	{id = "meat", maxCount = 2, chance = 4970},
	{id = "short sword", chance = 8140},
	{id = "ice cube", chance = 2180},
	{id = "battle shield", chance = 1350},
	{id = "halberd", chance = 560},
	{id = "norse shield", chance = 250},
	{id = "dark helmet", chance = 180},
	{id = "health potion", chance = 819},
	{id = "shard", chance = 60},
	{id = "club ring", chance = 130},
	{id = "frost giant pelt", chance = 5000}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 45, minDamage = 0, maxDamage = -110, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -90, range = 7, shootEffect = 12}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 300, shootEffect = 12, effect = 14}
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
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)