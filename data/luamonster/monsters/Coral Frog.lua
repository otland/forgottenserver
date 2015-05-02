local mType = Game.createMonsterType("Coral Frog")
local monster = {}
monster.description = "a coral frog"
monster.experience = 20
monster.outfit = {
	lookFeet = 114,
	lookLegs = 97,
	lookHead = 114,
	lookType = 226,
	lookBody = 98
}

monster.health = 60
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6079
monster.speed = 200
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Ribbit!", yell = false},
	{text = "Ribbit! Ribbit!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 10, chance = 76780},
	{id = "worm", chance = 13510}
}

monster.attacks = {
	{name = "melee",  attack = 24, skill = 10, minDamage = 0, maxDamage = -24, interval = 2000, effect = 0}
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