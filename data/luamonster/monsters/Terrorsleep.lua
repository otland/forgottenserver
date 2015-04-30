local mType = Game.createMonsterType("Terrorsleep")
local monster = {}
monster.description = "terrorsleep"
monster.experience = 5900
monster.outfit = {
	lookType = 593
}

monster.health = 7200
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 22497
monster.speed = 270
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
	{text = "Aktat Roshok! Marruk!", yell = false},
	{text = "I will eat you in your sleep.", yell = false},
	{text = "I am the darkness around you...", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "platinum coin", maxCount = 8, chance = 100000},
	{id = "small emerald", chance = 14000},
	{id = "small ruby", maxCount = 3, chance = 9600},
	{id = "small amethyst", maxCount = 3, chance = 17000},
	{id = "giant sword", chance = 560},
	{id = "warrior helmet", chance = 2820},
	{id = "knight armor", chance = 4000},
	{id = "white piece of cloth", chance = 4520},
	{id = "red piece of cloth", chance = 1130},
	{id = "great mana potion", maxCount = 2, chance = 36000},
	{id = "ultimate health potion", chance = 26000},
	{id = "small topaz", maxCount = 2, chance = 17000},
	{id = "blue crystal shard", chance = 6000},
	{id = "blue crystal splinter", chance = 13000},
	{id = "cyan crystal fragment", chance = 17000},
	{id = "trapped bad dream monster", chance = 13000},
	{id = "bowl of terror sweat", chance = 18000},
	{id = "", chance = 1130}
}

monster.attacks = {
	{name = "melee",  attack = 90, skill = 90, interval = 2000, effect = 0},
	{name = "condition", type = CONDITION_POISON,  chance = 20, interval = 2000, minDamage = -1000, maxDamage = -1500, radius = 7, effect = 8},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 10, interval = 2000, minDamage = -100, maxDamage = -300, radius = 5, effect = 14},
	{name = "feversleep skill reducer",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, radius = 5, effect = 14},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -350, maxDamage = -500, length = 6, radius = 5, effect = 49},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 20, interval = 2000, minDamage = -200, maxDamage = -450, target = true, radius = 1, shootEffect = 32, effect = 18}
}

monster.defenses = {
	defense = 50,
    armor = 50,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 350, maxDamage = 600, radius = 1, shootEffect = 32, effect = 13},
	{name = "invisible",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, radius = 1, shootEffect = 32, effect = 10},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 300, radius = 1, shootEffect = 32, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 5},
	{type = COMBAT_FIREDAMAGE, percent = 5},
	{type = COMBAT_HOLYDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = 5},
	{type = COMBAT_ICEDAMAGE, percent = 5}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)