local mType = Game.createMonsterType("Lizard Noble")
local monster = {}
monster.description = "a lizard noble"
monster.experience = 2000
monster.outfit = {
	lookType = 115
}

monster.health = 7000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6041
monster.speed = 200
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
	{text = "Where are zhe guardz when you need zhem!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 91300},
	{id = "platinum coin", maxCount = 20, chance = 10000},
	{id = "strong health potion", chance = 2550},
	{id = "small ruby", maxCount = 5, chance = 7100},
	{id = "great health potion", chance = 2900},
	{id = "lizard leather", chance = 220},
	{id = "lizard scale", chance = 650}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 20, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 25, interval = 2000, minDamage = -120, maxDamage = -250, range = 7, shootEffect = 15, effect = 21},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -100, range = 7, shootEffect = 15, effect = 13}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 50, interval = 2000, minDamage = 200, maxDamage = 250, shootEffect = 15, effect = 13}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)