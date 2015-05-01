local mType = Game.createMonsterType("Quara Predator")
local monster = {}
monster.description = "a quara predator"
monster.experience = 1600
monster.outfit = {
	lookType = 20
}

monster.health = 2200
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6067
monster.speed = 280
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
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Rrrah!", yell = false},
	{text = "Rraaar!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 28000},
	{id = "gold coin", maxCount = 53, chance = 50000},
	{id = "quara bone", chance = 9090},
	{id = "royal spear", maxCount = 7, chance = 9000},
	{id = "small diamond", maxCount = 2, chance = 5160},
	{id = "shrimp", maxCount = 5, chance = 4860},
	{id = "double axe", chance = 3171},
	{id = "fish fin", maxCount = 2, chance = 1940},
	{id = "great health potion", chance = 1000},
	{id = "assassin star", chance = 590},
	{id = "relic sword", chance = 680},
	{id = "glacier robe", chance = 420},
	{id = "skull helmet", chance = 400},
	{id = "giant shrimp", chance = 10}
}

monster.attacks = {
	{name = "melee",  attack = 105, skill = 80, minDamage = 0, maxDamage = -473, interval = 2000, effect = 0}
}

monster.defenses = {
	defense = 45,
    armor = 45,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 270, effect = 15},
	{name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 25, maxDamage = 75, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -25},
	{type = COMBAT_EARTHDAMAGE, percent = -10}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)