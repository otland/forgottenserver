local mType = Game.createMonsterType("Choking Fear")
local monster = {}
monster.description = "choking fear"
monster.experience = 4700
monster.outfit = {
	lookType = 586
}

monster.health = 5800
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 22493
monster.speed = 260
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 5
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
	{text = "Ah, sweet air... don't you miss it?", yell = false},
	{text = "Murr tat muuza!", yell = false},
	{text = "kchh", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "platinum coin", maxCount = 8, chance = 100000},
	{id = "energy ring", chance = 3000},
	{id = "life ring", chance = 4200},
	{id = "ring of healing", chance = 3000},
	{id = "brown piece of cloth", chance = 6000},
	{id = "yellow piece of cloth", chance = 3500},
	{id = "great mana potion", maxCount = 3, chance = 20000},
	{id = "terra boots", chance = 720},
	{id = "underworld rod", chance = 720},
	{id = "great spirit potion", maxCount = 3, chance = 20000},
	{id = "ultimate health potion", maxCount = 3, chance = 19000},
	{id = "spellbook of mind control", chance = 720},
	{id = "green crystal shard", chance = 1540},
	{id = "brown crystal splinter", maxCount = 2, chance = 52500},
	{id = "blue crystal splinter", maxCount = 3, chance = 10000},
	{id = "cluster of solace", chance = 620},
	{id = "dead weight", chance = 14180},
	{id = "hemp rope", chance = 14180},
	{id = "beastslayer axe", chance = 3290},
	{id = "guardian shield", chance = 1540},
	{id = "springsprout rod", chance = 510}
}

monster.attacks = {
	{name = "melee",  attack = 95, skill = 95, minDamage = 0, maxDamage = -499, interval = 2000, effect = 0},
	{name = "condition", type = CONDITION_POISON,  chance = 10, interval = 2000, minDamage = -700, maxDamage = -900, length = 5, spread = 3, effect = 17},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -300, target = true, radius = 1, shootEffect = 41, effect = 33},
	{name = "speed",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, target = true, duration = 15000, speedChange = -800, radius = 1, shootEffect = 41, effect = 33},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 10, interval = 2000, minDamage = -130, maxDamage = -300, radius = 4, shootEffect = 41, effect = 20},
	{name = "choking fear drown",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 41, effect = 20},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 20, interval = 2000, minDamage = -250, maxDamage = -500, target = true, radius = 4, shootEffect = 32, effect = 18}
}

monster.defenses = {
	defense = 35,
    armor = 35,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 80, maxDamage = 150, shootEffect = 32, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 15},
	{type = COMBAT_DEATHDAMAGE, percent = 50},
	{type = COMBAT_ICEDAMAGE, percent = 10}
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)