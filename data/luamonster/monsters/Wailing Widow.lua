local mType = Game.createMonsterType("Wailing Widow")
local monster = {}
monster.description = "a wailing widow"
monster.experience = 450
monster.outfit = {
	lookType = 347
}

monster.health = 850
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 11310
monster.speed = 220
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
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 37, chance = 50000},
	{id = "widow's mandibles", chance = 20950},
	{id = "health potion", chance = 4761},
	{id = "plate shield", chance = 2854},
	{id = "mana potion", chance = 4785},
	{id = "green mushroom", chance = 3208},
	{id = "wailing widow's necklace", chance = 900},
	{id = "Zaoan halberd", chance = 2210},
	{id = "halberd", chance = 4460}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 50, minDamage = 0, maxDamage = -120, interval = 2000, effect = 0},
	{name = "drunk",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, radius = 4, effect = 20},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -40, maxDamage = -70, radius = 3, effect = 14},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -60, maxDamage = -110, range = 7, shootEffect = 15, effect = 14}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 5, interval = 2000, minDamage = 70, maxDamage = 100, shootEffect = 15, effect = 25},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 820, shootEffect = 15, effect = 22}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)