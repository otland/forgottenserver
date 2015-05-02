local mType = Game.createMonsterType("Demon Outcast")
local monster = {}
monster.description = "demon outcast"
monster.experience = 6200
monster.outfit = {
	lookType = 590
}

monster.health = 6900
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 22549
monster.speed = 280
monster.maxSummons = 2

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

monster.summons = {
    {name = "energy elemental", chance = 10, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Back in the evil business!", yell = false},
	{text = "This prison break will have casualties!", yell = false},
	{text = "At last someone to hurt", yell = false},
	{text = "No one will imprison me again!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "platinum coin", maxCount = 6, chance = 100000},
	{id = "small diamond", maxCount = 5, chance = 10000},
	{id = "small sapphire", maxCount = 5, chance = 10000},
	{id = "small ruby", maxCount = 5, chance = 12000},
	{id = "small emerald", maxCount = 5, chance = 10000},
	{id = "might ring", subType = 20, chance = 910},
	{id = "stealth ring", chance = 1300},
	{id = "platinum amulet", chance = 1000},
	{id = "ring of healing", chance = 3000},
	{id = "giant sword", chance = 2000},
	{id = "ice rapier", subType = 1, chance = 660},
	{id = "devil helmet", chance = 910},
	{id = "crusader helmet", chance = 740},
	{id = "crown shield", chance = 740},
	{id = "demon shield", chance = 170},
	{id = "demonrage sword", chance = 350},
	{id = "red powder", chance = 1000},
	{id = "fire mushroom", maxCount = 6, chance = 20600},
	{id = "assassin star", maxCount = 10, chance = 8340},
	{id = "great mana potion", maxCount = 2, chance = 18000},
	{id = "ultimate health potion", maxCount = 3, chance = 20500},
	{id = "small topaz", maxCount = 5, chance = 9300},
	{id = "cluster of solace", chance = 410}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 90, minDamage = 0, maxDamage = -400, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 10, interval = 2000, minDamage = -250, maxDamage = -450, length = 6, effect = 48},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 10, interval = 2000, minDamage = -350, maxDamage = -550, length = 8, effect = 49},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 20, interval = 2000, minDamage = -100, maxDamage = -250, target = true, radius = 3, effect = 12},
	{name = "demon outcast skill reducer",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, range = 5, effect = 12},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 10, interval = 2000, minDamage = -80, maxDamage = -150, radius = 4, effect = 15}
}

monster.defenses = {
	defense = 40,
    armor = 40,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 250, maxDamage = 425, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 20},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 40},
	{type = COMBAT_HOLYDAMAGE, percent = -5},
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)