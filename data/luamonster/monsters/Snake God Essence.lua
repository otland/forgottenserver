local mType = Game.createMonsterType("Snake God Essence")
local monster = {}
monster.description = "snake god essence"
monster.experience = 1350
monster.outfit = {
	lookType = 356
}

monster.health = 20000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 12385
monster.speed = 300
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
	staticAttack = 95
}

monster.voices = {
	interval = 2000,
	chance = 10,
	{text = "AHHH ZHE POWER...", yell = true},
	{text = "ZHE TIME OF ZHE SNAKE HAZ COME!", yell = true}
}

monster.attacks = {
	{name = "melee",  attack = 160, skill = 90, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 40, interval = 2000, minDamage = 0, maxDamage = -300, length = 8, effect = 14},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 50, interval = 2000, minDamage = -150, maxDamage = -270, radius = 6, effect = 15}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = -10},
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)