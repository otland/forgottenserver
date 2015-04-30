local mType = Game.createMonsterType("Infected Weeper")
local monster = {}
monster.description = "infected weeper"
monster.experience = 4800
monster.outfit = {
	lookType = 489
}

monster.health = 6800
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 17251
monster.speed = 250
monster.maxSummons = 6

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
	staticAttack = 80
}

monster.summons = {
    {name = "Parasite", chance = 10, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Moooaaan!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 198, chance = 100000},
	{id = "platinum coin", maxCount = 7, chance = 100000},
	{id = "coal", chance = 1460}
}

monster.attacks = {
	{name = "melee",  attack = 70, skill = 70, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -250, maxDamage = -700, length = 5, spread = 3, effect = 14},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 30, interval = 2000, minDamage = -80, maxDamage = -250, radius = 3, effect = 16},
	{name = "speed",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, duration = 30000, speedChange = -800, length = 5, spread = 3, radius = 3, effect = 4}
}

monster.defenses = {
	defense = 45,
    armor = 45,
    {name = "invisible",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_DEATHDAMAGE, percent = 5},
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)