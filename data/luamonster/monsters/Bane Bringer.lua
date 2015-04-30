local mType = Game.createMonsterType("Bane Bringer")
local monster = {}
monster.description = "a bane bringer"
monster.experience = 400
monster.outfit = {
	lookType = 310
}

monster.health = 2500
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 9867
monster.speed = 260
monster.maxSummons = 0

monster.changeTarget = {
    interval = 0,
	chance = 8
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
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 30,
	{text = "You shall not succeed!", yell = false},
	{text = "This time we will prevail!", yell = false}
}

monster.loot = {
    {id = "dry piece of wood", chance = 100000},
	{id = "wooden trash", maxCount = 96, chance = 14285}
}

monster.attacks = {
	{name = "melee",  attack = 60, skill = 40, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = -80},
	{type = COMBAT_EARTHDAMAGE, percent = -80},
	{type = COMBAT_FIREDAMAGE, percent = -80},
	{type = COMBAT_HOLYDAMAGE, percent = -50},
	{type = COMBAT_DEATHDAMAGE, percent = -90},
	{type = COMBAT_ICEDAMAGE, percent = -80}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)