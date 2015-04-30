local mType = Game.createMonsterType("Pirate Ghost")
local monster = {}
monster.description = "a pirate ghost"
monster.experience = 250
monster.outfit = {
	lookType = 196
}

monster.health = 275
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 5566
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Yooh Ho Hooh Ho!", yell = false},
	{text = "Hell is waiting for You!", yell = false},
	{text = "It's alive!", yell = false},
	{text = "The curse! Aww the curse!", yell = false},
	{text = "You will not get my treasure!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 67, chance = 48000},
	{id = "tattered piece of robe", chance = 4300},
	{id = "stealth ring", chance = 650},
	{id = "parchment", chance = 910},
	{id = "spike sword", chance = 130},
	{id = "red robe", chance = 130}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 30, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -40, maxDamage = -80, radius = 1, effect = 14},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 10, interval = 2000, minDamage = -50, maxDamage = -65, target = true, range = 7, radius = 3, effect = 20}
}

monster.defenses = {
	defense = 0,
    armor = 0,
    {name = "combat", type = COMBAT_HEALING,  chance = 5, interval = 2000, minDamage = 40, maxDamage = 70, radius = 3, effect = 13}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = false, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = true},
	{type = "bleed", condition = true, combat = false},
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)