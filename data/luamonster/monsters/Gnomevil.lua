local mType = Game.createMonsterType("Gnomevil")
local monster = {}
monster.description = "gnomevil"
monster.experience = 45000
monster.outfit = {
	lookType = 504
}

monster.health = 250000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 18443
monster.speed = 470
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 8
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
	staticAttack = 50
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "No more Mr. Nicegnome!", yell = false},
	{text = "Taste the power of the dark side!", yell = false},
	{text = "I've seen the light! And it was all engulfing fire!", yell = false},
	{text = "Tatatata!!!", yell = false},
	{text = "Muhahaha!", yell = false},
	{text = "I'm feeling grrrreat!", yell = false}
}

monster.loot = {
    {id = "crystalline axe", chance = 9830},
	{id = "crystal crossbow", chance = 8120},
	{id = "Gnomevil's hat", chance = 99150},
	{id = "shiny blade", chance = 7690},
	{id = "mycological bow", chance = 9400},
	{id = "mycological mace", chance = 8120},
	{id = "crystalline sword", chance = 14100},
	{id = "decorative ribbon", chance = 1710}
}

monster.attacks = {
	{name = "melee",  attack = 140, skill = 80, interval = 1300, effect = 0},
	{name = "speed",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, duration = 2500, speedChange = -400, range = 7, shootEffect = 26},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 100, interval = 2000, minDamage = -820, maxDamage = -950, range = 7, shootEffect = 26, effect = 38},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 9, interval = 2000, minDamage = -230, maxDamage = -500, length = 8, shootEffect = 26, effect = 13},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 12, interval = 1000, minDamage = -350, maxDamage = -800, range = 3, shootEffect = 26, effect = 48}
}

monster.defenses = {
	defense = 35,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 1000, minDamage = 1000, maxDamage = 4000, shootEffect = 26, effect = 13}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = true, combat = false},
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