local mType = Game.createMonsterType("Frazzlemaw")
local monster = {}
monster.description = "frazzlemaw"
monster.experience = 3400
monster.outfit = {
	lookType = 594
}

monster.health = 4100
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 22567
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Mwaaaahnducate youuuuuu *gurgle*", yell = false},
	{text = "mwaaah!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "platinum coin", maxCount = 7, chance = 100000},
	{id = "banana skin", chance = 9500},
	{id = "piece of iron", chance = 10400},
	{id = "fishbone", chance = 10000},
	{id = "skull", chance = 12680},
	{id = "bone", chance = 10000},
	{id = "big bone", chance = 5500},
	{id = "two handed sword", chance = 3200},
	{id = "fish", maxCount = 3, chance = 6750},
	{id = "ham", maxCount = 2, chance = 6000},
	{id = "iron ore", chance = 3000},
	{id = "fish fin", chance = 4700},
	{id = "hardened bone", chance = 5000},
	{id = "fish tail", chance = 10800},
	{id = "assassin dagger", chance = 1000},
	{id = "haunted blade", chance = 2240},
	{id = "nightmare blade", chance = 1100},
	{id = "great mana potion", maxCount = 3, chance = 15000},
	{id = "great health potion", maxCount = 2, chance = 15000},
	{id = "gold ingot", chance = 2300},
	{id = "sai", chance = 1460},
	{id = "violet crystal shard", chance = 3000},
	{id = "brown crystal splinter", chance = 16000},
	{id = "red crystal fragment", chance = 7600},
	{id = "crystal rubbish", chance = 10000},
	{id = "cluster of solace", chance = 450},
	{id = "frazzle tongue", chance = 18760},
	{id = "frazzle skin", chance = 16000},
	{id = "fish remains", chance = 9420}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 90, interval = 2000, effect = 0},
	{name = "condition", type = CONDITION_BLEEDING,  chance = 10, interval = 2000, minDamage = -300, maxDamage = -400, radius = 3, effect = 1},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -700, length = 5, radius = 3, effect = 5},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -400, target = true, radius = 2, shootEffect = 12, effect = 45},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 15000, speedChange = -600, radius = 5, shootEffect = 12, effect = 14},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 10, interval = 2000, minDamage = -80, maxDamage = -150, radius = 4, shootEffect = 12, effect = 14}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 250, maxDamage = 425, radius = 4, shootEffect = 12, effect = 17}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 15},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = 15},
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