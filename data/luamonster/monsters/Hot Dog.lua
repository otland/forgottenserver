local mType = Game.createMonsterType("Hot Dog")
local monster = {}
monster.description = "a hot dog"
monster.experience = 190
monster.outfit = {
	lookType = 32
}

monster.health = 505
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5971
monster.speed = 300
monster.maxSummons = 0

monster.changeTarget = {
    interval = 2000,
	chance = 0
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
	{text = "Wuff Wuff", yell = false},
	{text = "Grrr Wuff", yell = false},
	{text = "Show me how good you are without some rolled newspaper!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 75, chance = 50000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -55, interval = 1200, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 30, interval = 2000, minDamage = -30, maxDamage = -60, length = 8, spread = 3, effect = 16},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 30, interval = 1000, minDamage = -50, maxDamage = -50, range = 7, effect = 37}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -20},
	{type = COMBAT_HOLYDAMAGE, percent = -5}
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
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)