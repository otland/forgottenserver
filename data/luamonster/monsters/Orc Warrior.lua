local mType = Game.createMonsterType("Orc Warrior")
local monster = {}
monster.description = "a orc warrior"
monster.experience = 50
monster.outfit = {
	lookType = 7
}

monster.health = 125
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5979
monster.speed = 190
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
	{text = "Grow truk grrrr.", yell = false},
	{text = "Trak grrrr brik.", yell = false},
	{text = "Alk!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 15, chance = 65000},
	{id = "meat", chance = 15000},
	{id = "chain armor", chance = 7360},
	{id = "copper shield", chance = 560},
	{id = "orc leather", chance = 4000},
	{id = "orc tooth", chance = 700},
	{id = "skull belt", chance = 980},
	{id = "broken helmet", chance = 10800},
	{id = "poison dagger", chance = 120}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 30, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 30},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = 10}
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