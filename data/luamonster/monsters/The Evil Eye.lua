local mType = Game.createMonsterType("The Evil Eye")
local monster = {}
monster.description = "the evil eye"
monster.experience = 500
monster.outfit = {
	lookType = 210
}

monster.health = 1200
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6037
monster.speed = 240
monster.maxSummons = 5

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
	canPushCreatures = false,
	targetDistance = 3,
	staticAttack = 90
}

monster.summons = {
    {name = "demon skeleton", chance = 13, interval = 1000},
	{name = "ghost", chance = 12, interval = 1000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Inferior creatures, bow before my power!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 45, chance = 100000},
	{id = "crystal mace", chance = 1000},
	{id = "djinn blade", chance = 800},
	{id = "worm", maxCount = 10, chance = 50000},
	{id = "bonelord eye", chance = 5000},
	{id = "gold coin", maxCount = 90, chance = 80000},
	{id = "clerical mace", chance = 5000},
	{id = "war hammer", chance = 1333}
}

monster.attacks = {
	{name = "melee",  attack = 24, skill = 65, minDamage = 0, maxDamage = -91, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 15, interval = 1000, minDamage = -60, maxDamage = -130, range = 7, shootEffect = 5},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 13, interval = 1000, minDamage = -85, maxDamage = -115, range = 7, shootEffect = 4},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 17, interval = 1000, minDamage = -135, maxDamage = -175, range = 7, shootEffect = 32, effect = 18},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 1000, minDamage = -40, maxDamage = -120, range = 7, shootEffect = 15, effect = 18},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 12, interval = 1000, minDamage = -110, maxDamage = -130, range = 7, shootEffect = 15, effect = 14},
	{name = "speed",  chance = 10, interval = 1000, minDamage = 0, maxDamage = 0, duration = 20000, speedChange = -850, range = 7, shootEffect = 15, effect = 14},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 8, interval = 1000, minDamage = -35, maxDamage = -85, length = 8, spread = 3, shootEffect = 15, effect = 9},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 6, interval = 1000, minDamage = -75, maxDamage = -85, length = 8, spread = 3, shootEffect = 15, effect = 14},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 9, interval = 1000, minDamage = -150, maxDamage = -250, length = 8, spread = 3, shootEffect = 15, effect = 2}
}

monster.defenses = {
	defense = 23,
    armor = 19,
    {name = "combat", type = COMBAT_HEALING,  chance = 9, interval = 1000, minDamage = 1, maxDamage = 219, shootEffect = 15, effect = 13}
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