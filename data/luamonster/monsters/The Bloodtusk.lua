local mType = Game.createMonsterType("The Bloodtusk")
local monster = {}
monster.description = "the bloodtuskh"
monster.experience = 300
monster.outfit = {
	lookType = 199
}

monster.health = 600
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6074
monster.speed = 190
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
    {id = "platinum coin", maxCount = 3, chance = 83825},
	{id = "gold coin", maxCount = 65, chance = 66900},
	{id = "tusk shield", chance = 83825},
	{id = "orc tusk", maxCount = 2, chance = 58825},
	{id = "mammoth fur cape", chance = 42175},
	{id = "furry club", chance = 50500},
	{id = "mammoth tusk", maxCount = 2, chance = 38000}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 50, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 15},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_ICEDAMAGE, percent = 20}
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)