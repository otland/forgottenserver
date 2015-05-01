local mType = Game.createMonsterType("Magma Crawler")
local monster = {}
monster.description = "a magma crawler"
monster.experience = 2700
monster.outfit = {
	lookType = 492
}

monster.health = 4800
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 17336
monster.speed = 220
monster.runHealth = 300
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
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Crrroak!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 99, chance = 50000},
	{id = "platinum coin", maxCount = 5, chance = 100000},
	{id = "blazing bone", chance = 11500},
	{id = "white piece of cloth", chance = 2310},
	{id = "yellow piece of cloth", chance = 2980},
	{id = "red piece of cloth", chance = 930},
	{id = "wand of everblazing", chance = 690},
	{id = "energy ring", chance = 1650},
	{id = "crystalline arrow", maxCount = 10, chance = 5950},
	{id = "magma boots", chance = 1820},
	{id = "small diamond", maxCount = 3, chance = 8800},
	{id = "magma amulet", subType = 200, chance = 3120},
	{id = "great health potion", chance = 7270},
	{id = "black shield", chance = 1550},
	{id = "blazing bone", chance = 12220},
	{id = "wand of draconia", chance = 4280},
	{id = "great mana potion", chance = 6500},
	{id = "iron ore", chance = 4280},
	{id = "yellow gem", chance = 1030},
	{id = "fiery heart", chance = 7810},
	{id = "magma clump", chance = 11600},
	{id = "coal", chance = 1675},
	{id = "blue crystal splinter", maxCount = 2, chance = 8500},
	{id = "brown crystal splinter", maxCount = 2, chance = 8500},
	{id = "green crystal fragment", chance = 7000},
	{id = "blue crystal shard", maxCount = 2, chance = 3930},
	{id = "fire sword", chance = 1680}
}

monster.attacks = {
	{name = "melee",  attack = 58, skill = 60, minDamage = 0, maxDamage = -204, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 10, interval = 2000, minDamage = -300, maxDamage = -1100, length = 8, effect = 18},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = -290, maxDamage = -800, length = 3, spread = 2, effect = 37},
	{name = "magma crawler soulfire",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, effect = 37},
	{name = "soulfire",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, effect = 37},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = -140, maxDamage = -180, radius = 3, effect = 16},
	{name = "speed",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, duration = 20000, speedChange = -800, radius = 2, effect = 14}
}

monster.defenses = {
	defense = 45,
    armor = 45,
    {name = "invisible",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 5},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_DEATHDAMAGE, percent = 5},
	{type = COMBAT_ICEDAMAGE, percent = 10}
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)