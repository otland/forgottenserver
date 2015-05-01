local mType = Game.createMonsterType("Wiggler")
local monster = {}
monster.description = "a wiggler"
monster.experience = 900
monster.outfit = {
	lookType = 510
}

monster.health = 1200
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 18483
monster.speed = 220
monsters.runHealth = 359
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
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Zzzrp!", yell = false},
	{text = "Crick! Crick!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "crystalline arrow", maxCount = 5, chance = 15540},
	{id = "drill bolt", maxCount = 5, chance = 15260},
	{id = "platinum coin", chance = 14790},
	{id = "white mushroom", maxCount = 5, chance = 7142},
	{id = "strong mana potion", chance = 5250},
	{id = "strong health potion", chance = 4930},
	{id = "green crystal splinter", chance = 3600},
	{id = "blue piece of cloth", chance = 2080},
	{id = "yellow piece of cloth", chance = 2160},
	{id = "green crystal fragment", chance = 2070},
	{id = "serpent sword", chance = 1360},
	{id = "black shield", chance = 1320},
	{id = "terra rod", chance = 850}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 70, minDamage = 0, maxDamage = -200, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = -180, maxDamage = -270, length = 4, spread = 3},
	{name = "condition", type = CONDITION_POISON,  chance = 10, interval = 2000, minDamage = -160, maxDamage = -200, range = 7, shootEffect = 6, effect = 46},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 30000, speedChange = -700, length = 3, spread = 2, shootEffect = 6, effect = 10}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 510, shootEffect = 6, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 5},
	{type = COMBAT_DEATHDAMAGE, percent = 10},
	{type = COMBAT_FIREDAMAGE, percent = -5}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)