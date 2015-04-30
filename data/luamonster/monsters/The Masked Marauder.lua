local mType = Game.createMonsterType("The Masked Marauder")
local monster = {}
monster.description = "the masked marauder"
monster.experience = 3500
monster.outfit = {
	lookType = 234
}

monster.health = 6800
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7349
monster.speed = 250
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
	{text = "Didn't you leave your house door open?", yell = false},
	{text = "Oops, your shoelaces are open!", yell = false},
	{text = "Look! It's Ferumbras behind you!", yell = false},
	{text = "Stop! I give up!", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 160, skill = 70, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 40, interval = 2000, minDamage = -38, maxDamage = -150, shootEffect = 9}
}

monster.defenses = {
	defense = 35,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 50, interval = 1000, minDamage = 75, maxDamage = 125, shootEffect = 9, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 30},
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
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)