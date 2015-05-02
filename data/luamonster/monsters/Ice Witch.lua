local mType = Game.createMonsterType("Ice Witch")
local monster = {}
monster.description = "an ice witch"
monster.experience = 580
monster.outfit = {
	lookFeet = 105,
	lookLegs = 105,
	lookType = 149,
	lookBody = 47
}

monster.health = 650
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20423
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
	{text = "So you think you are cool?", yell = false},
	{text = "I hope it is not too cold for you! HeHeHe.", yell = false},
	{text = "Freeze!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 90, chance = 29630},
	{id = "ice cube", chance = 10000},
	{id = "green mushroom", chance = 1310},
	{id = "glacier shoes", chance = 280},
	{id = "diamond sceptre", chance = 330},
	{id = "crystal sword", chance = 400},
	{id = "shard", chance = 530},
	{id = "clerical mace", chance = 920},
	{id = "strong mana potion", chance = 820},
	{id = "mystic turban", chance = 430},
	{id = "pair of earmuffs", chance = 90}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 30, minDamage = 0, maxDamage = -60, interval = 2000, effect = 0},
	{name = "outfit",  chance = 1, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, shootEffect = 13, effect = 13},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 10, interval = 2000, minDamage = -60, maxDamage = -130, length = 5, spread = 2, shootEffect = 13, effect = 43},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 20, interval = 2000, minDamage = -55, maxDamage = -115, range = 7, shootEffect = 37, effect = 44},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 20000, speedChange = -600, range = 7, shootEffect = 37, effect = 43}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 2000, minDamage = 90, maxDamage = 120, shootEffect = 37, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 40},
	{type = COMBAT_FIREDAMAGE, percent = 50},
	{type = COMBAT_DEATHDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = 30}
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