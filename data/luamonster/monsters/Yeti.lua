local mType = Game.createMonsterType("Yeti")
local monster = {}
monster.description = "a yeti"
monster.experience = 460
monster.outfit = {
	lookType = 110
}

monster.health = 950
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6038
monster.speed = 270
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 8
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

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Yooodelaaahooohooo!", yell = false},
	{text = "Yooodelaaaheeeheee!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 60, chance = 100000},
	{id = "gold coin", maxCount = 40, chance = 100000},
	{id = "meat", maxCount = 4, chance = 33333},
	{id = "ham", maxCount = 5, chance = 10000},
	{id = "snowball", maxCount = 22, chance = 10000},
	{id = "bunnyslippers", chance = 1333}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 32, interval = 2000, effect = 14},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 1000, minDamage = 0, maxDamage = -180, range = 7, radius = 1, shootEffect = 13, effect = 3},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 12, interval = 1000, minDamage = 0, maxDamage = -175, length = 3, spread = 3, radius = 1, shootEffect = 13, effect = 3}
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