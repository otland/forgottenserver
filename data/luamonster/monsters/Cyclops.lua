local mType = Game.createMonsterType("Cyclops")
local monster = {}
monster.description = "a cyclops"
monster.experience = 150
monster.outfit = {
	lookType = 22
}

monster.health = 260
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5962
monster.speed = 200
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Il lorstok human!", yell = false},
	{text = "Toks utat.", yell = false},
	{text = "Human, uh whil dyh!", yell = false},
	{text = "Youh ah trak!", yell = false},
	{text = "Let da mashing begin!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 47, chance = 82000},
	{id = "health potion", chance = 210},
	{id = "meat", chance = 30070},
	{id = "plate shield", chance = 2500},
	{id = "short sword", chance = 8000},
	{id = "battle shield", chance = 1400},
	{id = "wolf tooth chain", chance = 190},
	{id = "halberd", chance = 1003},
	{id = "dark helmet", chance = 220},
	{id = "club ring", chance = 90},
	{id = "cyclops trophy", chance = 80},
	{id = "cyclops toe", chance = 4930}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 60, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 25},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = 20}
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