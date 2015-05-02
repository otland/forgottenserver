local mType = Game.createMonsterType("mad mage")
local monster = {}
monster.description = "a mad mage"
monster.experience = 1800
monster.outfit = {
	lookType = 394
}

monster.health = 2200
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 13603
monster.speed = 240
monster.runHealth = 1000
monster.maxSummons = 1

monster.changeTarget = {
    interval = 5000,
	chance = 30
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 4,
	staticAttack = 90
}

monster.summons = {
    {name = "golden servant", chance = 10, interval = 1000}
}

monster.loot = {
    {id = "assassin star", maxCount = 4, chance = 2400},
	{id = "red piece of cloth", chance = 800},
	{id = "strong mana potion", maxCount = 2, chance = 1200},
	{id = "strong health potion", maxCount = 5, chance = 1300},
	{id = "gold coin", maxCount = 66, chance = 100000},
	{id = "dark mushroom", maxCount = 5, chance = 5000},
	{id = "gold coin", maxCount = 20, chance = 100000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = -45, maxDamage = -150, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 12, interval = 1000, minDamage = -50, maxDamage = -200, range = 7, shootEffect = 4},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 24, interval = 1400, minDamage = -55, maxDamage = -120, range = 6, shootEffect = 29},
	{name = "firefield",  chance = 20, interval = 1600, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 2, shootEffect = 4}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 1000, minDamage = 35, maxDamage = 80, shootEffect = 4, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -20},
	{type = COMBAT_EARTHDAMAGE, percent = 50},
	{type = COMBAT_HOLYDAMAGE, percent = -10}
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
	{type = "energy", condition = true, combat = true},
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