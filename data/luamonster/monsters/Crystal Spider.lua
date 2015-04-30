local mType = Game.createMonsterType("Crystal Spider")
local monster = {}
monster.description = "a crystal spider"
monster.experience = 900
monster.outfit = {
	lookType = 263
}

monster.health = 1250
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 7344
monster.speed = 220
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 15
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
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Screeech!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 92, chance = 50000},
	{id = "strong mana potion", chance = 14810},
	{id = "plate armor", chance = 10000},
	{id = "sniper arrow", maxCount = 6, chance = 6250},
	{id = "plate armor", chance = 5263},
	{id = "steel helmet", chance = 4545},
	{id = "crystal sword", chance = 2564},
	{id = "crystal necklace", chance = 1730},
	{id = "crystal ring", chance = 1560},
	{id = "spider silk", chance = 1950},
	{id = "time ring", chance = 1587},
	{id = "shard", chance = 750},
	{id = "knight legs", chance = 690},
	{id = "knight armor", chance = 540},
	{id = "glacier mask", chance = 650},
	{id = "platinum amulet", chance = 120},
	{id = "sapphire hammer", chance = 130},
	{id = "jewelled backpack", chance = 70}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 15000, speedChange = -800, range = 7, radius = 6, effect = 3},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 15, interval = 2000, minDamage = -50, maxDamage = -100, range = 7, radius = 6, shootEffect = 29, effect = 42},
	{name = "speed",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, duration = 10000, speedChange = -600, range = 7, radius = 6, shootEffect = 13, effect = 42}
}

monster.defenses = {
	defense = 40,
    armor = 40,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 250, radius = 6, shootEffect = 13, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -20},
	{type = COMBAT_EARTHDAMAGE, percent = 20}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = true, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
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