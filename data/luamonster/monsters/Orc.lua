local mType = Game.createMonsterType("Orc")
local monster = {}
monster.description = "a orc"
monster.experience = 25
monster.outfit = {
	lookType = 5
}

monster.health = 70
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5966
monster.speed = 150
monsters.runHealth = 15
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
	{text = "Grak brrretz!", yell = false},
	{text = "Grow truk grrrrr.", yell = false},
	{text = "Prek tars, dekklep zurk.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 14, chance = 84810},
	{id = "meat", chance = 10160},
	{id = "studded armor", chance = 7860},
	{id = "studded helmet", chance = 2950},
	{id = "studded shield", chance = 7300},
	{id = "sabre", chance = 5850},
	{id = "axe", chance = 4960},
	{id = "orc tooth", chance = 210},
	{id = "orc leather", chance = 590}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 25, minDamage = 0, maxDamage = -35, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
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