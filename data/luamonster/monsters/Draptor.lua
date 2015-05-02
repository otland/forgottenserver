local mType = Game.createMonsterType("Draptor")
local monster = {}
monster.description = "a draptor"
monster.experience = 2400
monster.outfit = {
	lookType = 382
}

monster.health = 3000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 13316
monster.speed = 282
monster.runHealth = 350
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 8
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 90, chance = 33750},
	{id = "gold coin", maxCount = 60, chance = 33750},
	{id = "dragon robe", chance = 950},
	{id = "draptor scales", chance = 6650},
	{id = "strong health potion", chance = 3150},
	{id = "strong mana potion", chance = 4150}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = -1, maxDamage = -150, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 30, interval = 3000, minDamage = -130, maxDamage = -310, radius = 3, effect = 49},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 35, interval = 3000, minDamage = -200, maxDamage = -300, range = 7, shootEffect = 5, effect = 49},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 17, interval = 2500, minDamage = -70, maxDamage = -250, length = 8, spread = 3, shootEffect = 5, effect = 7}
}

monster.defenses = {
	defense = 37,
    armor = 23,
    {name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 1000, minDamage = 57, maxDamage = 93, shootEffect = 5, effect = 13},
	{name = "speed",  chance = 12, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 457, shootEffect = 5, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = -10}
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