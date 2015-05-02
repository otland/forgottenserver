local mType = Game.createMonsterType("Corym Skirmisher")
local monster = {}
monster.description = "a corym skirmisher"
monster.experience = 260
monster.outfit = {
	lookLegs = 101,
	lookType = 533
}

monster.health = 450
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 19726
monster.speed = 200
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
	{text = "Squeak! Squeak!", yell = false},
	{text = "<sniff><sniff><sniff>", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 45, chance = 50000},
	{id = "cheese", chance = 20000},
	{id = "rat cheese", chance = 16666},
	{id = "soft cheese", chance = 14285},
	{id = "earflap", chance = 9090},
	{id = "cheese cutter", chance = 8333},
	{id = "bola", chance = 7692},
	{id = "life preserver", chance = 2941},
	{id = "ratana", chance = 2702},
	{id = "cheesy figurine", chance = 854},
	{id = "spike shield", chance = 645},
	{id = "leather harness", chance = 561},
	{id = "rat god doll", chance = 12}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 45, minDamage = 0, maxDamage = -110, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -90, range = 7, shootEffect = 27}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_DEATHDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE, percent = -20}
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