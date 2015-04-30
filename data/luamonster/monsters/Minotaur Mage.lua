local mType = Game.createMonsterType("Minotaur Mage")
local monster = {}
monster.description = "a minotaur mage"
monster.experience = 150
monster.outfit = {
	lookType = 23
}

monster.health = 155
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5981
monster.speed = 170
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
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Learrn tha secrret uf deathhh!", yell = false},
	{text = "Kaplar!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 35, chance = 85780},
	{id = "carrot", maxCount = 8, chance = 15003},
	{id = "torch", chance = 4950},
	{id = "leather legs", chance = 5190},
	{id = "leather helmet", chance = 3000},
	{id = "minotaur leather", chance = 2290},
	{id = "taurus mace", chance = 1000},
	{id = "wand of cosmic energy", chance = 600},
	{id = "mana potion", chance = 490},
	{id = "minotaur horn", maxCount = 2, chance = 3020},
	{id = "purple robe", maxCount = 2, chance = 6430}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 10, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 15, interval = 2000, minDamage = -20, maxDamage = -59, range = 7, shootEffect = 5, effect = 12},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = -50, maxDamage = -105, target = true, range = 7, radius = 1, shootEffect = 4, effect = 7},
	{name = "energyfield",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 1, shootEffect = 36, effect = 7}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE, percent = 10},
	{type = COMBAT_DEATHDAMAGE, percent = -10},
	{type = COMBAT_ICEDAMAGE, percent = -10}
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)