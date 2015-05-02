local mType = Game.createMonsterType("Damaged Worker Golem")
local monster = {}
monster.description = "a damaged worker golem"
monster.experience = 95
monster.outfit = {
	lookType = 304
}

monster.health = 260
monster.maxHealth = monster.health
monster.race = "energy"
monster.corpse = 9801
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
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Klonk klonk klonk", yell = false},
	{text = "Failure! Failure!", yell = false},
	{text = "Good morning citizen. How may I serve you?", yell = false},
	{text = "Target identified: Rat! Termination initiated!", yell = false},
	{text = "Rrrtttarrrttarrrtta", yell = false},
	{text = "Danger will...chrrr! Danger!", yell = false},
	{text = "Self-diagnosis failed.", yell = false},
	{text = "Aw... chhhrrr orders.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 88, chance = 68810},
	{id = "nail", chance = 1460},
	{id = "iron ore", chance = 400},
	{id = "rusty armor", chance = 790},
	{id = "gear crystal", chance = 200},
	{id = "sword ring", chance = 570}
}

monster.attacks = {
	{name = "melee",  attack = 15, skill = 50, minDamage = 0, maxDamage = -45, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -45, range = 7, shootEffect = 10}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 5, maxDamage = 11, shootEffect = 10, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 25},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 50},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = 50},
	{type = COMBAT_DEATHDAMAGE, percent = 10},
	{type = COMBAT_ICEDAMAGE, percent = 10}
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)