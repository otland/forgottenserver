local mType = Game.createMonsterType("Diblis the fair")
local monster = {}
monster.description = "a vampire lord"
monster.experience = 1800
monster.outfit = {
	lookType = 287
}

monster.health = 1600
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 8937
monster.speed = 280
monster.maxSummons = 4

monster.changeTarget = {
    interval = 5000,
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

monster.summons = {
    {name = "Banshee", chance = 50, interval = 4500}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "I envy you to be slain by someone as beautiful as me.", yell = false},
	{text = "I will drain your ugly corpses of the last drop of blood.", yell = false}
}

monster.loot = {
    {id = "skull", chance = 15000},
	{id = "strong health potion", chance = 1500},
	{id = "black pearl", maxCount = 2, chance = 8900},
	{id = "platinum coin", maxCount = 5, chance = 50000},
	{id = "gold coin", maxCount = 99, chance = 100000},
	{id = "ring of healing (faster regeneration)", chance = 14111},
	{id = "scum bag", chance = 500},
	{id = "vampire lord token", chance = 100000},
	{id = "vampire shield", chance = 2100},
	{id = "spellbook of lost souls", chance = 300}
}

monster.attacks = {
	{name = "melee",  attack = 95, skill = 70, minDamage = 0, maxDamage = -380, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 12, interval = 1000, minDamage = 0, maxDamage = -155, shootEffect = 32, effect = 18}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 12, interval = 1000, minDamage = 100, maxDamage = 235, shootEffect = 32, effect = 13},
	{name = "invisible",  chance = 25, interval = 3000, minDamage = 0, maxDamage = 0, shootEffect = 32, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = -15},
	{type = COMBAT_FIREDAMAGE, percent = -10}
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)