local mType = Game.createMonsterType("Dragon Lord Hatchling")
local monster = {}
monster.description = "a dragon lord hatchling"
monster.experience = 645
monster.outfit = {
	lookType = 272
}

monster.health = 750
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7622
monster.speed = 190
monsters.runHealth = 30
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
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Fchu?", yell = false},
	{text = "Rooawwrr", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 90, chance = 33750},
	{id = "gold coin", maxCount = 75, chance = 33750},
	{id = "dragon ham", chance = 80000},
	{id = "mana potion", chance = 300},
	{id = "green mushroom", chance = 560},
	{id = "magma boots", chance = 100}
}

monster.attacks = {
	{name = "melee",  attack = 45, skill = 30, minDamage = 0, maxDamage = -90, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 2000, minDamage = -90, maxDamage = -125, length = 5, spread = 2, effect = 7},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = -55, maxDamage = -105, target = true, range = 7, radius = 4, shootEffect = 4, effect = 7},
	{name = "firefield",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 3, shootEffect = 4, effect = 7}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 26, maxDamage = 48, shootEffect = 4, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 10},
	{type = COMBAT_ICEDAMAGE, percent = -10}
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)