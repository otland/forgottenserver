local mType = Game.createMonsterType("Overcharged Energy Elemental")
local monster = {}
monster.description = "an orvercharged energy elemental"
monster.experience = 1300
monster.outfit = {
	lookType = 290
}

monster.health = 1700
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 8966
monster.speed = 300
monster.runHealth = 1
monster.maxSummons = 0

monster.changeTarget = {
    interval = 20000,
	chance = 15
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 85
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "BZZZZZZZZZZ", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 56, chance = 50000},
	{id = "energy soil", chance = 14285},
	{id = "great health potion", chance = 10000},
	{id = "small amethyst", maxCount = 2, chance = 10000},
	{id = "berserk potion", chance = 2173}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 40, minDamage = 0, maxDamage = -200, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 11, interval = 1000, minDamage = 0, maxDamage = -250, radius = 4, shootEffect = 5, effect = 48},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 12, interval = 1000, minDamage = 0, maxDamage = -300, range = 3, shootEffect = 5, effect = 48},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 12, interval = 1000, minDamage = 0, maxDamage = -200, radius = 4, shootEffect = 5, effect = 3}
}

monster.defenses = {
	defense = 35,
    armor = 35,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 90, maxDamage = 150, shootEffect = 5, effect = 13}
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
	{type = "energy", condition = true, combat = true},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)