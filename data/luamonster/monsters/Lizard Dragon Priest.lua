local mType = Game.createMonsterType("Lizard Dragon Priest")
local monster = {}
monster.description = "a lizard dragon priest"
monster.experience = 1320
monster.outfit = {
	lookType = 339
}

monster.health = 1450
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 11280
monster.speed = 220
monster.maxSummons = 2

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

monster.summons = {
    {name = "Dragon Hatchling", chance = 20, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "I ssssmell warm blood!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 5000},
	{id = "gold coin", maxCount = 88, chance = 4400},
	{id = "bunch of ripe rice", chance = 960},
	{id = "platinum coin", maxCount = 2, chance = 4090},
	{id = "yellow gem", chance = 1000},
	{id = "small amethyst", maxCount = 3, chance = 4900},
	{id = "dragon priest's wandtip", chance = 9910},
	{id = "focus cape", chance = 660},
	{id = "strong mana potion", chance = 12110},
	{id = "lizard leather", chance = 980},
	{id = "terra rod", chance = 1001},
	{id = "great mana potion", chance = 7940},
	{id = "Zaoan robe", chance = 300},
	{id = "wand of inferno", chance = 1480},
	{id = "lizard scale", chance = 1130},
	{id = "Zaoan shoes", chance = 450},
	{id = "life ring", chance = 770}
}

monster.attacks = {
	{name = "melee",  attack = 25, skill = 30, interval = 2000, effect = 0},
	{name = "fireball",  chance = 20, interval = 2000, minDamage = -125, maxDamage = -190, range = 7},
	{name = "condition", type = CONDITION_POISON,  chance = 15, interval = 2000, minDamage = -320, maxDamage = -400, target = true, range = 7, radius = 1, shootEffect = 15, effect = 21}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 30, interval = 2000, minDamage = 200, maxDamage = 300, radius = 1, shootEffect = 15, effect = 13},
	{name = "invisible",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, radius = 1, shootEffect = 15, effect = 13}
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