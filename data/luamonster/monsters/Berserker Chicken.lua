local mType = Game.createMonsterType("Berserker Chicken")
local monster = {}
monster.description = "a berserker chicken"
monster.experience = 220
monster.outfit = {
	lookType = 111
}

monster.health = 465
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6042
monster.speed = 300
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Gokgoooook", yell = false},
	{text = "Cluck Cluck", yell = false},
	{text = "I will fill MY cushion with YOUR hair! CLUCK!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 1200, effect = 0},
	{name = "combat", type = COMBAT_DROWNDAMAGE,  chance = 15, interval = 2000, minDamage = -41, maxDamage = -70, effect = 5},
	{name = "Blood Rage",  chance = 30, interval = 2000, minDamage = 0, maxDamage = 0, effect = 5}
}

monster.defenses = {
	defense = 35,
    armor = 30,
    {name = "speed",  chance = 40, interval = 1000, minDamage = 0, maxDamage = 0, duration = 8000, speedChange = 400, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -20},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = 10},
	{type = COMBAT_FIREDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE, percent = 10},
	{type = COMBAT_DEATHDAMAGE, percent = 10},
	{type = COMBAT_ICEDAMAGE, percent = 10}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
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