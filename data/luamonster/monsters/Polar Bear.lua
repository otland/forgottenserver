local mType = Game.createMonsterType("Polar Bear")
local monster = {}
monster.description = "a polar bear"
monster.experience = 28
monster.outfit = {
	lookType = 42
}

monster.health = 85
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5987
monster.speed = 135
monsters.runHealth = 5
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 0
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
	{text = "GROARRR!", yell = true}
}

monster.loot = {
    {id = "ham", maxCount = 2, chance = 50000},
	{id = "meat", maxCount = 4, chance = 5000},
	{id = "polar bear paw", chance = 920}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 10, minDamage = 0, maxDamage = -30, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -1},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_DEATHDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 1}
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