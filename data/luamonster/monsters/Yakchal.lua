local mType = Game.createMonsterType("Yakchal")
local monster = {}
monster.description = "an yakchal"
monster.experience = 4400
monster.outfit = {
	lookFeet = 105,
	lookLegs = 105,
	lookType = 149,
	lookBody = 47
}

monster.health = 5000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20546
monster.speed = 200
monster.maxSummons = 4

monster.changeTarget = {
    interval = 2000,
	chance = 5
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
	staticAttack = 50
}

monster.summons = {
    {name = "Ice Golem", chance = 13, interval = 1000}
}

monster.voices = {
	interval = 2000,
	chance = 5,
	{text = "So you think you are cool?", yell = false},
	{text = "I hope it is not too cold for you! HeHeHe.", yell = false},
	{text = "Freeze!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 88, chance = 10000},
	{id = "blue piece of cloth", chance = 5000},
	{id = "crystal sword", chance = 4000},
	{id = "mastermind potion", chance = 1500},
	{id = "berserk potion", chance = 1500},
	{id = "bullseye potion", chance = 1500},
	{id = "pair of earmuffs", chance = 1000},
	{id = "dragon necklace", subType = 200, chance = 3250},
	{id = "glacier kilt", chance = 2650},
	{id = "boots of haste", chance = 2000},
	{id = "glacier robe", chance = 3000},
	{id = "queen's sceptre", chance = 1500},
	{id = "skull staff", chance = 4500},
	{id = "gold ingot", chance = 5000},
	{id = "crystal mace", chance = 4000},
	{id = "shard", chance = 100000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -389, interval = 2000, effect = 0},
	{name = "outfit",  chance = 10, interval = 1000, minDamage = 0, maxDamage = 0, range = 7, effect = 13},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 18, interval = 2000, minDamage = 0, maxDamage = -430, target = true, radius = 4, shootEffect = 37, effect = 42},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 34, interval = 3000, minDamage = -200, maxDamage = -310, target = true, range = 7, radius = 3, shootEffect = 13, effect = 42},
	{name = "speed",  chance = 10, interval = 1000, minDamage = 0, maxDamage = 0, duration = 20000, speedChange = -600, range = 7, shootEffect = 13, effect = 14}
}

monster.defenses = {
	defense = 20,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 1000, minDamage = 50, maxDamage = 100, shootEffect = 13, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 40},
	{type = COMBAT_FIREDAMAGE, percent = 50},
	{type = COMBAT_DEATHDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = 26}
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
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)