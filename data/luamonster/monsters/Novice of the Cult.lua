local mType = Game.createMonsterType("Novice of the Cult")
local monster = {}
monster.description = "a novice of the cult"
monster.experience = 100
monster.outfit = {
	lookFeet = 114,
	lookLegs = 114,
	lookHead = 114,
	lookType = 133,
	lookBody = 95
}

monster.health = 285
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20467
monster.speed = 200
monster.maxSummons = 1

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
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.summons = {
    {name = "Chicken", chance = 10, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Fear us!", yell = false},
	{text = "You will not tell anyone what you have seen!", yell = false},
	{text = "Your curiosity will be punished!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 40, chance = 43380},
	{id = "rope belt", chance = 5910},
	{id = "scarf", chance = 2900},
	{id = "cultish robe", chance = 1030},
	{id = "music sheet", chance = 970},
	{id = "book", chance = 700},
	{id = "pirate voodoo doll", chance = 520},
	{id = "dwarven ring", chance = 500},
	{id = "wand of vortex", chance = 450},
	{id = "garlic necklace", subType = 150, chance = 420},
	{id = "small diamond", chance = 210}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -20, maxDamage = -80, target = true, range = 7, radius = 1, shootEffect = 15, effect = 14}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 20, maxDamage = 40, radius = 1, shootEffect = 15, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_ENERGYDAMAGE, percent = -8},
	{type = COMBAT_EARTHDAMAGE, percent = 10},
	{type = COMBAT_FIREDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE, percent = 10},
	{type = COMBAT_DEATHDAMAGE, percent = -8},
	{type = COMBAT_ICEDAMAGE, percent = 10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = false, combat = false},
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