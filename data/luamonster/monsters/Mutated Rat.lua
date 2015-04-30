local mType = Game.createMonsterType("Mutated Rat")
local monster = {}
monster.description = "a mutated rat"
monster.experience = 450
monster.outfit = {
	lookType = 305
}

monster.health = 550
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 9871
monster.speed = 220
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
	{text = "Grrrrrrrrrrrrrr!", yell = false},
	{text = "Fcccccchhhhhh", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 65, chance = 38000},
	{id = "gold coin", maxCount = 65, chance = 40000},
	{id = "skull of Ratha", chance = 20180},
	{id = "mouldy cheese", chance = 930},
	{id = "stone herb", chance = 4920},
	{id = "green mushroom", chance = 1390},
	{id = "plate shield", chance = 3750},
	{id = "halberd", chance = 3000},
	{id = "stealth ring", chance = 540},
	{id = "spellbook of enlightenment", chance = 290},
	{id = "health potion", chance = 560},
	{id = "tower shield", chance = 60},
	{id = "mutated rat tail", chance = 3770}
}

monster.attacks = {
	{name = "melee",  attack = 45, skill = 60, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -45, maxDamage = -85, range = 7, shootEffect = 15, effect = 21},
	{name = "condition", type = CONDITION_POISON,  chance = 10, interval = 2000, minDamage = -80, maxDamage = -100, length = 5, shootEffect = 15, effect = 21},
	{name = "speed",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, duration = 30000, speedChange = -600, range = 7, shootEffect = 15, effect = 14},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -30, maxDamage = -70, range = 7, radius = 3, shootEffect = 15, effect = 14},
	{name = "condition", type = CONDITION_POISON,  chance = 10, interval = 2000, minDamage = -80, maxDamage = 0, range = 7, radius = 3, shootEffect = 15, effect = 21}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 5, interval = 2000, minDamage = 80, maxDamage = 95, radius = 3, shootEffect = 15, effect = 13}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = true, combat = false},
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