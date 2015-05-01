local mType = Game.createMonsterType("Crystalcrusher")
local monster = {}
monster.description = "a crystalcrusher"
monster.experience = 500
monster.outfit = {
	lookType = 511
}

monster.health = 570
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 18487
monster.speed = 230
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
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Creak!", yell = false},
	{text = "Crunch!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 90, chance = 85000},
	{id = "brown crystal splinter", chance = 5250},
	{id = "blue crystal splinter", chance = 5000},
	{id = "green crystal splinter", chance = 4920},
	{id = "crystalline arrow", maxCount = 3, chance = 5000},
	{id = "crystalline spikes", chance = 3690}
}

monster.attacks = {
	{name = "melee",  attack = 52, skill = 54, minDamage = 0, maxDamage = -167, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = -110, maxDamage = -260, target = true, radius = 3, effect = 9}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 30, interval = 2000, minDamage = 5, maxDamage = 15, effect = 13},
	{name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 129, maxDamage = 175, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_DEATHDAMAGE, percent = 10},
	{type = COMBAT_ICEDAMAGE, percent = -3},
	{type = COMBAT_HOLYDAMAGE, percent = -1},
	{type = COMBAT_FIREDAMAGE, percent = 15}
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