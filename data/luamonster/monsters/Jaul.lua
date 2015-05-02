local mType = Game.createMonsterType("Jaul")
local monster = {}
monster.description = "a jaul"
monster.experience = 30000
monster.outfit = {
	lookType = 444
}

monster.health = 100000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 15220
monster.speed = 560
monster.runHealth = 60
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
	{text = "QJELL AFAR GOU JEY!!", yell = false},
	{text = "DIE!! KENH!!", yell = false}
}

monster.loot = {
    {id = "ornate shield", chance = 50000},
	{id = "depth galea", chance = 33333},
	{id = "deepling axe", chance = 25000},
	{id = "ornate chestplate", chance = 25000},
	{id = "depth lorica", chance = 20000},
	{id = "depth calcei", chance = 14285},
	{id = "ornate mace", chance = 14285},
	{id = "ornate legs", chance = 7142}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -2000, interval = 2000, effect = 10},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 19, interval = 2200, minDamage = 0, maxDamage = -1000, target = true, range = 7, radius = 7, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 32, interval = 3000, minDamage = -200, maxDamage = -800, target = true, range = 7, radius = 7, shootEffect = 4, effect = 18},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 27, interval = 1300, minDamage = 0, maxDamage = -600, radius = 3, shootEffect = 4, effect = 5},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 6, interval = 1200, minDamage = -300, maxDamage = -900, length = 8, shootEffect = 4, effect = 18},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 5, interval = 1000, minDamage = -500, maxDamage = -1000, length = 8, shootEffect = 4, effect = 16},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 5, interval = 1000, minDamage = -1000, maxDamage = -2000, length = 8, shootEffect = 4, effect = 42},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 4000, minDamage = -500, maxDamage = -1000, length = 8, spread = 3, shootEffect = 4, effect = 54},
	{name = "speed",  chance = 14, interval = 1900, minDamage = 0, maxDamage = 0, target = true, duration = 15000, speedChange = -600, range = 7, radius = 1, shootEffect = 4, effect = 54}
}

monster.defenses = {
	defense = 40,
    armor = 40,
    {name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 2000, minDamage = 1000, maxDamage = 2300, shootEffect = 4, effect = 13},
	{name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 5500, minDamage = 14000, maxDamage = 21000, shootEffect = 4, effect = 13}
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