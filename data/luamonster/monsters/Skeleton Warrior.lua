local mType = Game.createMonsterType("Skeleton Warrior")
local monster = {}
monster.description = "a skeleton warrior"
monster.experience = 65
monster.outfit = {
	lookType = 298
}

monster.health = 45
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 5972
monster.speed = 150
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
    illusionable = false,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 10, chance = 43500},
	{id = "bone", chance = 50000},
	{id = "sword", chance = 1500},
	{id = "mace", chance = 2000},
	{id = "white mushroom", maxCount = 3, chance = 24000},
	{id = "brown mushroom", chance = 1700},
	{id = "pelvis bone", chance = 10630}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 20, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -7, maxDamage = -13, range = 1, effect = 14}
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)