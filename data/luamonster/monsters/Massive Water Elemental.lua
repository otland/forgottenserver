local mType = Game.createMonsterType("Massive Water Elemental")
local monster = {}
monster.description = "a massive water elemental"
monster.experience = 1100
monster.outfit = {
	lookType = 11
}

monster.health = 1250
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 10499
monster.speed = 280
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

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "platinum coin", maxCount = 2, chance = 20000},
	{id = "fish", maxCount = 2, chance = 40000},
	{id = "small diamond", maxCount = 2, chance = 1900},
	{id = "small emerald", maxCount = 2, chance = 1000},
	{id = "great health potion", chance = 10000},
	{id = "great mana potion", chance = 10400},
	{id = "energy ring", chance = 910},
	{id = "life ring", chance = 1000},
	{id = "rainbow trout", chance = 1340},
	{id = "green perch", chance = 1590}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 45, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DROWNDAMAGE,  chance = 15, interval = 2000, minDamage = -330, maxDamage = -450, target = true, range = 7, radius = 2, effect = 2},
	{name = "icicle",  chance = 20, interval = 2000, minDamage = -170, maxDamage = -210, range = 7, radius = 2, effect = 2},
	{name = "condition", type = CONDITION_POISON,  chance = 20, interval = 2000, minDamage = -355, maxDamage = -420, radius = 5, effect = 21}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 5, interval = 2000, minDamage = 120, maxDamage = 250, radius = 5, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 30},
	{type = COMBAT_ENERGYDAMAGE, percent = -25},
	{type = COMBAT_DEATHDAMAGE, percent = 50},
	{type = COMBAT_HOLYDAMAGE, percent = 30}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)