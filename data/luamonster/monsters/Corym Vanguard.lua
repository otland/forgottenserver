local mType = Game.createMonsterType("Corym Vanguard")
local monster = {}
monster.description = "a corym vanguard"
monster.experience = 490
monster.outfit = {
	lookLegs = 101,
	lookHead = 101,
	lookType = 534
}

monster.health = 700
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 19734
monster.speed = 210
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
	staticAttack = 70
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Gimme! Gimme!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 120, chance = 100000},
	{id = "cheese", chance = 20000},
	{id = "cheese cutter", chance = 16666},
	{id = "rat cheese", chance = 14285},
	{id = "soft cheese", chance = 14285},
	{id = "earflap", chance = 11111},
	{id = "bola", chance = 10000},
	{id = "life preserver", chance = 5000},
	{id = "ratana", chance = 5000},
	{id = "spike shield", chance = 4761},
	{id = "cheesy figurine", chance = 3846},
	{id = "spiky club", chance = 3225},
	{id = "leather harness", chance = 1612},
	{id = "rat god doll", chance = 14}
}

monster.attacks = {
	{name = "melee",  attack = 56, skill = 40, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = -50, maxDamage = -100, length = 5, spread = 3, effect = 9},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 15, interval = 2000, minDamage = -40, maxDamage = -70, target = true, radius = 4, effect = 18}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 30, maxDamage = 60, radius = 4, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
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