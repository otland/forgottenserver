local mType = Game.createMonsterType("Thul")
local monster = {}
monster.description = "a thul"
monster.experience = 2700
monster.outfit = {
	lookType = 46
}

monster.health = 2950
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6065
monster.speed = 520
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
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.summons = {
    {name = "Massive Water Elementals", chance = 10, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Gaaahhh!", yell = false}
}

monster.loot = {
    {id = "fish fin", chance = 100000},
	{id = "gold coin", maxCount = 77, chance = 50000},
	{id = "relic sword", chance = 50000},
	{id = "shrimp", chance = 25000}
}

monster.attacks = {
	{name = "melee",  attack = 285, skill = 85, minDamage = 0, maxDamage = -1354, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 25, interval = 2000, minDamage = 0, maxDamage = -170, radius = 3, effect = 10},
	{name = "poisonfield",  chance = 19, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, radius = 3, shootEffect = 15, effect = 10},
	{name = "speed",  chance = 18, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = -360, range = 7, shootEffect = 13, effect = 10},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 20, interval = 2000, minDamage = -108, maxDamage = -137, range = 7, shootEffect = 29, effect = 42}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -15}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)