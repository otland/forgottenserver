local mType = Game.createMonsterType("Ancient Scarab")
local monster = {}
monster.description = "a ancient scarab"
monster.experience = 720
monster.outfit = {
	lookType = 79
}

monster.health = 1000
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 6021
monster.speed = 230
monster.maxSummons = 3

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
	targetDistance = 1,
	staticAttack = 80
}

monster.summons = {
    {name = "Larva", chance = 10, interval = 2000}
}

monster.loot = {
    {id = "gold coin", maxCount = 90, chance = 50000},
	{id = "gold coin", maxCount = 97, chance = 50000},
	{id = "magic light wand", chance = 11480},
	{id = "plate armor", chance = 10300},
	{id = "scarab coin", maxCount = 2, chance = 7692},
	{id = "small emerald", maxCount = 3, chance = 4810},
	{id = "small amethyst", maxCount = 4, chance = 5000},
	{id = "scarab pincers", chance = 3571},
	{id = "ancient amulet", chance = 2570},
	{id = "scarab amulet", chance = 3410},
	{id = "terra hood", chance = 490},
	{id = "strong health potion", chance = 660},
	{id = "daramian waraxe", chance = 420},
	{id = "scarab shield", chance = 480}
}

monster.attacks = {
	{name = "melee",  attack = 65, skill = 30, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = -15, maxDamage = -145, range = 7, shootEffect = 15, effect = 21},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 25000, speedChange = -700, range = 7, shootEffect = 15, effect = 21},
	{name = "condition", type = CONDITION_POISON,  chance = 30, interval = 2000, minDamage = -440, maxDamage = -520, radius = 5, shootEffect = 15, effect = 21}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 380, radius = 5, shootEffect = 15, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -20},
	{type = COMBAT_ICEDAMAGE, percent = -5}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
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