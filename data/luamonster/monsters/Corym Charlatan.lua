local mType = Game.createMonsterType("Corym Charlatan")
local monster = {}
monster.description = "a corym charlatan"
monster.experience = 150
monster.outfit = {
	lookLegs = 116,
	lookType = 532,
	lookBody = 97
}

monster.health = 250
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 19725
monster.speed = 180
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Mehehe!", yell = false},
	{text = "Beware! Me hexing you!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 35, chance = 50000},
	{id = "rat cheese", chance = 14285},
	{id = "soft cheese", chance = 12500},
	{id = "cheese", chance = 10000},
	{id = "bola", chance = 7692},
	{id = "earflap", chance = 7142},
	{id = "cheese cutter", chance = 5882},
	{id = "ratana", chance = 909},
	{id = "life preserver", chance = 854},
	{id = "leather harness", chance = 537},
	{id = "cheesy figurine", chance = 518},
	{id = "spike shield", chance = 492}
}

monster.attacks = {
	{name = "melee",  attack = 35, skill = 50, minDamage = 0, maxDamage = -105, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 25},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_DEATHDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE, percent = -20}
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