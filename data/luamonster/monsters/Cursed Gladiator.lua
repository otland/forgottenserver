local mType = Game.createMonsterType("Cursed Gladiator")
local monster = {}
monster.description = "cursed gladiator"
monster.experience = 215
monster.outfit = {
	lookType = 100
}

monster.health = 435
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 7349
monster.speed = 170
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
	{text = "Death where are you?", yell = false},
	{text = "Slay me, end my curse.", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 60, skill = 40, minDamage = 0, maxDamage = -150, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 50, interval = 4000, minDamage = 0, maxDamage = 50, target = true, range = 5, radius = 1, effect = 14}
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
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)