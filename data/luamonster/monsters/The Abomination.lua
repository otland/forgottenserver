local mType = Game.createMonsterType("The Abomination")
local monster = {}
monster.description = "the abomination"
monster.experience = 25000
monster.outfit = {
	lookType = 238
}

monster.health = 38050
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 6532
monster.speed = 340
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

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Blubb", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "platinum coin", maxCount = 3, chance = 10000},
	{id = "demonic essence", chance = 2857},
	{id = "soul orb", chance = 2500}
}

monster.attacks = {
	{name = "melee",  attack = 120, skill = 90, minDamage = 0, maxDamage = -600, interval = 2000, effect = 0},
	{name = "speed",  chance = 12, interval = 1000, minDamage = 0, maxDamage = 0, duration = 10000, speedChange = -800, radius = 6, effect = 21},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 9, interval = 1000, minDamage = -200, maxDamage = -650, radius = 4, effect = 21},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 11, interval = 1000, minDamage = -400, maxDamage = -900, target = true, radius = 4, shootEffect = 15, effect = 19},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 19, interval = 2000, minDamage = -350, maxDamage = -850, length = 7, spread = 3, shootEffect = 15, effect = 19}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 75, interval = 1000, minDamage = 505, maxDamage = 605, shootEffect = 15, effect = 13}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)