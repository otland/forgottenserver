local mType = Game.createMonsterType("Amazon")
local monster = {}
monster.description = "a amazon"
monster.experience = 60
monster.outfit = {
	lookFeet = 132,
	lookLegs = 114,
	lookHead = 113,
	lookType = 137,
	lookBody = 120
}

monster.health = 110
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20323
monster.speed = 180
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
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Yeeee ha!", yell = false},
	{text = "Your head shall be mine!", yell = false},
	{text = "Your head will be mine!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 20, chance = 40000},
	{id = "skull", maxCount = 2, chance = 80000},
	{id = "dagger", chance = 80000},
	{id = "protective charm", chance = 5000},
	{id = "brown bread", chance = 30333},
	{id = "sabre", chance = 23000},
	{id = "girlish hair decoration", chance = 10000},
	{id = "torch", chance = 1005},
	{id = "crystal necklace", chance = 287},
	{id = "small ruby", chance = 161}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 52163808, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -40, range = 7, shootEffect = 9}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_DEATHDAMAGE, percent = -5}
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