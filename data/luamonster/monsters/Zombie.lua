local mType = Game.createMonsterType("Zombie")
local monster = {}
monster.description = "a zombie"
monster.experience = 280
monster.outfit = {
	lookType = 311
}

monster.health = 500
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 9875
monster.speed = 180
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
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Mst.... klll....", yell = false},
	{text = "Whrrrr... ssss.... mmm.... grrrrl", yell = false},
	{text = "Dnnnt... cmmm... clsrrr....", yell = false},
	{text = "Httt.... hmnnsss...", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 65, chance = 82000},
	{id = "half-eaten brain", chance = 10000},
	{id = "brass helmet", chance = 9400},
	{id = "mace", chance = 7250},
	{id = "battle hammer", chance = 7000},
	{id = "rusty armor", chance = 5680},
	{id = "steel helmet", chance = 4600},
	{id = "halberd", chance = 3750},
	{id = "life ring (faster regeneration)", chance = 1000},
	{id = "mana potion", chance = 740},
	{id = "simple dress", chance = 560}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -130, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 10, interval = 2000, minDamage = -14, maxDamage = -23, range = 1},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -15, maxDamage = -24, range = 7, effect = 14},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 15, interval = 2000, minDamage = -30, maxDamage = -49, range = 1, effect = 39}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = false, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = true, combat = true},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)