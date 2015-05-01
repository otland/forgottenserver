local mType = Game.createMonsterType("Sandstone Scorpion")
local monster = {}
monster.description = "a sandstone scorpion"
monster.experience = 680
monster.outfit = {
	lookType = 398
}

monster.health = 900
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 13501
monster.speed = 230
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
	{text = "*rattle*", yell = false},
	{text = "*tak tak*", yell = false},
	{text = "*tak tak tak*", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 128, chance = 90000},
	{id = "platinum coin", maxCount = 2, chance = 11111},
	{id = "small emerald", maxCount = 2, chance = 9900},
	{id = "daramian mace", chance = 6250},
	{id = "steel helmet", chance = 5000},
	{id = "black shield", chance = 1724},
	{id = "fist on a stick", chance = 280}
}

monster.attacks = {
	{name = "melee",  attack = 55, skill = 60, minDamage = 0, maxDamage = -193, interval = 2000, effect = 0}
}

monster.defenses = {
	defense = 40,
    armor = 40,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 60, maxDamage = 100, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_ICEDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE, percent = -1},
	{type = COMBAT_FIREDAMAGE, percent = 50}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)