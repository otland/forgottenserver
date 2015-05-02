local mType = Game.createMonsterType("Obujos")
local monster = {}
monster.description = "a obujos"
monster.experience = 25000
monster.outfit = {
	lookType = 445
}

monster.health = 90000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 15224
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
	{text = "JN OBU!!", yell = false},
	{text = "QJELL KEJH!!", yell = false}
}

monster.loot = {
    {id = "depth scutum", chance = 100000},
	{id = "deepling axe", chance = 14285},
	{id = "ornate legs", chance = 14285}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -1200, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 23, interval = 3000, minDamage = -200, maxDamage = -800, target = true, range = 7, radius = 7, shootEffect = 32, effect = 18},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 20, interval = 3500, minDamage = -200, maxDamage = -600, target = true, range = 7, radius = 6, shootEffect = 32, effect = 41},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 25, interval = 2000, minDamage = -200, maxDamage = -800, target = true, range = 7, radius = 1, shootEffect = 6, effect = 9},
	{name = "combat", type = COMBAT_ICEDAMAGE,  chance = 7, interval = 1200, minDamage = -600, maxDamage = -1300, length = 8, shootEffect = 6, effect = 53},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 14, interval = 2000, minDamage = -600, maxDamage = -1600, length = 8, spread = 3, shootEffect = 6, effect = 43}
}

monster.defenses = {
	defense = 40,
    armor = 40,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 1000, minDamage = 1000, maxDamage = 2100, shootEffect = 6, effect = 13}
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