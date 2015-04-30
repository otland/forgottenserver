local mType = Game.createMonsterType("Dragon Lord")
local monster = {}
monster.description = "a dragon lord"
monster.experience = 2100
monster.outfit = {
	lookType = 39
}

monster.health = 1900
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5984
monster.speed = 200
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
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "ZCHHHHH", yell = true},
	{text = "YOU WILL BURN!", yell = true}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 33750},
	{id = "gold coin", maxCount = 100, chance = 33750},
	{id = "gold coin", maxCount = 45, chance = 33750},
	{id = "gemmed book", chance = 9000},
	{id = "dragon ham", maxCount = 5, chance = 80000},
	{id = "dragon lord trophy", chance = 80},
	{id = "energy ring", chance = 5250},
	{id = "fire sword", chance = 290},
	{id = "golden mug", chance = 3190},
	{id = "green mushroom", chance = 12000},
	{id = "life crystal", chance = 680},
	{id = "power bolt", maxCount = 7, chance = 6700},
	{id = "red dragon leather", chance = 1040},
	{id = "red dragon scale", chance = 1920},
	{id = "royal helmet", chance = 280},
	{id = "royal spear", maxCount = 3, chance = 8800},
	{id = "small sapphire", chance = 5300},
	{id = "strange helmet", chance = 360},
	{id = "strong health potion", chance = 970},
	{id = "tower shield", chance = 250},
	{id = "dragon slayer", chance = 100},
	{id = "dragon scale mail", chance = 170}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 105, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 20, interval = 2000, minDamage = -100, maxDamage = -200, target = true, range = 7, radius = 4, shootEffect = 4, effect = 7},
	{name = "firefield",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 4, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 15, interval = 2000, minDamage = -150, maxDamage = -230, length = 8, spread = 3, radius = 4, shootEffect = 4, effect = 7}
}

monster.defenses = {
	defense = 35,
    armor = 35,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 57, maxDamage = 93, radius = 4, shootEffect = 4, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 80},
	{type = COMBAT_ICEDAMAGE, percent = -10}
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