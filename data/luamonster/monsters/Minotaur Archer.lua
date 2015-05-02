local mType = Game.createMonsterType("Minotaur Archer")
local monster = {}
monster.description = "a minotaur archer"
monster.experience = 65
monster.outfit = {
	lookType = 24
}

monster.health = 100
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5982
monster.speed = 170
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
	{text = "Ruan Wihmpy!", yell = false},
	{text = "Kaplar!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 30, chance = 75410},
	{id = "bolt", maxCount = 20, chance = 90450},
	{id = "piercing bolt", maxCount = 4, chance = 12340},
	{id = "crossbow", chance = 830},
	{id = "meat", chance = 5000},
	{id = "brass armor", chance = 530},
	{id = "minotaur leather", chance = 1900},
	{id = "scale armor", chance = 530},
	{id = "broken crossbow", chance = 15400},
	{id = "piece of archer armor", chance = 8260},
	{id = "minotaur horn", maxCount = 2, chance = 1990}
}

monster.attacks = {
	{name = "melee",  attack = 25, skill = 10, minDamage = 0, maxDamage = -25, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 40, interval = 2000, minDamage = 0, maxDamage = -80, range = 7, shootEffect = 2}
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