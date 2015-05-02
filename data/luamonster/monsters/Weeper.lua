local mType = Game.createMonsterType("Weeper")
local monster = {}
monster.description = "weeper"
monster.experience = 4800
monster.outfit = {
	lookType = 489
}

monster.health = 6800
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 17252
monster.speed = 260
monster.runHealth = 570
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
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 70
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Moooaaan!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 99, chance = 50000},
	{id = "bronze amulet", maxCount = 7, subType = 200, chance = 100000},
	{id = "blazing bone", chance = 14890},
	{id = "eye of a weeper", chance = 15870},
	{id = "fiery heart", chance = 13570},
	{id = "magma clump", chance = 13240},
	{id = "prismatic bolt", maxCount = 5, chance = 10900},
	{id = "great mana potion", maxCount = 2, chance = 14800},
	{id = "ultimate health potion", maxCount = 2, chance = 15440},
	{id = "brown crystal splinter", maxCount = 2, chance = 12100},
	{id = "small ruby", maxCount = 3, chance = 15690},
	{id = "wand of everblazing", chance = 1480},
	{id = "red crystal fragment", chance = 8880},
	{id = "violet crystal shard", chance = 5430},
	{id = "fire sword", chance = 3280},
	{id = "fire axe", chance = 2150},
	{id = "magma legs", chance = 790},
	{id = "magma coat", chance = 790},
	{id = "coal", chance = 1320}
}

monster.attacks = {
	{name = "melee",  attack = 90, skill = 90, minDamage = 0, maxDamage = -450, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = -400, maxDamage = -1000, length = 8, effect = 37},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 100, interval = 3000, minDamage = -80, maxDamage = -250, radius = 3, effect = 16},
	{name = "speed",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, duration = 30000, speedChange = -800, length = 5, spread = 3, effect = 4}
}

monster.defenses = {
	defense = 50,
    armor = 50,
    {name = "invisible",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_DEATHDAMAGE, percent = 5},
	{type = COMBAT_ICEDAMAGE, percent = -5}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = true, combat = false},
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