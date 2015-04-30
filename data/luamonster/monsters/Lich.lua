local mType = Game.createMonsterType("Lich")
local monster = {}
monster.description = "a lich"
monster.experience = 900
monster.outfit = {
	lookFeet = 115,
	lookLegs = 119,
	lookHead = 95,
	lookType = 99,
	lookBody = 116
}

monster.health = 880
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6028
monster.speed = 220
monster.maxSummons = 4

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
    {name = "Bonebeast", chance = 10, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Doomed be the living!", yell = false},
	{text = "Death awaits all!", yell = false},
	{text = "Thy living flesh offends me!", yell = false},
	{text = "Death and Decay!", yell = false},
	{text = "You will endure agony beyond thy death!", yell = false},
	{text = "Pain sweet pain!", yell = false},
	{text = "Come to me my children!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 139, chance = 100000},
	{id = "platinum coin", chance = 19720},
	{id = "spellbook", chance = 10000},
	{id = "strong mana potion", chance = 7500},
	{id = "black pearl", maxCount = 3, chance = 5960},
	{id = "white pearl", chance = 5000},
	{id = "ancient shield", chance = 2422},
	{id = "small topaz", maxCount = 3, chance = 2430},
	{id = "small emerald", maxCount = 3, chance = 2230},
	{id = "ring of healing", chance = 1540},
	{id = "strange helmet", chance = 740},
	{id = "yellow gem", chance = 690},
	{id = "skull staff", chance = 550},
	{id = "platinum amulet", chance = 450},
	{id = "castle shield", chance = 350},
	{id = "mind stone", chance = 350},
	{id = "lightning boots", chance = 200},
	{id = "blue robe", chance = 150},
	{id = "maxilla maximus", chance = 100}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -140, maxDamage = -190, length = 7, effect = 14},
	{name = "condition", type = CONDITION_POISON,  chance = 10, interval = 2000, minDamage = -300, maxDamage = -400, length = 7, effect = 17},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -200, maxDamage = -245, range = 1, effect = 14},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 30000, speedChange = -300, range = 7, effect = 14},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -130, maxDamage = -195, radius = 3, effect = 14}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 80, maxDamage = 100, radius = 3, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 80},
	{type = COMBAT_HOLYDAMAGE, percent = -10}
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)