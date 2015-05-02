local mType = Game.createMonsterType("Adept of the Cult")
local monster = {}
monster.description = "an adept of the cult"
monster.experience = 400
monster.outfit = {
	lookFeet = 57,
	lookLegs = 94,
	lookHead = 114,
	lookType = 194,
	lookBody = 94
}

monster.health = 430
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20311
monster.speed = 215
monster.maxSummons = 2

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
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 90
}

monster.summons = {
    {name = "Ghoul", chance = 10, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Feel the power of the cult!", yell = false},
	{text = "Praise the voodoo!", yell = false},
	{text = "Power to the cult!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 60, chance = 65520},
	{id = "cultish robe", chance = 10080},
	{id = "rope belt", chance = 10000},
	{id = "pirate voodoo doll", chance = 1730},
	{id = "silver amulet", subType = 200, chance = 1020},
	{id = "clerical mace", chance = 1260},
	{id = "book", chance = 940},
	{id = "music sheet", chance = 700},
	{id = "amber staff", chance = 680},
	{id = "time ring", chance = 420},
	{id = "small ruby", chance = 320},
	{id = "hailstorm rod", chance = 220},
	{id = "broken key ring", chance = 120},
	{id = "lunar staff", chance = 120},
	{id = "red robe", chance = 80},
	{id = "cultish symbol", chance = 90}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -90, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 20, interval = 2000, minDamage = -70, maxDamage = -150, range = 7, shootEffect = 15, effect = 21},
	{name = "drunk",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 1, shootEffect = 31, effect = 40}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 3000, minDamage = 45, maxDamage = 60, shootEffect = 31, effect = 13},
	{name = "invisible",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 31, effect = 8}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 40},
	{type = COMBAT_HOLYDAMAGE, percent = 30},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = 20}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
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