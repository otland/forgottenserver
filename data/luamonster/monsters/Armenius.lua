local mType = Game.createMonsterType("Armenius")
local monster = {}
monster.description = "a armenius"
monster.experience = 720
monster.outfit = {
	lookType = 79
}

monster.health = 1000
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 6021
monster.speed = 330
monster.maxSummons = 3

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
    {name = "Larva", chance = 15, interval = 1000}
}

monster.loot = {
    {id = "gold coin", maxCount = 90, chance = 100000},
	{id = "gold coin", maxCount = 90, chance = 100000},
	{id = "magic light wand", chance = 11111},
	{id = "plate armor", chance = 10000},
	{id = "scarab coin", maxCount = 2, chance = 7692},
	{id = "small emerald", maxCount = 3, chance = 4347},
	{id = "small amethyst", maxCount = 4, chance = 4166},
	{id = "scarab pincers", chance = 3571},
	{id = "ancient amulet", chance = 2564},
	{id = "scarab amulet", chance = 2063},
	{id = "terra hood", chance = 1298},
	{id = "strong health potion", chance = 1041},
	{id = "daramian waraxe", chance = 781},
	{id = "scarab shield", chance = 518}
}

monster.attacks = {
	{name = "melee",  attack = 105, skill = 30, minDamage = 0, maxDamage = -210, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 12, interval = 1000, minDamage = -15, maxDamage = -135, range = 7, shootEffect = 15, effect = 21},
	{name = "speed",  chance = 13, interval = 1000, minDamage = 0, maxDamage = 0, duration = 25000, speedChange = -700, range = 7, shootEffect = 15, effect = 21},
	{name = "condition", type = CONDITION_POISON,  chance = 17, interval = 3000, minDamage = -30, maxDamage = -100, radius = 5, shootEffect = 15, effect = 21}
}

monster.defenses = {
	defense = 20,
    armor = 25,
    {name = "speed",  chance = 8, interval = 1000, minDamage = 0, maxDamage = 0, duration = 9000, speedChange = 350, shootEffect = 15, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -15},
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