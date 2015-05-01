local mType = Game.createMonsterType("Ironblight")
local monster = {}
monster.description = "ironblight"
monster.experience = 4400
monster.outfit = {
	lookType = 498
}

monster.health = 6600
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 17424
monster.speed = 220
monsters.runHealth = 260
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
	{text = "Clonk!", yell = false},
	{text = "Yowl!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 199, chance = 100000},
	{id = "platinum coin", maxCount = 8, chance = 100000},
	{id = "small amethyst", maxCount = 3, chance = 15020},
	{id = "small emerald", maxCount = 3, chance = 10890},
	{id = "great mana potion", chance = 17640},
	{id = "war crystal", chance = 21230},
	{id = "ultimate health potion", chance = 18330},
	{id = "shiny stone", chance = 15230},
	{id = "red gem", chance = 3510},
	{id = "blue gem", chance = 480},
	{id = "red crystal fragment", chance = 10540},
	{id = "green crystal fragment", chance = 10540},
	{id = "green crystal shard", chance = 5720},
	{id = "brown crystal splinter", maxCount = 2, chance = 12610},
	{id = "crystalline spikes", chance = 18400},
	{id = "crystal of balance", chance = 4480},
	{id = "crystal of power", chance = 2480},
	{id = "magic sulphur", chance = 830},
	{id = "twigs", chance = 620},
	{id = "epee", chance = 480},
	{id = "sapphire hammer", chance = 480},
	{id = "composite hornbow", chance = 210},
	{id = "glacial rod", chance = 960},
	{id = "springsprout rod", chance = 1720},
	{id = "jade hat", chance = 210},
	{id = "terra legs", chance = 1110}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 50, minDamage = 0, maxDamage = -300, interval = 2000, effect = 0},
	{name = "condition", type = CONDITION_POISON,  chance = 10, interval = 2000, minDamage = -460, maxDamage = -480, radius = 6, shootEffect = 15, effect = 21},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 15, interval = 2000, minDamage = -260, maxDamage = -350, length = 7, shootEffect = 29, effect = 44},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = -180, maxDamage = -250, target = true, radius = 2, shootEffect = 20, effect = 51},
	{name = "speed",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, duration = 30000, speedChange = -800, length = 5, spread = 3, shootEffect = 20, effect = 4}
}

monster.defenses = {
	defense = 35,
    armor = 35,
    {name = "invisible",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 20, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 25},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_DEATHDAMAGE, percent = 50},
	{type = COMBAT_FIREDAMAGE, percent = 60}
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