local mType = Game.createMonsterType("Cyclops Drone")
local monster = {}
monster.description = "a cyclops drone"
monster.experience = 200
monster.outfit = {
	lookType = 280
}

monster.health = 325
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7847
monster.speed = 205
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 95
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Fee! Fie! Foe! Fum!", yell = false},
	{text = "Luttl pest!", yell = false},
	{text = "Me makking you pulp!", yell = false},
	{text = "Humy tasy! Hum hum!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 30, chance = 82000},
	{id = "meat", maxCount = 2, chance = 50430},
	{id = "short sword", chance = 8000},
	{id = "plate shield", chance = 2000},
	{id = "battle shield", chance = 1600},
	{id = "halberd", chance = 680},
	{id = "cyclops trophy", chance = 120},
	{id = "dark helmet", chance = 190},
	{id = "strong health potion", chance = 520},
	{id = "club ring", chance = 90},
	{id = "cyclops toe", chance = 6750}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 60, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 35, interval = 2000, minDamage = 0, maxDamage = -80, range = 7, shootEffect = 12}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = 1},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = 20}
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