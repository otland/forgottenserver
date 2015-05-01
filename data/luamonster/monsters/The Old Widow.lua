local mType = Game.createMonsterType("The Old Widow")
local monster = {}
monster.description = "the old widow"
monster.experience = 4200
monster.outfit = {
	lookType = 208
}

monster.health = 3200
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5977
monster.speed = 240
monster.maxSummons = 2

monster.changeTarget = {
    interval = 5000,
	chance = 8
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

monster.summons = {
    {name = "giant spider", chance = 13, interval = 1000}
}

monster.loot = {
    {id = "gold coin", maxCount = 99, chance = 100000},
	{id = "platinum coin", maxCount = 10, chance = 100000},
	{id = "spider silk", chance = 100000},
	{id = "steel helmet", chance = 100000},
	{id = "great health potion", maxCount = 4, chance = 100000},
	{id = "knight armor", chance = 50000},
	{id = "stealth ring", chance = 33333},
	{id = "energy ring", chance = 33333},
	{id = "time ring", chance = 33333},
	{id = "sweet smelling bait", chance = 33333},
	{id = "knight legs", chance = 25000},
	{id = "platinum amulet", chance = 25000},
	{id = "spool of yarn", chance = 25000},
	{id = "bloody edge", chance = 3225},
	{id = "dreaded cleaver", chance = 1639}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = -100, maxDamage = -500, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 1000, minDamage = -250, maxDamage = -300, range = 7, shootEffect = 15, effect = 21},
	{name = "speed",  chance = 20, interval = 1000, minDamage = 0, maxDamage = 0, duration = 25000, speedChange = -850, range = 7, shootEffect = 15, effect = 21},
	{name = "poisonfield",  chance = 10, interval = 1000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 4, shootEffect = 15, effect = 21}
}

monster.defenses = {
	defense = 21,
    armor = 17,
    {name = "combat", type = COMBAT_HEALING,  chance = 17, interval = 1000, minDamage = 225, maxDamage = 275, shootEffect = 15, effect = 13},
	{name = "speed",  chance = 8, interval = 1000, minDamage = 0, maxDamage = 0, duration = 6000, speedChange = 345, shootEffect = 15, effect = 14}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)