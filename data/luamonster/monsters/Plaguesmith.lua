local mType = Game.createMonsterType("Plaguesmith")
local monster = {}
monster.description = "a plaguesmith"
monster.experience = 4500
monster.outfit = {
	lookType = 247
}

monster.health = 8250
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 6516
monster.speed = 240
monsters.runHealth = 500
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
	staticAttack = 70
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "You are looking a bit feverish!", yell = false},
	{text = "You don't look that good!", yell = false},
	{text = "Hachoo!", yell = false},
	{text = "Cough Cough", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 100, chance = 40000},
	{id = "gold coin", maxCount = 65, chance = 50000},
	{id = "dirty cape", chance = 60000},
	{id = "mouldy cheese", chance = 50000},
	{id = "morning star", chance = 29000},
	{id = "two handed sword", chance = 20000},
	{id = "steel shield", chance = 20000},
	{id = "piece of iron", chance = 20000},
	{id = "battle hammer", chance = 20000},
	{id = "soul orb", chance = 11111},
	{id = "great health potion", chance = 10000},
	{id = "demonic essence", chance = 9033},
	{id = "onyx arrow", maxCount = 4, chance = 7692},
	{id = "rusty armor", chance = 540},
	{id = "platinum coin", maxCount = 2, chance = 7142},
	{id = "knight legs", chance = 6250},
	{id = "small amethyst", maxCount = 3, chance = 5000},
	{id = "club ring", chance = 4761},
	{id = "axe ring", chance = 4347},
	{id = "war hammer", chance = 2127},
	{id = "silver brooch", chance = 2000},
	{id = "piece of royal steel", chance = 1234},
	{id = "steel boots", chance = 1123},
	{id = "piece of draconian steel", chance = 1030},
	{id = "piece of hell steel", chance = 1010},
	{id = "hammer of wrath", chance = 952},
	{id = "emerald bangle", chance = 341},
	{id = "cornucopia", chance = 100}
}

monster.attacks = {
	{name = "melee",  attack = 98, skill = 100, minDamage = 0, maxDamage = -539, interval = 1500, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -60, maxDamage = -114, radius = 4, effect = 21},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = -100, maxDamage = -350, length = 5, spread = 3, effect = 8},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 30000, speedChange = -800, radius = 4, effect = 21}
}

monster.defenses = {
	defense = 40,
    armor = 40,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 200, maxDamage = 280, effect = 13},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 440, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = 1},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 30}
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