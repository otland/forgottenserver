local mType = Game.createMonsterType("Wyrm")
local monster = {}
monster.description = "a wyrm"
monster.experience = 1550
monster.outfit = {
	lookType = 291
}

monster.health = 1825
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 8941
monster.speed = 230
monsters.runHealth = 1
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
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "GRRR!", yell = false},
	{text = "GROOOOAAAAAAAAR!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 30000},
	{id = "gold coin", maxCount = 100, chance = 30000},
	{id = "gold coin", maxCount = 30, chance = 30000},
	{id = "dragon ham", maxCount = 3, chance = 34800},
	{id = "burst arrow", maxCount = 10, chance = 7650},
	{id = "strong health potion", chance = 19970},
	{id = "wand of draconia", chance = 990},
	{id = "small diamond", maxCount = 3, chance = 750},
	{id = "strong mana potion", chance = 15310},
	{id = "crossbow", chance = 5920},
	{id = "lightning pendant", subType = 200, chance = 720},
	{id = "focus cape", chance = 1250},
	{id = "wand of starstorm", chance = 420},
	{id = "hibiscus dress", chance = 250},
	{id = "dragonbone staff", chance = 110},
	{id = "composite hornbow", chance = 90},
	{id = "wyrm scale", chance = 15360},
	{id = "shockwave amulet", subType = 5, chance = 110}
}

monster.attacks = {
	{name = "melee",  attack = 67, skill = 60, minDamage = 0, maxDamage = -235, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 15, interval = 2000, minDamage = -100, maxDamage = -220, radius = 3, effect = 49},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 10, interval = 2000, minDamage = -130, maxDamage = -200, length = 5, spread = 2, effect = 48},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 20, interval = 2000, minDamage = -100, maxDamage = -125, range = 7, shootEffect = 5, effect = 12},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -98, maxDamage = -145, length = 4, shootEffect = 5, effect = 3}
}

monster.defenses = {
	defense = 35,
    armor = 35,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 100, maxDamage = 150, shootEffect = 5, effect = 13},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, radius = 1, shootEffect = 5, effect = 22}
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
	{type = "energy", condition = true, combat = true},
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