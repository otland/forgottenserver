local mType = Game.createMonsterType("Swamp Troll")
local monster = {}
monster.description = "a swamp troll"
monster.experience = 25
monster.outfit = {
	lookType = 76
}

monster.health = 55
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 6018
monster.speed = 170
monsters.runHealth = 15
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 0
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Grrrr", yell = false},
	{text = "Groar!", yell = false},
	{text = "Me strong! Me ate spinach!", yell = false}
}

monster.loot = {
    {id = "fish", chance = 60000},
	{id = "gold coin", maxCount = 5, chance = 50300},
	{id = "torch", chance = 15000},
	{id = "spear", chance = 13000},
	{id = "mouldy cheese", chance = 10000},
	{id = "leather boots", chance = 9500},
	{id = "swamp grass", chance = 3100},
	{id = "medicine pouch", chance = 2160},
	{id = "wood", chance = 2140},
	{id = "troll green", chance = 1200},
	{id = "fishing rod", chance = 60}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -13, interval = 2000, effect = 0}
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