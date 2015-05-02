local mType = Game.createMonsterType("Lizard Zaogun")
local monster = {}
monster.description = "a lizard zaogun"
monster.experience = 1700
monster.outfit = {
	lookType = 343
}

monster.health = 2955
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 11284
monster.speed = 250
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
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 31500},
	{id = "gold coin", maxCount = 100, chance = 31500},
	{id = "gold coin", maxCount = 68, chance = 31000},
	{id = "great health potion", maxCount = 3, chance = 7000},
	{id = "zaogun flag", chance = 8280},
	{id = "lizard scale", chance = 12520},
	{id = "Zaoan shoes", chance = 1000},
	{id = "red lantern", chance = 2170},
	{id = "platinum coin", maxCount = 2, chance = 48900},
	{id = "small emerald", maxCount = 5, chance = 4830},
	{id = "lizard leather", chance = 14360},
	{id = "strong health potion", chance = 1900},
	{id = "tower shield", chance = 1000},
	{id = "Zaoan legs", chance = 1001},
	{id = "Zaoan armor", chance = 530},
	{id = "zaogun shoulderplates", chance = 14980}
}

monster.attacks = {
	{name = "melee",  attack = 82, skill = 75, minDamage = 0, maxDamage = -349, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -220, maxDamage = -375, target = true, range = 7, radius = 1, shootEffect = 15, effect = 21}
}

monster.defenses = {
	defense = 40,
    armor = 40,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 175, maxDamage = 275, shootEffect = 15, effect = 15}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 5},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 45},
	{type = COMBAT_DEATHDAMAGE, percent = 10},
	{type = COMBAT_ICEDAMAGE, percent = 15}
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