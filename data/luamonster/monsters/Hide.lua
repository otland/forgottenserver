local mType = Game.createMonsterType("Hide")
local monster = {}
monster.description = "hide"
monster.experience = 240
monster.outfit = {
	lookType = 219
}

monster.health = 500
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 6060
monster.speed = 280
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
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "platinum coin", maxCount = 5, chance = 100000},
	{id = "steel helmet", chance = 50000},
	{id = "spider silk", chance = 50000},
	{id = "terra hood", chance = 50000},
	{id = "knight legs", chance = 50000},
	{id = "time ring", chance = 33333}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 60, minDamage = 0, maxDamage = -140, interval = 2000, effect = 14}
}

monster.defenses = {
	defense = 40,
    armor = 25,
    {name = "speed",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 340, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 40},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_FIREDAMAGE, percent = -15},
	{type = COMBAT_ICEDAMAGE, percent = -15}
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