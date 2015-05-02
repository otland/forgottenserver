local mType = Game.createMonsterType("Orc Spearman")
local monster = {}
monster.description = "a orc spearman"
monster.experience = 38
monster.outfit = {
	lookType = 50
}

monster.health = 105
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5996
monster.speed = 176
monster.runHealth = 10
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
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Ugaar!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 11, chance = 25050},
	{id = "machete", chance = 3000},
	{id = "meat", chance = 30200},
	{id = "spear", chance = 17440},
	{id = "studded helmet", chance = 9000},
	{id = "studded legs", chance = 10000},
	{id = "orc leather", chance = 2300},
	{id = "orc tooth", chance = 150}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 15, minDamage = 0, maxDamage = -25, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -30, range = 7, shootEffect = 1}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = 20}
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