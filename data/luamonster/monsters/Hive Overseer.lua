local mType = Game.createMonsterType("Hive Overseer")
local monster = {}
monster.description = "a hive overseer"
monster.experience = 5500
monster.outfit = {
	lookType = 458
}

monster.health = 7500
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 15354
monster.speed = 200
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 4,
	staticAttack = 95
}

monster.summons = {
    {name = "Spidris Elite", chance = 40, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Zopp!", yell = false},
	{text = "Kropp!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 40000},
	{id = "gold coin", maxCount = 98, chance = 58000},
	{id = "platinum coin", maxCount = 6, chance = 84000},
	{id = "kollos shell", chance = 28000},
	{id = "great mana potion", chance = 18000},
	{id = "compound eye", chance = 16000},
	{id = "ultimate health potion", chance = 12000},
	{id = "small ruby", maxCount = 2, chance = 16000},
	{id = "giant shimmering pearl", chance = 6000},
	{id = "gold ingot", chance = 29000},
	{id = "gooey mass", maxCount = 2, chance = 13000},
	{id = "calopteryx cape", chance = 830},
	{id = "hive scythe", chance = 1650},
	{id = "hive bow", chance = 830},
	{id = "carapace shield", chance = 920},
	{id = "steel boots", chance = 550}
}

monster.attacks = {
	{name = "melee",  attack = 90, skill = 90, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = -60, maxDamage = -80, radius = 4, effect = 21},
	{name = "condition", type = CONDITION_POISON,  chance = 20, interval = 2000, minDamage = -600, maxDamage = -1000, radius = 4, effect = 9}
}

monster.defenses = {
	defense = 45,
    armor = 45,
    {name = "combat", type = COMBAT_HEALING,  chance = 50, interval = 2000, minDamage = 50, maxDamage = 100, radius = 4, effect = 13},
	{name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 500, maxDamage = 700, radius = 4, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 60},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 70},
	{type = COMBAT_HOLYDAMAGE, percent = 10}
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