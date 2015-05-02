local mType = Game.createMonsterType("Cobra")
local monster = {}
monster.description = "a cobra"
monster.experience = 30
monster.outfit = {
	lookType = 81
}

monster.health = 65
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 3007
monster.speed = 170
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
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
	{text = "Zzzzzz", yell = false},
	{text = "Fsssss", yell = false}
}

monster.loot = {
    {id = "cobra tongue", chance = 5000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = 0, interval = 2000, effect = 0},
	{name = "condition", type = CONDITION_POISON,  chance = 15, interval = 2000, minDamage = -20, maxDamage = -40, range = 7, shootEffect = 15}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = false, combat = false},
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