local mType = Game.createMonsterType("Tanjis")
local monster = {}
monster.description = "a tanjis"
monster.experience = 14500
monster.outfit = {
	lookType = 446
}

monster.health = 17350
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 15228
monster.speed = 560
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
	{text = "JAUL QJELL, JAKNH JEH KENH!!", yell = false}
}

monster.loot = {
    {id = "depth ocrea", chance = 50000},
	{id = "ornate mace", chance = 25000},
	{id = "ornate shield", chance = 20000}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 0, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 20, interval = 2500, minDamage = -200, maxDamage = -600, target = true, range = 7, radius = 4, shootEffect = 15, effect = 47},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 27, interval = 3500, minDamage = -200, maxDamage = -400, target = true, range = 7, radius = 4, shootEffect = 29, effect = 42},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 15, interval = 3500, minDamage = -100, maxDamage = -400, target = true, range = 1, radius = 1, shootEffect = 29, effect = 42},
	{name = "combat", type = COMBAT_DROWNDAMAGE,  chance = 11, interval = 2300, minDamage = -200, maxDamage = -500, target = true, range = 7, radius = 4, shootEffect = 29, effect = 54},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 14, interval = 2300, minDamage = -200, maxDamage = -600, range = 7, radius = 7, shootEffect = 29, effect = 26},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = -100, maxDamage = -400, target = true, range = 7, radius = 1, shootEffect = 12, effect = 26},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 7, interval = 1200, minDamage = -300, maxDamage = -800, length = 8, radius = 1, shootEffect = 12, effect = 39},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 14, interval = 2000, minDamage = -200, maxDamage = -500, length = 8, spread = 3, radius = 1, shootEffect = 12, effect = 48},
	{name = "speed",  chance = 16, interval = 2150, minDamage = 0, maxDamage = 0, target = true, duration = 15000, speedChange = -600, range = 7, radius = 1, shootEffect = 12, effect = 48}
}

monster.defenses = {
	defense = 40,
    armor = 40,
    {name = "combat", type = COMBAT_HEALING,  chance = 40, interval = 3000, minDamage = 800, maxDamage = 1100, radius = 1, shootEffect = 12, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -20},
	{type = COMBAT_ENERGYDAMAGE, percent = -20},
	{type = COMBAT_EARTHDAMAGE, percent = -20},
	{type = COMBAT_DEATHDAMAGE, percent = -20}
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)