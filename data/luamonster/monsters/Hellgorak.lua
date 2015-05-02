local mType = Game.createMonsterType("Hellgorak")
local monster = {}
monster.description = "a hellgorak"
monster.experience = 10000
monster.outfit = {
	lookFeet = 81,
	lookLegs = 21,
	lookHead = 19,
	lookType = 12,
	lookBody = 96
}

monster.health = 40000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6068
monster.speed = 360
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
	chance = 0,
	{text = "I'll sacrifice yours souls to seven!", yell = false},
	{text = "I'm bad news for you mortals!", yell = false},
	{text = "No man can defeat me!", yell = false},
	{text = "Your puny skills are no match for me.", yell = false},
	{text = "I smell your fear.", yell = false},
	{text = "Delicious!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 80000},
	{id = "gold coin", maxCount = 100, chance = 70000},
	{id = "crystal coin", maxCount = 3, chance = 100000},
	{id = "white pearl", maxCount = 7, chance = 3000},
	{id = "mastermind shield", chance = 1650},
	{id = "great mana potion", chance = 7000},
	{id = "great health potion", chance = 7000},
	{id = "gold coin", maxCount = 83, chance = 60000},
	{id = "assassin star", maxCount = 25, chance = 100000}
}

monster.attacks = {
	{name = "melee",  attack = 130, skill = 130, minDamage = 0, maxDamage = -910, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 11, interval = 1000, minDamage = -250, maxDamage = -819, length = 8, effect = 48},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 14, interval = 2000, minDamage = -90, maxDamage = -500, radius = 5, effect = 32},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 11, interval = 1000, minDamage = -50, maxDamage = -520, target = true, radius = 5, effect = 7},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 5, interval = 2000, minDamage = 0, maxDamage = -150, radius = 7, effect = 3}
}

monster.defenses = {
	defense = 65,
    armor = 70,
    {name = "combat", type = COMBAT_HEALING,  chance = 11, interval = 1000, minDamage = 400, maxDamage = 900, effect = 15}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 99},
	{type = COMBAT_EARTHDAMAGE, percent = 99},
	{type = COMBAT_FIREDAMAGE, percent = 99},
	{type = COMBAT_DEATHDAMAGE, percent = 99},
	{type = COMBAT_ICEDAMAGE, percent = 99},
	{type = COMBAT_HOLYDAMAGE, percent = 99},
	{type = COMBAT_DROWNDAMAGE, percent = -50}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)