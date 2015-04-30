local mType = Game.createMonsterType("Pirate Skeleton")
local monster = {}
monster.description = "a pirate skeleton"
monster.experience = 85
monster.outfit = {
	lookType = 195
}

monster.health = 190
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6070
monster.speed = 180
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 25, chance = 48000},
	{id = "big bone", chance = 5140},
	{id = "bone", chance = 4250},
	{id = "short sword", chance = 1003},
	{id = "sword", chance = 550},
	{id = "bone club", chance = 960},
	{id = "skull", chance = 4460},
	{id = "spooky blue eye", chance = 4730}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 40, interval = 2000, effect = 0}
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