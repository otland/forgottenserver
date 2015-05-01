local mType = Game.createMonsterType("Hand of Cursed Fate")
local monster = {}
monster.description = "a hand of cursed fate"
monster.experience = 5000
monster.outfit = {
	lookType = 230
}

monster.health = 7500
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6312
monster.speed = 260
monsters.runHealth = 3500
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
	staticAttack = 20
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 60000},
	{id = "gold coin", maxCount = 100, chance = 60000},
	{id = "gold coin", maxCount = 67, chance = 50000},
	{id = "platinum coin", maxCount = 7, chance = 100000},
	{id = "concentrated demonic blood", maxCount = 4, chance = 30000},
	{id = "soul orb", chance = 31111},
	{id = "demonic essence", chance = 12000},
	{id = "small sapphire", maxCount = 4, chance = 11000},
	{id = "ultimate health potion", chance = 18000},
	{id = "great mana potion", maxCount = 2, chance = 19990},
	{id = "mind stone", chance = 9090},
	{id = "onyx arrow", maxCount = 3, chance = 8333},
	{id = "assassin star", maxCount = 5, chance = 7692},
	{id = "emerald bangle", chance = 3500},
	{id = "protection amulet", subType = 250, chance = 8740},
	{id = "wand of inferno", chance = 5590},
	{id = "knight armor", chance = 4550},
	{id = "death ring", chance = 1750},
	{id = "crown armor", chance = 1400},
	{id = "energy ring", chance = 3150},
	{id = "platinum amulet", chance = 1005},
	{id = "abyss hammer", chance = 495},
	{id = "gold ingot", chance = 700},
	{id = "skull staff", chance = 700},
	{id = "violet gem", chance = 700},
	{id = "yellow gem", chance = 5940},
	{id = "boots of haste", chance = 540},
	{id = "golden figurine", chance = 195},
	{id = "mysterious voodoo skull", chance = 247},
	{id = "sudden death rune", maxCount = 8, subType = 3, chance = 4200}
}

monster.attacks = {
	{name = "melee",  attack = 130, skill = 70, minDamage = 0, maxDamage = -520, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -920, range = 1},
	{name = "drunk",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, radius = 4, effect = 39},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -220, maxDamage = -880, range = 1, effect = 39}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 1000, effect = 14},
	{name = "invisible",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, effect = 13},
	{name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 2000, minDamage = 100, maxDamage = 250, effect = 13}
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)