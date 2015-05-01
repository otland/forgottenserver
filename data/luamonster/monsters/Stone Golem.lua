local mType = Game.createMonsterType("Stone Golem")
local monster = {}
monster.description = "a stone golem"
monster.experience = 160
monster.outfit = {
	lookType = 67
}

monster.health = 270
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6005
monster.speed = 180
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
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 40, chance = 90000},
	{id = "small stone", maxCount = 4, chance = 13890},
	{id = "sulphurous stone", chance = 10370},
	{id = "ancient stone", chance = 1020},
	{id = "shiny stone", chance = 760},
	{id = "piece of marble rock", chance = 380},
	{id = "club ring", chance = 5070},
	{id = "coal", chance = 550},
	{id = "carlin sword", chance = 2500},
	{id = "iron ore", chance = 1980},
	{id = "crystal ring", chance = 120},
	{id = "red gem", chance = 30}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 45, minDamage = 0, maxDamage = -110, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 20},
	{type = COMBAT_ENERGYDAMAGE, percent = 15},
	{type = COMBAT_FIREDAMAGE, percent = 20},
	{type = COMBAT_DEATHDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = -10}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
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