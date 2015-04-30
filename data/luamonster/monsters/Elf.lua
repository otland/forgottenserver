local mType = Game.createMonsterType("Elf")
local monster = {}
monster.description = "a elf"
monster.experience = 42
monster.outfit = {
	lookType = 62
}

monster.health = 100
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6003
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
	{text = "Ulathil beia Thratha!", yell = false},
	{text = "Bahaha aka!", yell = false},
	{text = "You are not welcome here.", yell = false},
	{text = "Flee as long as you can.", yell = false},
	{text = "Death to the defilers!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 30, chance = 44000},
	{id = "arrow", maxCount = 3, chance = 7060},
	{id = "plum", maxCount = 2, chance = 20000},
	{id = "longsword", chance = 10800},
	{id = "leather boots", chance = 11410},
	{id = "plate shield", chance = 9300},
	{id = "studded armor", chance = 8960},
	{id = "studded helmet", chance = 13500},
	{id = "heaven blossom", chance = 940},
	{id = "elvish talisman", chance = 2100}
}

monster.attacks = {
	{name = "melee",  attack = 15, skill = 10, interval = 2000, effect = 3},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -25, range = 7, shootEffect = 3, effect = 3}
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