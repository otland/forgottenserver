local mType = Game.createMonsterType("Serpent Spawn")
local monster = {}
monster.description = "a serpent spawn"
monster.experience = 3050
monster.outfit = {
	lookType = 220
}

monster.health = 3000
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 4323
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
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Sssssouls for the one", yell = false},
	{text = "HISSSS", yell = true},
	{text = "Tsssse one will risssse again", yell = false},
	{text = "I bring your deathhh, mortalssss", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 32300},
	{id = "gold coin", maxCount = 100, chance = 32300},
	{id = "gold coin", maxCount = 39, chance = 32300},
	{id = "charmer's tiara", chance = 180},
	{id = "crown armor", chance = 510},
	{id = "golden mug", chance = 2870},
	{id = "great mana potion", chance = 2000},
	{id = "green mushroom", chance = 18200},
	{id = "life crystal", chance = 800},
	{id = "life ring", chance = 6250},
	{id = "mercenary sword", chance = 2070},
	{id = "noble axe", chance = 750},
	{id = "power bolt", chance = 6200},
	{id = "royal helmet", chance = 140},
	{id = "small sapphire", chance = 12000},
	{id = "snakebite rod", chance = 930},
	{id = "spellbook of mind control", chance = 90},
	{id = "strange helmet", chance = 670},
	{id = "swamplair armor", chance = 90},
	{id = "tower shield", chance = 920},
	{id = "warrior helmet", chance = 560},
	{id = "snake skin", chance = 14800},
	{id = "winged tail", chance = 960},
	{id = "old parchment", chance = 600},
	{id = "energy ring", chance = 590}
}

monster.attacks = {
	{name = "melee",  attack = 63, skill = 70, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = -80, maxDamage = -300, range = 7, shootEffect = 15},
	{name = "outfit",  chance = 1, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, shootEffect = 15, effect = 13},
	{name = "speed",  chance = 25, interval = 2000, minDamage = 0, maxDamage = 0, target = true, duration = 12000, speedChange = -850, range = 7, radius = 4, shootEffect = 15, effect = 9},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 10, interval = 2000, minDamage = -200, maxDamage = -500, length = 8, radius = 4, shootEffect = 15, effect = 20},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 10, interval = 2000, minDamage = -200, maxDamage = -500, length = 8, spread = 3, radius = 4, shootEffect = 15, effect = 21}
}

monster.defenses = {
	defense = 35,
    armor = 35,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 250, maxDamage = 500, radius = 4, shootEffect = 15, effect = 13},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 340, radius = 4, shootEffect = 15, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -10}
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