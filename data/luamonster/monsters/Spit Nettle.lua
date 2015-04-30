local mType = Game.createMonsterType("Spit Nettle")
local monster = {}
monster.description = "a spit nettle"
monster.experience = 20
monster.outfit = {
	lookType = 221
}

monster.health = 150
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 6062
monster.speed = 0
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 20
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "shadow herb", chance = 10650},
	{id = "nettle spit", chance = 10000},
	{id = "gold coin", maxCount = 5, chance = 10600},
	{id = "sling herb", maxCount = 2, chance = 5263},
	{id = "sling herb", chance = 1010},
	{id = "nettle blossom", chance = 833},
	{id = "sling herb", chance = 362}
}

monster.attacks = {
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 1000, minDamage = -15, maxDamage = -40, range = 7, shootEffect = 15},
	{name = "condition", type = CONDITION_POISON,  chance = 15, interval = 2000, minDamage = -40, maxDamage = -100, range = 7, shootEffect = 15}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 8, maxDamage = 16, shootEffect = 15, effect = 13}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)