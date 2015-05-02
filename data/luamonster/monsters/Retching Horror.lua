local mType = Game.createMonsterType("Retching Horror")
local monster = {}
monster.description = "retching horror"
monster.experience = 3900
monster.outfit = {
	lookType = 588
}

monster.health = 5300
monster.maxHealth = monster.health
monster.race = "fire"
monster.corpse = 22508
monster.speed = 250
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
	{text = "Wait for us, little maggot...", yell = false},
	{text = "We will devour you...", yell = false},
	{text = "My little beetles, go forth, eat, feast!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "platinum coin", maxCount = 9, chance = 100000},
	{id = "fire sword", chance = 1240},
	{id = "crown shield", chance = 1700},
	{id = "tower shield", chance = 550},
	{id = "", chance = 3700},
	{id = "brown mushroom", maxCount = 2, chance = 14000},
	{id = "beastslayer axe", chance = 4260},
	{id = "spiked squelcher", chance = 410},
	{id = "mercenary sword", chance = 2500},
	{id = "great mana potion", chance = 12500},
	{id = "great health potion", chance = 15000},
	{id = "underworld rod", chance = 1800},
	{id = "wand of starstorm", chance = 960},
	{id = "goosebump leather", chance = 16400},
	{id = "pool of chitinous glue", chance = 14000}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 90, minDamage = 0, maxDamage = -400, interval = 2000, effect = 0},
	{name = "drunk",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, length = 4, spread = 3, effect = 15},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, target = true, duration = 15000, speedChange = -700, radius = 4, shootEffect = 27, effect = 32},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -110, radius = 4, shootEffect = 27, effect = 10},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -200, target = true, radius = 1, shootEffect = 22, effect = 10},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 2000, minDamage = -200, maxDamage = -350, target = true, radius = 1, shootEffect = 4, effect = 37}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_DEATHDAMAGE, percent = 20},
	{type = COMBAT_ICEDAMAGE, percent = 15}
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)