local mType = Game.createMonsterType("Poacher")
local monster = {}
monster.description = "a poacher"
monster.experience = 70
monster.outfit = {
	lookFeet = 115,
	lookLegs = 119,
	lookHead = 115,
	lookType = 129,
	lookAddons = 1,
	lookBody = 119
}

monster.health = 90
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20487
monster.speed = 185
monster.runHealth = 5
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "You will not live to tell anyone!", yell = false},
	{text = "You are my game today!", yell = false},
	{text = "Look what has stepped into my trap!", yell = false}
}

monster.loot = {
    {id = "leather legs", chance = 26740},
	{id = "leather helmet", chance = 30600},
	{id = "roll", maxCount = 2, chance = 11110},
	{id = "bow", chance = 14930},
	{id = "arrow", maxCount = 17, chance = 49500},
	{id = "poison arrow", maxCount = 3, chance = 2930},
	{id = "torch", chance = 4180},
	{id = "closed trap", chance = 710}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 25, minDamage = 0, maxDamage = -35, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -35, range = 7, shootEffect = 3}
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