local mType = Game.createMonsterType("The Frog Prince")
local monster = {}
monster.description = "the frog prince"
monster.experience = 1
monster.outfit = {
	lookFeet = 66,
	lookLegs = 69,
	lookHead = 69,
	lookType = 224,
	lookBody = 66
}

monster.health = 55
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 6079
monster.speed = 230
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 20
}

monster.flags = {
	summonable = true,
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
	{text = "Ribbit!", yell = false},
	{text = "Ribbit! Ribbit!", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 1, skill = 10, minDamage = 0, maxDamage = -1, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 85},
	{type = COMBAT_ICEDAMAGE, percent = 90},
	{type = COMBAT_FIREDAMAGE, percent = 10}
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