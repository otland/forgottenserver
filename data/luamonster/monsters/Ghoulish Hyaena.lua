local mType = Game.createMonsterType("Ghoulish Hyaena")
local monster = {}
monster.description = "a ghoulish hyaena"
monster.experience = 195
monster.outfit = {
	lookFeet = 50,
	lookLegs = 40,
	lookHead = 20,
	lookType = 94,
	lookBody = 30
}

monster.health = 400
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6026
monster.speed = 200
monsters.runHealth = 30
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
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
	{text = "Grawrrr!!", yell = false},
	{text = "Hoouu!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 40, chance = 89000},
	{id = "small ruby", maxCount = 2, chance = 2700},
	{id = "worm", maxCount = 7, chance = 65000},
	{id = "meat", maxCount = 3, chance = 51060},
	{id = "health potion", chance = 19840}
}

monster.attacks = {
	{name = "melee",  attack = 56, skill = 30, minDamage = 0, maxDamage = -113, interval = 2000, effect = 0},
	{name = "condition", type = CONDITION_POISON,  chance = 15, interval = 2000, minDamage = -60, maxDamage = 0, length = 3, spread = 2, effect = 21}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 2000, speedChange = 3000, effect = 21}
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)