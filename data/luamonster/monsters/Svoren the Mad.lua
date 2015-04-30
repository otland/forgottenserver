local mType = Game.createMonsterType("Svoren the Mad")
local monster = {}
monster.description = "svoren the mad"
monster.experience = 3000
monster.outfit = {
	lookFeet = 38,
	lookLegs = 118,
	lookHead = 80,
	lookType = 254,
	lookBody = 99
}

monster.health = 6300
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7349
monster.speed = 180
monster.maxSummons = 0

monster.changeTarget = {
    interval = 0,
	chance = 0
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
	staticAttack = 95
}

monster.voices = {
	interval = 2000,
	chance = 5,
	{text = "NO mommy NO. Leave me alone!", yell = false},
	{text = "Not that tower again!", yell = false},
	{text = "The cat has grown some horns!!", yell = false},
	{text = "What was I doing here again?", yell = false},
	{text = "Are we there soon mommy?", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 150, skill = 60, interval = 2000, effect = 0},
	{name = "speed",  chance = 35, interval = 3500, minDamage = 0, maxDamage = 0, target = true, duration = 40, speedChange = -250, range = 1, radius = 1, effect = 14}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = true, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = true, combat = true},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)