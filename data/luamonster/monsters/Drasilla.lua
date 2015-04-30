local mType = Game.createMonsterType("Drasilla")
local monster = {}
monster.description = "drasilla"
monster.experience = 700
monster.outfit = {
	lookType = 34
}

monster.health = 1320
monster.maxHealth = monster.health
monster.race = "blood"
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
	{text = "FCHHHHHHHH!", yell = false},
	{text = "GROOOOAAAAAAAAR!", yell = false},
	{text = "DIRTY LITTLE HUMANS", yell = false},
	{text = "YOU CAN'T KEEP ME HERE FOREVER", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 60, skill = 30, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 60, interval = 6000, minDamage = -100, maxDamage = -180, length = 8, spread = 3, effect = 7},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 50, interval = 3000, minDamage = -70, maxDamage = -115, target = true, range = 10, radius = 5, effect = 7}
}

monster.defenses = {
	defense = 33,
    armor = 32,
    {name = "combat", type = COMBAT_HEALING,  chance = 65, interval = 6000, minDamage = 20, maxDamage = 50, radius = 5, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = -10}
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