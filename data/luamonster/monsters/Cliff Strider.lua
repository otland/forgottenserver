local mType = Game.createMonsterType("Cliff Strider")
local monster = {}
monster.description = "a cliff strider"
monster.experience = 5700
monster.outfit = {
	lookType = 497
}

monster.health = 9400
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 17420
monster.speed = 260
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Knorrrr", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "gold coin", maxCount = 95, chance = 100000},
	{id = "platinum coin", maxCount = 10, chance = 100000},
	{id = "great mana potion", maxCount = 4, chance = 30950},
	{id = "ultimate health potion", maxCount = 2, chance = 25260},
	{id = "soul orb", chance = 17080},
	{id = "vein of ore", maxCount = 2, chance = 18000},
	{id = "cliff strider claw", chance = 15630},
	{id = "pulverized ore", chance = 15110},
	{id = "iron ore", chance = 14490},
	{id = "shiny stone", chance = 11280},
	{id = "prismatic bolt", maxCount = 8, chance = 9090},
	{id = "black pearl", chance = 9090},
	{id = "blue crystal splinter", maxCount = 2, chance = 7970},
	{id = "white pearl", maxCount = 3, chance = 9630},
	{id = "cyan crystal fragment", chance = 8070},
	{id = "blue crystal shard", chance = 6520},
	{id = "red gem", chance = 6110},
	{id = "crystal of balance", chance = 2690},
	{id = "wand of defiance", chance = 2000},
	{id = "magic sulphur", chance = 1450},
	{id = "sapphire hammer", chance = 1350},
	{id = "glacial rod", chance = 1240},
	{id = "spiked squelcher", chance = 1040},
	{id = "crystal crossbow", chance = 830},
	{id = "giant sword", chance = 620},
	{id = "crusader helmet", chance = 410},
	{id = "crystal of power", chance = 720},
	{id = "crystalline sword", chance = 620},
	{id = "knight legs", chance = 830},
	{id = "blue gem", chance = 830},
	{id = "crown armor", chance = 310},
	{id = "steel boots", chance = 100},
	{id = "hammer of wrath", chance = 70}
}

monster.attacks = {
	{name = "melee",  attack = 95, skill = 95, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -800, target = true, radius = 4, shootEffect = 12, effect = 45},
	{name = "cliff strider skill reducer",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, radius = 4, shootEffect = 12, effect = 45},
	{name = "cliff strider electrify",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, range = 1, radius = 4, shootEffect = 12, effect = 45},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -1000, length = 6, radius = 4, shootEffect = 12, effect = 35},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 15, interval = 2000, minDamage = -100, maxDamage = -300, radius = 4, shootEffect = 12, effect = 49}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 1},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_DEATHDAMAGE, percent = 40},
	{type = COMBAT_FIREDAMAGE, percent = 1}
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
	{type = "energy", condition = true, combat = true},
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