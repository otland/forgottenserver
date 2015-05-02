local mType = Game.createMonsterType("Shark")
local monster = {}
monster.description = "a shark"
monster.experience = 700
monster.outfit = {
	lookType = 453
}

monster.health = 1200
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 15287
monster.speed = 230
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
	{text = "Rarr chomp chomp!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 38, chance = 40000},
	{id = "fish", maxCount = 4, chance = 25000},
	{id = "deepling scales", chance = 9090},
	{id = "eye of a deepling", chance = 1270},
	{id = "small sapphire", chance = 1222},
	{id = "giant shimmering pearl", chance = 550},
	{id = "fish fin", chance = 161}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 60, minDamage = 0, maxDamage = -175, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 1},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 1},
	{type = COMBAT_ICEDAMAGE, percent = 1}
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
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)