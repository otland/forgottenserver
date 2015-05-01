local mType = Game.createMonsterType("Energy Overlord")
local monster = {}
monster.description = "an energy overlord"
monster.experience = 2800
monster.outfit = {
	lookType = 290
}

monster.health = 4000
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 8966
monster.speed = 290
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
    {id = "gold coin", maxCount = 64, chance = 100000},
	{id = "pure energy", chance = 100000},
	{id = "platinum coin", maxCount = 2, chance = 25000}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 40, minDamage = 0, maxDamage = -200, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 25, interval = 2000, minDamage = 0, maxDamage = -800, length = 7, effect = 48},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 11, interval = 1000, minDamage = 0, maxDamage = -200, range = 3, effect = 48},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 9, interval = 1000, minDamage = -50, maxDamage = -200, radius = 5, effect = 51}
}

monster.defenses = {
	defense = 40,
    armor = 40,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 90, maxDamage = 150, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 50},
	{type = COMBAT_FIREDAMAGE, percent = -1}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = false, combat = false},
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
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)