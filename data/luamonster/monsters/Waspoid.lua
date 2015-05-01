local mType = Game.createMonsterType("Waspoid")
local monster = {}
monster.description = "a waspoid"
monster.experience = 830
monster.outfit = {
	lookType = 462
}

monster.health = 1100
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 15396
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
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 95
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Peeex!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 40000},
	{id = "gold coin", maxCount = 35, chance = 50000},
	{id = "platinum coin", chance = 40430},
	{id = "waspoid wing", chance = 13890},
	{id = "waspoid claw", chance = 9096},
	{id = "compound eye", chance = 6060},
	{id = "black pearl", chance = 4230},
	{id = "emerald bangle", chance = 2040},
	{id = "yellow gem", chance = 1040},
	{id = "grasshopper legs", chance = 230},
	{id = "hive scythe", chance = 330},
	{id = "carapace shield", chance = 120}
}

monster.attacks = {
	{name = "melee",  attack = 55, skill = 80, minDamage = 0, maxDamage = -248, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -110, maxDamage = -180, target = true, radius = 3, effect = 21},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -80, maxDamage = -100, range = 7, shootEffect = 15, effect = 21}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = 25},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = 5},
	{type = COMBAT_HOLYDAMAGE, percent = -5}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)