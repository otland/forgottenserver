local mType = Game.createMonsterType("Dipthrah")
local monster = {}
monster.description = "a dipthrah"
monster.experience = 2900
monster.outfit = {
	lookType = 87
}

monster.health = 4200
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6031
monster.speed = 320
monster.maxSummons = 4

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

monster.summons = {
    {name = "Priestess", chance = 15, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Come closer to learn the final lesson.", yell = false},
	{text = "You can't escape death forever.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 80, chance = 50000},
	{id = "gold coin", maxCount = 80, chance = 50000},
	{id = "small sapphire", maxCount = 3, chance = 7000},
	{id = "great mana potion", chance = 7000},
	{id = "energy ring", chance = 7000},
	{id = "mind stone", chance = 1500},
	{id = "blue gem", chance = 1500},
	{id = "ankh", chance = 500},
	{id = "skull staff", chance = 500},
	{id = "pharaoh sword", chance = 300},
	{id = "ornamented ankh", chance = 100000}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 40, minDamage = 0, maxDamage = -200, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 20, interval = 4000, minDamage = -100, maxDamage = -800, range = 1},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 15, interval = 2000, minDamage = -100, maxDamage = -500, range = 7, effect = 14},
	{name = "speed",  chance = 15, interval = 1000, minDamage = 0, maxDamage = 0, duration = 50000, speedChange = -650, range = 7, effect = 14},
	{name = "drunk",  chance = 12, interval = 1000, minDamage = 0, maxDamage = 0, radius = 7, effect = 2},
	{name = "melee",  attack = 0, skill = 0, minDamage = -50, maxDamage = -600, interval = 3000, effect = 2}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 1000, minDamage = 100, maxDamage = 200, effect = 13}
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
	{type = "physical", condition = false, combat = true},
	{type = "bleed", condition = true, combat = false},
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)