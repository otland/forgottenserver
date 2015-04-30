local mType = Game.createMonsterType("Lizard High Guard")
local monster = {}
monster.description = "a lizard high guard"
monster.experience = 1450
monster.outfit = {
	lookType = 337
}

monster.health = 1800
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 11272
monster.speed = 240
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
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Hizzzzzzz!", yell = false},
	{text = "To armzzzz!", yell = false},
	{text = "Engage zze aggrezzor!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 32000},
	{id = "gold coin", maxCount = 100, chance = 32000},
	{id = "gold coin", maxCount = 27, chance = 32000},
	{id = "platinum coin", maxCount = 2, chance = 4900},
	{id = "tower shield", chance = 1040},
	{id = "high guard flag", chance = 2990},
	{id = "Zaoan shoes", chance = 700},
	{id = "lizard leather", chance = 1000},
	{id = "high guard shoulderplates", chance = 8150},
	{id = "lizard scale", chance = 970},
	{id = "spiked iron ball", chance = 7000},
	{id = "small emerald", maxCount = 4, chance = 2520},
	{id = "strong health potion", chance = 11925},
	{id = "bunch of ripe rice", chance = 4950},
	{id = "great health potion", chance = 7070},
	{id = "red lantern", chance = 1220},
	{id = "Zaoan legs", chance = 720},
	{id = "Zaoan armor", chance = 80}
}

monster.attacks = {
	{name = "melee",  attack = 72, skill = 75, interval = 2000, effect = 0}
}

monster.defenses = {
	defense = 35,
    armor = 35,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 25, maxDamage = 75, effect = 15}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 5},
	{type = COMBAT_ICEDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 45}
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