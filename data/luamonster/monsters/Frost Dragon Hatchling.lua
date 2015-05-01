local mType = Game.createMonsterType("Frost Dragon Hatchling")
local monster = {}
monster.description = "a frost dragon hatchling"
monster.experience = 745
monster.outfit = {
	lookType = 283
}

monster.health = 800
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 7969
monster.speed = 200
monsters.runHealth = 80
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
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Rooawwrr", yell = false},
	{text = "Fchu?", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 55, chance = 86750},
	{id = "dragon ham", chance = 79600},
	{id = "spellbook of enlightenment", chance = 400},
	{id = "health potion", chance = 560},
	{id = "frosty heart", chance = 5000}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 70, minDamage = 0, maxDamage = -160, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 10, interval = 2000, minDamage = -60, maxDamage = -110, length = 5, spread = 2, effect = 44},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 15, interval = 2000, minDamage = -60, maxDamage = -110, target = true, radius = 4, effect = 42},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, target = true, duration = 12000, speedChange = -600, radius = 4, shootEffect = 29, effect = 42}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 45, maxDamage = 50, shootEffect = 29, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -5}
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
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)