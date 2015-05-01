local mType = Game.createMonsterType("Deathstrike")
local monster = {}
monster.description = "deathstrike"
monster.experience = 45000
monster.outfit = {
	lookType = 500
}

monster.health = 80000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 18384
monster.speed = 470
monsters.runHealth = 10000
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
	{text = "Deeestructiooon!", yell = false},
	{text = "Maaahhhrrr!", yell = false},
	{text = "You are nothing!", yell = false},
	{text = "Taaake ... this!", yell = false}
}

monster.loot = {
    {id = "shiny blade", chance = 11430},
	{id = "mycological mace", chance = 8570},
	{id = "crystalline sword", chance = 10480},
	{id = "Deathstrike's snippet", chance = 100000},
	{id = "crystal crossbow", chance = 12380},
	{id = "crystalline axe", chance = 11430},
	{id = "mycological bow", chance = 11430}
}

monster.attacks = {
	{name = "melee",  attack = 120, skill = 80, minDamage = 0, maxDamage = -540, interval = 1300, effect = 0},
	{name = "speed",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, duration = 2500, speedChange = -400, range = 7, shootEffect = 26},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 100, interval = 2000, minDamage = -820, maxDamage = -950, range = 7, shootEffect = 26, effect = 7},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 12, interval = 1000, minDamage = -350, maxDamage = -800, range = 3, shootEffect = 26, effect = 48}
}

monster.defenses = {
	defense = 35,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 1000, minDamage = 1000, maxDamage = 5500, shootEffect = 26, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = -20},
	{type = COMBAT_DEATHDAMAGE, percent = -20}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)