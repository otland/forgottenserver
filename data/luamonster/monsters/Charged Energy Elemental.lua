local mType = Game.createMonsterType("Charged Energy Elemental")
local monster = {}
monster.description = "a charged energy elemental"
monster.experience = 450
monster.outfit = {
	lookType = 293
}

monster.health = 500
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 8966
monster.speed = 270
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
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 22, chance = 50000},
	{id = "flash arrow", maxCount = 3, chance = 6250},
	{id = "energy soil", chance = 2063}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 30, minDamage = 0, maxDamage = -100, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 20, interval = 2000, minDamage = -100, maxDamage = -168, target = true, range = 6, radius = 4, shootEffect = 5, effect = 48},
	{name = "condition", type = CONDITION_ENERGY,  chance = 15, interval = 1000, minDamage = 0, maxDamage = 0, radius = 3, shootEffect = 5, effect = 49}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 90, maxDamage = 150, shootEffect = 5, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5}
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)