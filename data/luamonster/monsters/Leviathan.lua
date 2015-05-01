local mType = Game.createMonsterType("Leviathan")
local monster = {}
monster.description = "a leviathan"
monster.experience = 5000
monster.outfit = {
	lookType = 275
}

monster.health = 6000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 8307
monster.speed = 758
monsters.runHealth = 600
monster.maxSummons = 0

monster.changeTarget = {
    interval = 2000,
	chance = 50
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
	staticAttack = 95
}

monster.voices = {
	interval = 3000,
	chance = 200,
	{text = "CHHHRRRR", yell = false},
	{text = "HISSSS", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 80, chance = 40000},
	{id = "platinum coin", maxCount = 6, chance = 40000},
	{id = "small sapphire", maxCount = 3, chance = 10000},
	{id = "gold coin", maxCount = 80, chance = 40000}
}

monster.attacks = {
	{name = "melee",  attack = 70, skill = 75, minDamage = 0, maxDamage = -298, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 8, interval = 1000, minDamage = -130, maxDamage = -460, length = 9, spread = 3, effect = 51},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 10, interval = 1000, minDamage = -365, maxDamage = -491, length = 9, spread = 3, effect = 42},
	{name = "combat", type = COMBAT_DROWNDAMAGE,  chance = 11, interval = 1000, minDamage = -15, maxDamage = -20, target = true, radius = 4, effect = 2}
}

monster.defenses = {
	defense = 30,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 1000, minDamage = 50, maxDamage = 150, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 30},
	{type = COMBAT_DEATHDAMAGE, percent = -10}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
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
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)