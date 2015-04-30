local mType = Game.createMonsterType("Grave Robber")
local monster = {}
monster.description = "a grave robber"
monster.experience = 65
monster.outfit = {
	lookFeet = 38,
	lookLegs = 76,
	lookHead = 114,
	lookType = 146,
	lookAddons = 3,
	lookBody = 95
}

monster.health = 165
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20411
monster.speed = 205
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
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "This crypt is taken!", yell = false},
	{text = "Leave or you are hunted!", yell = false},
	{text = "The treasure will be mine!", yell = false},
	{text = "The X marks the spot!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 38, chance = 54260},
	{id = "axe", chance = 2500},
	{id = "mace", chance = 1700},
	{id = "brass armor", chance = 1700},
	{id = "steel shield", chance = 500},
	{id = "iron helmet", chance = 500},
	{id = "rope belt", chance = 7510},
	{id = "potato", maxCount = 3, chance = 5680},
	{id = "nomad parchment", chance = 170},
	{id = "dirty turban", chance = 1700}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 35, interval = 2000, effect = 0},
	{name = "condition", type = CONDITION_POISON,  chance = 15, interval = 2000, minDamage = -100, maxDamage = -160, target = true, range = 7, radius = 1, shootEffect = 15}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 20},
	{type = COMBAT_DEATHDAMAGE, percent = -10},
	{type = COMBAT_ICEDAMAGE, percent = -10}
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