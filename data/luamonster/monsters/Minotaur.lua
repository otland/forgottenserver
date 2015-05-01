local mType = Game.createMonsterType("Minotaur")
local monster = {}
monster.description = "a minotaur"
monster.experience = 50
monster.outfit = {
	lookType = 25
}

monster.health = 100
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5969
monster.speed = 170
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
	{text = "Kaplar!", yell = false},
	{text = "Hurr!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 25, chance = 67500},
	{id = "plate shield", chance = 20020},
	{id = "mace", chance = 12840},
	{id = "sword", chance = 5000},
	{id = "chain armor", chance = 10000},
	{id = "meat", chance = 5000},
	{id = "brass helmet", chance = 7700},
	{id = "axe", chance = 4000},
	{id = "shovel", chance = 310},
	{id = "minotaur leather", chance = 990},
	{id = "bronze amulet", subType = 200, chance = 110},
	{id = "minotaur horn", maxCount = 2, chance = 2090}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 20, minDamage = 0, maxDamage = -45, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -15},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = 10},
	{type = COMBAT_FIREDAMAGE, percent = 20}
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