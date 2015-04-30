local mType = Game.createMonsterType("Grorlam")
local monster = {}
monster.description = "grorlam"
monster.experience = 2400
monster.outfit = {
	lookType = 205
}

monster.health = 3000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6005
monster.speed = 240
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 3
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

monster.loot = {
    {id = "gold coin", maxCount = 20, chance = 100000},
	{id = "scale armor", chance = 10000},
	{id = "worm", maxCount = 10, chance = 50000},
	{id = "stone", chance = 10000},
	{id = "small stone", maxCount = 5, chance = 20000},
	{id = "carlin sword", chance = 2500},
	{id = "torch", chance = 20000},
	{id = "fishing rod", chance = 5000}
}

monster.attacks = {
	{name = "melee",  attack = 60, skill = 75, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 1000, minDamage = -150, maxDamage = -200, range = 7, shootEffect = 12}
}

monster.defenses = {
	defense = 25,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 1000, minDamage = 100, maxDamage = 150, shootEffect = 12, effect = 13},
	{name = "speed",  chance = 6, interval = 1000, minDamage = 0, maxDamage = 0, duration = 6000, speedChange = 270, shootEffect = 12, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 30},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE, percent = 20}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)