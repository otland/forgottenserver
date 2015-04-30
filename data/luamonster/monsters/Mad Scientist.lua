local mType = Game.createMonsterType("Mad Scientist")
local monster = {}
monster.description = "a mad scientist"
monster.experience = 205
monster.outfit = {
	lookFeet = 97,
	lookLegs = 38,
	lookHead = 97,
	lookType = 133,
	lookAddons = 1
}

monster.health = 325
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20439
monster.speed = 190
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
	{text = "Die in the name of Science!", yell = false},
	{text = "You will regret interrupting my studies!", yell = false},
	{text = "Let me test this!", yell = false},
	{text = "I will study your corpse!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 65, chance = 30000},
	{id = "gold coin", maxCount = 50, chance = 30000},
	{id = "mana potion", chance = 19000},
	{id = "health potion", chance = 19000},
	{id = "white mushroom", maxCount = 3, chance = 8000},
	{id = "soulfire rune", subType = 3, chance = 6000},
	{id = "life crystal", chance = 2000},
	{id = "magic light wand", chance = 2000},
	{id = "small enchanted amethyst", chance = 470},
	{id = "elemental ashes", chance = 820},
	{id = "cookie", maxCount = 5, chance = 1200},
	{id = "mastermind potion", chance = 130}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 25, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DROWNDAMAGE,  chance = 15, interval = 2000, minDamage = -20, maxDamage = -56, target = true, range = 7, radius = 3, shootEffect = 39, effect = 3},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -20, maxDamage = -36, radius = 3, shootEffect = 15, effect = 12},
	{name = "speed",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, duration = 2000, speedChange = -300, range = 7, radius = 3, shootEffect = 15, effect = 46}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 10, maxDamage = 30, radius = 3, shootEffect = 15, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE, percent = 20},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = 10}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)