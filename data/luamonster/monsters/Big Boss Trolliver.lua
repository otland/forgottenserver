local mType = Game.createMonsterType("Big Boss Trolliver")
local monster = {}
monster.description = "big boss trolliver"
monster.experience = 105
monster.outfit = {
	lookType = 281
}

monster.health = 150
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7926
monster.speed = 140
monsters.runHealth = 20
monster.maxSummons = 0

monster.changeTarget = {
    interval = 2000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 95
}

monster.loot = {
    {id = "gold coin", maxCount = 57, chance = 6000},
	{id = "meat", maxCount = 3, chance = 1500},
	{id = "spear", maxCount = 2, chance = 2000},
	{id = "studded club", chance = 5000}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 35, minDamage = 0, maxDamage = -90, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 22},
	{type = COMBAT_EARTHDAMAGE, percent = -3},
	{type = COMBAT_DEATHDAMAGE, percent = -4},
	{type = COMBAT_HOLYDAMAGE, percent = 11}
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