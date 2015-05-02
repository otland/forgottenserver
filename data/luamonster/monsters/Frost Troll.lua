local mType = Game.createMonsterType("Frost Troll")
local monster = {}
monster.description = "a frost troll"
monster.experience = 23
monster.outfit = {
	lookType = 53
}

monster.health = 55
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5998
monster.speed = 190
monster.runHealth = 10
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Brrrr", yell = false},
	{text = "Broar!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 12, chance = 50840},
	{id = "spear", chance = 21500},
	{id = "fish", chance = 18000},
	{id = "wooden shield", chance = 15850},
	{id = "rapier", chance = 15500},
	{id = "twigs", chance = 8300},
	{id = "frosty ear of a troll", chance = 2000},
	{id = "coat", chance = 1200}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -20, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 50},
	{type = COMBAT_DEATHDAMAGE, percent = -15},
	{type = COMBAT_HOLYDAMAGE, percent = 10}
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
	{type = "energy", condition = false, combat = false},
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