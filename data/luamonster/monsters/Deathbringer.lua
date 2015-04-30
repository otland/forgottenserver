local mType = Game.createMonsterType("Deathbringer")
local monster = {}
monster.description = "deathbringer"
monster.experience = 5100
monster.outfit = {
	lookType = 231
}

monster.health = 10000
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 7349
monster.speed = 300
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
	interval = 5000,
	chance = 10,
	{text = "YOU FOOLS WILL PAY!", yell = true},
	{text = "YOU ALL WILL DIE!!", yell = true},
	{text = "DEATH, DESTRUCTION!", yell = true},
	{text = "I will eat your soul!", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 155, skill = 50, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 1000, minDamage = -80, maxDamage = -120, target = true, range = 7, radius = 4, shootEffect = 4, effect = 1},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 17, interval = 3000, minDamage = -300, maxDamage = -450, length = 8, spread = 3, radius = 4, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 12, interval = 2000, minDamage = -300, maxDamage = -450, length = 8, spread = 3, radius = 4, shootEffect = 4, effect = 21},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -80, maxDamage = -100, radius = 6, shootEffect = 4, effect = 3},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 25, interval = 3000, minDamage = -80, maxDamage = -150, range = 7, radius = 6, shootEffect = 4, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE, percent = -15}
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
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)