local mType = Game.createMonsterType("Haunted Treeling")
local monster = {}
monster.description = "a haunted treeling"
monster.experience = 310
monster.outfit = {
	lookType = 310
}

monster.health = 450
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 9867
monster.speed = 220
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
	staticAttack = 85
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Knarrrz", yell = false},
	{text = "Huuhuuhuuuhuuaarrr", yell = false},
	{text = "Knorrrrrr", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 95, chance = 60000},
	{id = "wooden trash", chance = 25000},
	{id = "red mushroom", chance = 7142},
	{id = "white mushroom", maxCount = 2, chance = 5263},
	{id = "haunted piece of wood", chance = 5000},
	{id = "health potion", chance = 5000},
	{id = "orange mushroom", chance = 1851},
	{id = "strong health potion", chance = 900},
	{id = "dwarven ring", chance = 621},
	{id = "small sapphire", chance = 585},
	{id = "bullseye potion", chance = 80}
}

monster.attacks = {
	{name = "melee",  attack = 60, skill = 40, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 5, interval = 2000, minDamage = -30, maxDamage = -100, radius = 4, effect = 9},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 15000, speedChange = -700, length = 5, radius = 4, effect = 46},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -100, target = true, range = 7, radius = 1, shootEffect = 39, effect = 47},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = -55, maxDamage = -100, target = true, radius = 4, shootEffect = 39, effect = 17},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, radius = 1, shootEffect = 39, effect = 21}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)