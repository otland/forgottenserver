local mType = Game.createMonsterType("Mutated Bat")
local monster = {}
monster.description = "a mutated bat"
monster.experience = 615
monster.outfit = {
	lookType = 307
}

monster.health = 900
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 9829
monster.speed = 210
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

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Shriiiiiek", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 50, chance = 53000},
	{id = "gold coin", maxCount = 70, chance = 40000},
	{id = "bat wing", maxCount = 2, chance = 4900},
	{id = "battle shield", chance = 7760},
	{id = "black pearl", maxCount = 3, chance = 720},
	{id = "black shield", chance = 70},
	{id = "energy ring", chance = 990},
	{id = "star herb", chance = 7260},
	{id = "rusty armor", maxCount = 2, chance = 12530},
	{id = "rusty armor", chance = 12530},
	{id = "small amethyst", maxCount = 2, chance = 500},
	{id = "star herb", chance = 5060},
	{id = "mercenary sword", chance = 110},
	{id = "batwing hat", chance = 80},
	{id = "mutated bat ear", chance = 4900}
}

monster.attacks = {
	{name = "melee",  attack = 56, skill = 50, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -70, maxDamage = -180, range = 7, shootEffect = 15},
	{name = "combat", type = COMBAT_DROWNDAMAGE,  chance = 15, interval = 2000, minDamage = -30, maxDamage = -90, radius = 6, shootEffect = 15, effect = 25},
	{name = "mutated bat curse",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, radius = 6, shootEffect = 15, effect = 25},
	{name = "condition", type = CONDITION_POISON,  chance = 15, interval = 2000, minDamage = -190, maxDamage = -240, length = 4, spread = 3, radius = 6, shootEffect = 15, effect = 21}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 80, maxDamage = 95, radius = 6, shootEffect = 15, effect = 13}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)