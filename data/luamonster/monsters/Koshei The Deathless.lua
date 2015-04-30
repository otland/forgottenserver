local mType = Game.createMonsterType("Koshei The Deathless")
local monster = {}
monster.description = "koshei the deathless"
monster.experience = 0
monster.outfit = {
	lookFeet = 115,
	lookLegs = 119,
	lookHead = 95,
	lookType = 99,
	lookBody = 116
}

monster.health = 2000
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6028
monster.speed = 390
monster.maxSummons = 1

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
	staticAttack = 90
}

monster.summons = {
    {name = "bonebeast", chance = 16, interval = 1000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Your pain will be beyond imagination!", yell = false},
	{text = "You can't defeat me! I will ressurect and take your soul!", yell = false},
	{text = "Death is my ally!", yell = false},
	{text = "Welcome to my domain, visitor!", yell = false},
	{text = "You will be my toy on the other side!", yell = false},
	{text = "What a disgusting smell of life!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "staff", chance = 10000},
	{id = "platinum amulet", chance = 1666},
	{id = "blue robe", chance = 709},
	{id = "lightning boots", chance = 688},
	{id = "castle shield", chance = 588},
	{id = "stealth ring", chance = 888},
	{id = "dirty cape", chance = 10000},
	{id = "gold coin", maxCount = 12, chance = 80000},
	{id = "spellbook", chance = 10000},
	{id = "gold ring", chance = 900},
	{id = "lich staff", chance = 1000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 9, interval = 3000, minDamage = -60, maxDamage = -250, range = 1, effect = 13},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 11, interval = 1000, minDamage = -70, maxDamage = -135, radius = 3, effect = 14},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 9, interval = 2000, minDamage = -50, maxDamage = -140, length = 8, radius = 3, effect = 18},
	{name = "condition", type = CONDITION_CURSED,  chance = 15, interval = 3000, minDamage = -54, maxDamage = -54, range = 1, radius = 3, effect = 18},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 30000, speedChange = -900, range = 7, radius = 3, effect = 14}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 30, interval = 1000, minDamage = 150, maxDamage = 300, radius = 3, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 80},
	{type = COMBAT_HOLYDAMAGE, percent = -15},
	{type = COMBAT_FIREDAMAGE, percent = -10}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)