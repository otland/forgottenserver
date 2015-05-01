local mType = Game.createMonsterType("Ice Overlord")
local monster = {}
monster.description = "an ice overlord"
monster.experience = 1950
monster.outfit = {
	lookType = 11
}

monster.health = 4000
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 8965
monster.speed = 390
monsters.runHealth = 1
monster.maxSummons = 0

monster.changeTarget = {
    interval = 20000,
	chance = 15
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
	staticAttack = 85
}

monster.loot = {
    {id = "flawless ice crystal", chance = 100000},
	{id = "gold coin", maxCount = 38, chance = 50000},
	{id = "platinum coin", maxCount = 3, chance = 50000}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 70, minDamage = 0, maxDamage = -400, interval = 2000, effect = 0},
	{name = "speed",  chance = 18, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = -800, radius = 6, effect = 43},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 9, interval = 1000, minDamage = -50, maxDamage = -400, range = 7, shootEffect = 37, effect = 44}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 90, maxDamage = 150, shootEffect = 37, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 50},
	{type = COMBAT_ENERGYDAMAGE, percent = -25}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)