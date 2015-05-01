local mType = Game.createMonsterType("Manta Ray")
local monster = {}
monster.description = "a manta ray"
monster.experience = 125
monster.outfit = {
	lookType = 449
}

monster.health = 680
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 15276
monster.speed = 200
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = false,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 95
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Flap flap flap!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 38, chance = 50000}
}

monster.attacks = {
	{name = "melee",  attack = 45, skill = 34, minDamage = 0, maxDamage = -99, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 10, interval = 2000, minDamage = -15, maxDamage = -75, range = 7, shootEffect = 5, effect = 12},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, length = 4, shootEffect = 5, effect = 12}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -1},
	{type = COMBAT_ENERGYDAMAGE, percent = 1},
	{type = COMBAT_ICEDAMAGE, percent = -1}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)