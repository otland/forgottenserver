local mType = Game.createMonsterType("Annihilon")
local monster = {}
monster.description = "annihilon"
monster.experience = 10000
monster.outfit = {
	lookFeet = 96,
	lookLegs = 96,
	lookHead = 19,
	lookType = 12,
	lookBody = 104
}

monster.health = 60000
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 6068
monster.speed = 380
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 8
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
	staticAttack = 85
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Flee as long as you can!", yell = false},
	{text = "Annihilon's might will crush you all!", yell = false},
	{text = "I am coming for you!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "rusty armor", chance = 50000},
	{id = "giant shimmering pearl", maxCount = 2, chance = 33333},
	{id = "heavy mace", chance = 25000},
	{id = "blue gem", chance = 20000},
	{id = "emerald bangle", chance = 20000},
	{id = "yellow gem", chance = 20000},
	{id = "infernal bolt", maxCount = 46, chance = 20000},
	{id = "flaming arrow", maxCount = 46, chance = 20000},
	{id = "gold ingot", chance = 20000},
	{id = "guardian halberd", chance = 20000},
	{id = "halberd", chance = 20000},
	{id = "red gem", chance = 20000},
	{id = "soul orb", maxCount = 5, chance = 20000},
	{id = "assassin star", maxCount = 50, chance = 16666},
	{id = "violet gem", chance = 16666},
	{id = "berserk potion", chance = 16666},
	{id = "power bolt", maxCount = 94, chance = 16666},
	{id = "viper star", maxCount = 70, chance = 16666},
	{id = "platinum coin", maxCount = 30, chance = 16666},
	{id = "onyx flail", chance = 14285},
	{id = "mastermind potion", chance = 14285},
	{id = "ultimate health potion", chance = 14285},
	{id = "great health potion", chance = 14285},
	{id = "great spirit potion", chance = 14285},
	{id = "green gem", chance = 12500},
	{id = "demon horn", maxCount = 2, chance = 12500},
	{id = "crown shield", chance = 11111},
	{id = "great mana potion", chance = 11111},
	{id = "paladin armor", chance = 10000},
	{id = "tower shield", chance = 9090},
	{id = "guardian shield", chance = 7692},
	{id = "diamond sceptre", chance = 7142},
	{id = "demon shield", chance = 4166},
	{id = "mastermind shield", chance = 4166},
	{id = "lavos armor", chance = 1851},
	{id = "demonbone", chance = 1234},
	{id = "obsidian truncheon", chance = 1234},
	{id = "obsidian truncheon", chance = 1234},
	{id = "rusty armor", chance = 1234}
}

monster.attacks = {
	{name = "melee",  attack = 175, skill = 185, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 11, interval = 1000, minDamage = 0, maxDamage = -600, length = 8, effect = 18},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 15, interval = 2000, minDamage = -200, maxDamage = -700, radius = 4, effect = 42},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 18, interval = 3000, minDamage = -50, maxDamage = -255, target = true, radius = 5, effect = 35},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = -50, maxDamage = -600, target = true, radius = 6, shootEffect = 4, effect = 7}
}

monster.defenses = {
	defense = 55,
    armor = 60,
    {name = "combat", type = COMBAT_HEALING,  chance = 14, interval = 1000, minDamage = 400, maxDamage = 900, radius = 6, shootEffect = 4, effect = 15},
	{name = "speed",  chance = 4, interval = 1000, minDamage = 0, maxDamage = 0, duration = 7000, speedChange = 500, radius = 6, shootEffect = 4, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 96},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = 95},
	{type = COMBAT_ICEDAMAGE, percent = 20}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)