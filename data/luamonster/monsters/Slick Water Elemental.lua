local mType = Game.createMonsterType("Slick Water Elemental")
local monster = {}
monster.description = "a slick water elemental"
monster.experience = 450
monster.outfit = {
	lookType = 286
}

monster.health = 550
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 8965
monster.speed = 280
monster.runHealth = 1
monster.maxSummons = 0

monster.changeTarget = {
    interval = 20000,
	chance = 15
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 85
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Blubb", yell = false},
	{text = "Splipsh Splash", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 70, chance = 22500},
	{id = "gold coin", maxCount = 60, chance = 22500},
	{id = "shiver arrow", maxCount = 3, chance = 2575},
	{id = "iced soil", chance = 6000}
}

monster.attacks = {
	{name = "melee",  attack = 70, skill = 40, minDamage = 0, maxDamage = -175, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -130, range = 7, shootEffect = 30},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 10, interval = 1000, minDamage = 0, maxDamage = -220, range = 6, shootEffect = 13},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 18, interval = 2000, minDamage = 0, maxDamage = -103, range = 4, shootEffect = 37}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 90, maxDamage = 150, shootEffect = 37, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 40}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
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
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)