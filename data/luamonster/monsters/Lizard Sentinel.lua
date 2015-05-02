local mType = Game.createMonsterType("Lizard Sentinel")
local monster = {}
monster.description = "a lizard sentinel"
monster.experience = 110
monster.outfit = {
	lookType = 114
}

monster.health = 265
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6040
monster.speed = 180
monster.runHealth = 10
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Tssss!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 80, chance = 89000},
	{id = "spear", maxCount = 3, chance = 8750},
	{id = "chain armor", chance = 8560},
	{id = "scale armor", chance = 7730},
	{id = "hunting spear", chance = 4700},
	{id = "lizard scale", chance = 960},
	{id = "obsidian lance", chance = 1120},
	{id = "sentinel shield", chance = 320},
	{id = "lizard leather", chance = 990},
	{id = "health potion", chance = 590},
	{id = "halberd", chance = 510},
	{id = "small diamond", chance = 190}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 20, minDamage = 0, maxDamage = -45, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 30, interval = 2000, minDamage = 0, maxDamage = -70, range = 7, shootEffect = 1}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -10}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)