local mType = Game.createMonsterType("Dragonling")
local monster = {}
monster.description = "a dragonling"
monster.experience = 2200
monster.outfit = {
	lookType = 505
}

monster.health = 2600
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 18438
monster.speed = 330
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
	{text = "FCHHHHHHHHHHHHHHHH", yell = false},
	{text = "FI?", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 37500},
	{id = "gold coin", maxCount = 100, chance = 37500},
	{id = "gold coin", maxCount = 27, chance = 37500},
	{id = "strong mana potion", maxCount = 2, chance = 16900},
	{id = "strong health potion", maxCount = 2, chance = 24480},
	{id = "blazing bone", chance = 12410}
}

monster.attacks = {
	{name = "melee",  attack = 60, skill = 60, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 2000, minDamage = -120, maxDamage = -250, length = 5, spread = 2, shootEffect = 4, effect = 16},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 20, interval = 2000, minDamage = -115, maxDamage = -180, radius = 3, shootEffect = 4, effect = 48},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 20, interval = 2000, minDamage = -95, maxDamage = -150, radius = 3, shootEffect = 4, effect = 6},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, target = true, duration = 16000, speedChange = -700, radius = 1, shootEffect = 41, effect = 32}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 100, maxDamage = 200, radius = 1, shootEffect = 41, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = -5}
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)