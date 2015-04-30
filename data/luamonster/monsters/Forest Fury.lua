local mType = Game.createMonsterType("Forest Fury")
local monster = {}
monster.description = "a forest fury"
monster.experience = 235
monster.outfit = {
	lookType = 569
}

monster.health = 480
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 21359
monster.speed = 210
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
	canPushCreatures = true,
	targetDistance = 4,
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "To arms, sisters!", yell = false},
	{text = "Feel the wrath of mother forest!", yell = false},
	{text = "By the power of Greenskull!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 53, chance = 87250},
	{id = "bolt", maxCount = 15, chance = 48270},
	{id = "meat", chance = 24930},
	{id = "piercing bolt", maxCount = 5, chance = 14900},
	{id = "venison", chance = 13210},
	{id = "elven hoof", chance = 7030},
	{id = "crossbow", chance = 2750},
	{id = "small topaz", chance = 470},
	{id = "elvish bow", chance = 90}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 40, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 40, interval = 1500, minDamage = 0, maxDamage = -100, target = true, range = 7, radius = 4, shootEffect = 17, effect = 15},
	{name = "forest fury skill reducer",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, range = 5, radius = 4, shootEffect = 17, effect = 15}
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