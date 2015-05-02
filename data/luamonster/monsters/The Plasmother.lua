local mType = Game.createMonsterType("The Plasmother")
local monster = {}
monster.description = "the plasmother"
monster.experience = 8900
monster.outfit = {
	lookType = 238
}

monster.health = 16050
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 6532
monster.speed = 310
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5500,
	chance = 10
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
    {id = "gold coin", maxCount = 70, chance = 20000},
	{id = "gold coin", maxCount = 60, chance = 20000},
	{id = "gold coin", maxCount = 45, chance = 20000},
	{id = "platinum coin", maxCount = 13, chance = 25000},
	{id = "demonic essence", chance = 45000},
	{id = "black pearl", maxCount = 3, chance = 5000},
	{id = "small sapphire", maxCount = 3, chance = 5000},
	{id = "soul orb", chance = 35000},
	{id = "the Plasmother's remains", chance = 100000}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 30, minDamage = 0, maxDamage = -100, interval = 2000, effect = 0},
	{name = "speed",  chance = 8, interval = 1000, minDamage = 0, maxDamage = 0, duration = 10000, speedChange = -800, radius = 6, effect = 21},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -200, maxDamage = -350, radius = 4, effect = 21},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 3000, minDamage = -200, maxDamage = -530, target = true, radius = 4, shootEffect = 15, effect = 17}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 75, interval = 1000, minDamage = 505, maxDamage = 605, shootEffect = 15, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 15},
	{type = COMBAT_FIREDAMAGE, percent = -20}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)