local mType = Game.createMonsterType("Gang Member")
local monster = {}
monster.description = "a gang member"
monster.experience = 70
monster.outfit = {
	lookFeet = 20,
	lookLegs = 42,
	lookHead = 114,
	lookType = 151,
	lookBody = 19
}

monster.health = 295
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20403
monster.speed = 200
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = false,
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
	{text = "This is our territory!", yell = false},
	{text = "Help me guys!", yell = false},
	{text = "I don't like the way you look!", yell = false},
	{text = "You're wearing the wrong colours!", yell = false},
	{text = "Don't mess with us!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 30, chance = 50110},
	{id = "spear", chance = 8700},
	{id = "leather legs", chance = 15330},
	{id = "studded legs", chance = 5220},
	{id = "bread", chance = 4760},
	{id = "club ring", chance = 740}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 60, interval = 2000, effect = 0}
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