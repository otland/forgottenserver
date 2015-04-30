local mType = Game.createMonsterType("Souleater")
local monster = {}
monster.description = "a souleater"
monster.experience = 1300
monster.outfit = {
	lookType = 355
}

monster.health = 1100
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 12631
monster.speed = 200
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
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Life is such a fickle thing!", yell = false},
	{text = "I will devour your soul.", yell = false},
	{text = "Souuuls!", yell = false},
	{text = "I will feed on you.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 33400},
	{id = "gold coin", maxCount = 100, chance = 33400},
	{id = "platinum coin", maxCount = 6, chance = 49610},
	{id = "wand of cosmic energy", chance = 910},
	{id = "ectoplasmic sushi", chance = 2000},
	{id = "great mana potion", chance = 8000},
	{id = "ultimate health potion", chance = 9400},
	{id = "lizard essence", chance = 15250},
	{id = "necrotic rod", chance = 980},
	{id = "souleater trophy", chance = 20},
	{id = "death ring", chance = 300},
	{id = "spirit container", chance = 140}
}

monster.attacks = {
	{name = "melee",  attack = 60, skill = 60, interval = 2000, effect = 0},
	{name = "icicle",  chance = 20, interval = 2000, minDamage = -50, maxDamage = -100, range = 7},
	{name = "souleater drown",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 10, interval = 2000, minDamage = -100, maxDamage = -200, length = 4, spread = 3, effect = 20},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 25, interval = 2000, minDamage = -30, maxDamage = -60, radius = 4, effect = 14}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "invisible",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, radius = 4, effect = 13},
	{name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 120, maxDamage = 125, radius = 4, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 70},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = -10},
	{type = COMBAT_ICEDAMAGE, percent = 50}
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)