local mType = Game.createMonsterType("Crawler")
local monster = {}
monster.description = "a crawler"
monster.experience = 1000
monster.outfit = {
	lookType = 456
}

monster.health = 1450
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 15292
monster.speed = 200
monsters.runHealth = 40
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
	staticAttack = 95
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Sssschrchrsss!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 90, chance = 50000},
	{id = "life ring", chance = 50000},
	{id = "crawler head plating", chance = 18430},
	{id = "compound eye", chance = 14640},
	{id = "small topaz", maxCount = 2, chance = 10040},
	{id = "great mana potion", chance = 9300},
	{id = "great health potion", chance = 6200},
	{id = "war hammer", chance = 2070},
	{id = "springsprout rod", chance = 710},
	{id = "yellow gem", chance = 530},
	{id = "grasshopper legs", chance = 100}
}

monster.attacks = {
	{name = "melee",  attack = 65, skill = 60, minDamage = 0, maxDamage = -228, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = -100, maxDamage = -180, range = 7, shootEffect = 39}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 3000, speedChange = 300, shootEffect = 39, effect = 14}
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