local mType = Game.createMonsterType("Vulcongra")
local monster = {}
monster.description = "vulcongra"
monster.experience = 1100
monster.outfit = {
	lookType = 509
}

monster.health = 1600
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 18476
monster.speed = 230
monsters.runHealth = 220
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
	{text = "Fuchah! Fuchah!", yell = false},
	{text = "Fuchah!", yell = false},
	{text = "Yag! Yag! Yag!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 7, chance = 50000},
	{id = "platinum coin", chance = 14780},
	{id = "banana", maxCount = 10, chance = 10420},
	{id = "wand of inferno", chance = 1000},
	{id = "sword ring", chance = 2920},
	{id = "fire sword", chance = 260},
	{id = "magma coat", chance = 140},
	{id = "magma amulet", subType = 200, chance = 970},
	{id = "fiery heart", chance = 9380},
	{id = "magma clump", chance = 12440},
	{id = "blazing bone", chance = 8030},
	{id = "strong health potion", chance = 7100},
	{id = "strong mana potion", chance = 7100},
	{id = "red crystal fragment", chance = 2280},
	{id = "coal", chance = 940},
	{id = "brown crystal splinter", chance = 4850}
}

monster.attacks = {
	{name = "melee",  attack = 67, skill = 60, minDamage = 0, maxDamage = -235, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 2000, minDamage = -195, maxDamage = -340, length = 8, effect = 6},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = -100, maxDamage = -200, range = 7, shootEffect = 4, effect = 6},
	{name = "vulcongra soulfire",  chance = 100, interval = 3000, minDamage = 0, maxDamage = 0, shootEffect = 4, effect = 6}
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