local mType = Game.createMonsterType("Zulazza the Corruptor")
local monster = {}
monster.description = "a zulazza the corruptor"
monster.experience = 9800
monster.outfit = {
	lookType = 334
}

monster.health = 28000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 11107
monster.speed = 290
monster.runHealth = 1500
monster.maxSummons = 0

monster.changeTarget = {
    interval = 2000,
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
	staticAttack = 80
}

monster.voices = {
	interval = 2000,
	chance = 10,
	{text = "I'm Zulazza, and you won't forget me that fazzt.", yell = false},
	{text = "Oh, HE will take revenge on zzizz azzault when you zztep in front of HIZZ fazze!", yell = false}
}

monster.loot = {
    {id = "great health potion", chance = 30500},
	{id = "platinum coin", maxCount = 30, chance = 41325},
	{id = "gold coin", maxCount = 100, chance = 49650},
	{id = "rusty armor", chance = 50500},
	{id = "gold ingot", maxCount = 4, chance = 33000},
	{id = "blue gem", chance = 30500},
	{id = "green gem", chance = 20500},
	{id = "ultimate health potion", chance = 10500},
	{id = "dragon scale boots", chance = 5500},
	{id = "soul orb", maxCount = 4, chance = 19250},
	{id = "tower shield", chance = 15500},
	{id = "viper star", maxCount = 67, chance = 8100},
	{id = "giant shimmering pearl", maxCount = 2, chance = 28000},
	{id = "yellow gem", chance = 15500},
	{id = "red gem", chance = 10500},
	{id = "mastermind potion", chance = 10500},
	{id = "violet gem", chance = 25500},
	{id = "great mana potion", chance = 20500},
	{id = "earthborn titan armor", chance = 5500},
	{id = "mastermind shield", chance = 5500},
	{id = "emerald bangle", chance = 10500},
	{id = "paladin armor", chance = 5500},
	{id = "guardian shield", chance = 5500}
}

monster.attacks = {
	{name = "melee",  attack = 200, skill = 200, minDamage = 0, maxDamage = -2100, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 40, interval = 2000, minDamage = -500, maxDamage = -800, length = 8, effect = 18},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 30, interval = 2000, minDamage = -300, maxDamage = -800, radius = 3, effect = 21},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 25, interval = 2000, minDamage = -50, maxDamage = -130, range = 7, effect = 15},
	{name = "speed",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, duration = 20000, speedChange = -500, range = 7, effect = 14}
}

monster.defenses = {
	defense = 119,
    armor = 96,
    {name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 2000, minDamage = 2000, maxDamage = 3000, effect = 13}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = true, combat = true},
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