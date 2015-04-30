local mType = Game.createMonsterType("Warlord Ruzad")
local monster = {}
monster.description = "the warlord ruzad"
monster.experience = 1700
monster.outfit = {
	lookFeet = 50,
	lookLegs = 40,
	lookHead = 20,
	lookType = 2,
	lookBody = 30
}

monster.health = 1950
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6008
monster.speed = 270
monster.maxSummons = 3

monster.changeTarget = {
    interval = 2000,
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

monster.summons = {
    {name = "Orc Berserker", chance = 30, interval = 2000}
}

monster.loot = {
    {id = "gold coin", maxCount = 59, chance = 50000},
	{id = "meat", maxCount = 3, chance = 20000},
	{id = "fish", chance = 6667},
	{id = "fire sword", chance = 8970},
	{id = "plate armor", chance = 7333},
	{id = "throwing star", maxCount = 18, chance = 6666},
	{id = "orcish axe", chance = 13000},
	{id = "brass legs", chance = 3333},
	{id = "protection amulet", subType = 250, chance = 9000},
	{id = "two handed sword", chance = 18180},
	{id = "crusader helmet", chance = 6670},
	{id = "gold ring", chance = 5506}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 65, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 30},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 80},
	{type = COMBAT_DEATHDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = 10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
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