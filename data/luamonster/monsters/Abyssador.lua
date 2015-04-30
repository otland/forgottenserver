local mType = Game.createMonsterType("Abyssador")
local monster = {}
monster.description = "abyssador"
monster.experience = 50000
monster.outfit = {
	lookType = 495
}

monster.health = 300000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 17413
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
	interval = 3000,
	chance = 10,
	{text = "*** BRAINS *** SMALL ***", yell = false},
	{text = "*** LIVE *** DRY ***", yell = false},
	{text = "*** IMPORTANT ***", yell = false},
	{text = "*** FIRE *** HOME *** VICTORY ***", yell = false},
	{text = "*** EXISTENCE *** FUTILE ***", yell = false},
	{text = "*** TIME ***", yell = false},
	{text = "*** STEALTH ***", yell = false},
	{text = "*** DEATH ***", yell = false}
}

monster.loot = {
    {id = "Abyssador's lash", chance = 100000},
	{id = "mycological bow", chance = 10000},
	{id = "shiny blade", chance = 8000},
	{id = "decorative ribbon", chance = 10000},
	{id = "crystal crossbow", chance = 4500},
	{id = "crystalline axe", chance = 5500},
	{id = "crystalline sword", chance = 5500},
	{id = "mycological mace", chance = 4500}
}

monster.attacks = {
	{name = "melee",  attack = 160, skill = 110, interval = 2500, effect = 0},
	{name = "speed",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, duration = 2500, speedChange = -400, range = 7, shootEffect = 30},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 800, minDamage = -820, maxDamage = -1250, range = 7, radius = 10, shootEffect = 30, effect = 17},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 10, interval = 750, minDamage = -650, maxDamage = -1300, target = true, range = 3, radius = 12, shootEffect = 30, effect = 17},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 700, minDamage = -600, maxDamage = -1230, target = true, range = 3, radius = 4, shootEffect = 15, effect = 17},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 12, interval = 700, minDamage = -40, maxDamage = -130, radius = 3, shootEffect = 15, effect = 21}
}

monster.defenses = {
	defense = 20,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 1, interval = 1000, minDamage = 0, maxDamage = 300000, radius = 3, shootEffect = 15, effect = 13},
	{name = "combat", type = COMBAT_HEALING,  chance = 30, interval = 5000, minDamage = 1000, maxDamage = 1000, radius = 3, shootEffect = 15, effect = 13},
	{name = "invisible",  chance = 25, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, shootEffect = 15, effect = 13}
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
	{type = "drunk", condition = true, combat = false},
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)