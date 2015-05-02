local mType = Game.createMonsterType("Fury")
local monster = {}
monster.description = "a fury"
monster.experience = 4500
monster.outfit = {
	lookLegs = 96,
	lookHead = 94,
	lookType = 149,
	lookAddons = 3,
	lookBody = 77
}

monster.health = 4100
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20399
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
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 70
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Ahhhhrrrr!", yell = false},
	{text = "Waaaaah!", yell = false},
	{text = "Carnage!", yell = false},
	{text = "Dieee!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 30000},
	{id = "gold coin", maxCount = 100, chance = 30000},
	{id = "gold coin", maxCount = 69, chance = 38000},
	{id = "platinum coin", maxCount = 4, chance = 2800},
	{id = "concentrated demonic blood", maxCount = 3, chance = 35000},
	{id = "demonic essence", chance = 22500},
	{id = "golden legs", chance = 130},
	{id = "great health potion", chance = 10500},
	{id = "meat", chance = 25000},
	{id = "noble axe", chance = 2000},
	{id = "orichalcum pearl", maxCount = 4, chance = 1500},
	{id = "red piece of cloth", chance = 4000},
	{id = "rusty legs", chance = 1920},
	{id = "rope", maxCount = 3, chance = 8000},
	{id = "soul orb", chance = 21500},
	{id = "steel boots", chance = 790},
	{id = "terra rod", chance = 20000},
	{id = "assassin dagger", chance = 660},
	{id = "soul orb", chance = 50},
	{id = "crystal ring", chance = 410},
	{id = "jalapeño pepper", maxCount = 4, chance = 29280},
	{id = "death ring", chance = 60}
}

monster.attacks = {
	{name = "melee",  attack = 170, skill = 50, minDamage = 0, maxDamage = -510, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 2000, minDamage = -200, maxDamage = -300, length = 8, spread = 3, effect = 5},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 5, interval = 2000, minDamage = -120, maxDamage = -700, length = 8, effect = 1},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 10, interval = 2000, minDamage = -120, maxDamage = -300, radius = 4, effect = 1},
	{name = "fury skill reducer",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, effect = 1},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -120, maxDamage = -300, radius = 3, effect = 10},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 10, interval = 2000, minDamage = -125, maxDamage = -250, range = 7, shootEffect = 32, effect = 39},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 30000, speedChange = -800, range = 7, shootEffect = 32, effect = 39}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 800, shootEffect = 32, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = 30},
	{type = COMBAT_DEATHDAMAGE, percent = -10},
	{type = COMBAT_ICEDAMAGE, percent = 30}
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