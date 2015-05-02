local mType = Game.createMonsterType("Spirit of Fire")
local monster = {}
monster.description = "the spirit of fire"
monster.experience = 950
monster.outfit = {
	lookType = 242
}

monster.health = 2210
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7349
monster.speed = 220
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
	chance = 6,
	{text = "Feel the heat.", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 50, minDamage = 0, maxDamage = -300, interval = 2000, effect = 0},
	{name = "firefield",  chance = 15, interval = 1000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 4, shootEffect = 4},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 20, interval = 1000, minDamage = -315, maxDamage = -650, range = 7, shootEffect = 4}
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