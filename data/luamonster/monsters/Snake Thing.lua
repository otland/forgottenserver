local mType = Game.createMonsterType("Snake Thing")
local monster = {}
monster.description = "snake thing"
monster.experience = 4600
monster.outfit = {
	lookType = 220
}

monster.health = 20000
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 12385
monster.speed = 240
monster.maxSummons = 0

monster.changeTarget = {
    interval = 2000,
	chance = 10
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 2000,
	chance = 10,
	{text = "POWER! I SEED MORE POWER!", yell = true}
}

monster.attacks = {
	{name = "melee",  attack = 160, skill = 40, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 35, interval = 2000, minDamage = 0, maxDamage = -500, length = 8, spread = 3, effect = 21},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 40, interval = 2000, minDamage = 0, maxDamage = -2398, length = 8, effect = 20},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 30, interval = 2000, minDamage = -30, maxDamage = -600, radius = 6, effect = 21}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -10}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)