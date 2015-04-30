local mType = Game.createMonsterType("Guzzlemaw")
local monster = {}
monster.description = "guzzlemaw"
monster.experience = 5500
monster.outfit = {
	lookType = 584
}

monster.health = 6400
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 22485
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
	staticAttack = 80
}

monster.voices = {
	interval = 2000,
	chance = 5,
	{text = "Gmmmooooh! *chomp*", yell = false},
	{text = "MWAAAH! *gurgle*", yell = false},
	{text = "*chomp* Mmmoh! *chomp*", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "platinum coin", maxCount = 7, chance = 100000},
	{id = "banana skin", chance = 10700},
	{id = "piece of iron", chance = 10500},
	{id = "fishbone", chance = 9500},
	{id = "skull", chance = 10400},
	{id = "bone", chance = 9200},
	{id = "big bone", chance = 4500},
	{id = "two handed sword", chance = 2700},
	{id = "fish", maxCount = 3, chance = 7000},
	{id = "ham", chance = 10000},
	{id = "iron ore", chance = 3000},
	{id = "fish fin", chance = 5000},
	{id = "hardened bone", chance = 5700},
	{id = "fish tail", chance = 9400},
	{id = "assassin dagger", chance = 1000},
	{id = "haunted blade", chance = 2000},
	{id = "nightmare blade", chance = 380},
	{id = "great mana potion", maxCount = 3, chance = 17000},
	{id = "great health potion", maxCount = 2, chance = 18500},
	{id = "sai", chance = 1200},
	{id = "violet crystal shard", chance = 3000},
	{id = "brown crystal splinter", maxCount = 2, chance = 12000},
	{id = "red crystal fragment", chance = 7600},
	{id = "crystal rubbish", chance = 12000},
	{id = "cluster of solace", chance = 920},
	{id = "frazzle tongue", chance = 15000},
	{id = "frazzle skin", chance = 14000},
	{id = "fish remains", chance = 10110}
}

monster.attacks = {
	{name = "melee",  attack = 95, skill = 95, interval = 2000, effect = 0},
	{name = "condition", type = CONDITION_BLEEDING,  chance = 10, interval = 2000, minDamage = -500, maxDamage = -1000, radius = 3, effect = 1},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -900, length = 8, spread = 3, radius = 3, effect = 5},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -500, target = true, radius = 2, shootEffect = 12, effect = 45},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 15000, speedChange = -800, radius = 6, shootEffect = 12, effect = 14},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 10, interval = 2000, minDamage = -120, maxDamage = -250, radius = 4, shootEffect = 12, effect = 14}
}

monster.defenses = {
	defense = 50,
    armor = 50,
    {name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 2000, minDamage = 250, maxDamage = 425, radius = 4, shootEffect = 12, effect = 17}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 5},
	{type = COMBAT_EARTHDAMAGE, percent = 15},
	{type = COMBAT_FIREDAMAGE, percent = 5},
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
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)