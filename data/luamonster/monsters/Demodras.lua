local mType = Game.createMonsterType("Demodras")
local monster = {}
monster.description = "demodras"
monster.experience = 4000
monster.outfit = {
	lookType = 204
}

monster.health = 4500
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5984
monster.speed = 230
monsters.runHealth = 300
monster.maxSummons = 2

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
	staticAttack = 90
}

monster.summons = {
    {name = "Dragon", chance = 17, interval = 1000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "I WILL SET THE WORLD IN FIRE!", yell = true},
	{text = "I WILL PROTECT MY BROOD!", yell = true}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "dragon ham", maxCount = 2, chance = 20000},
	{id = "golden mug", chance = 1818},
	{id = "gemmed book", chance = 3333},
	{id = "broadsword", chance = 5000},
	{id = "royal helmet", chance = 588},
	{id = "dragon scale mail", chance = 333},
	{id = "power bolt", maxCount = 10, chance = 2222},
	{id = "burst arrow", maxCount = 5, chance = 2222},
	{id = "green mushroom", chance = 6666},
	{id = "worm", maxCount = 10, chance = 50000},
	{id = "gold coin", maxCount = 100, chance = 100000},
	{id = "small emerald", maxCount = 2, chance = 833},
	{id = "fire sword", chance = 1428},
	{id = "small sapphire", maxCount = 2, chance = 2222},
	{id = "tower shield", chance = 1333},
	{id = "red dragon leather", chance = 5000},
	{id = "red dragon scale", chance = 5000},
	{id = "dragon claw", chance = 100000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = -160, maxDamage = -600, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 20, interval = 3000, minDamage = -250, maxDamage = -350, target = true, range = 7, radius = 4, shootEffect = 4, effect = 7},
	{name = "firefield",  chance = 10, interval = 1000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 6, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 20, interval = 4000, minDamage = -250, maxDamage = -550, length = 8, spread = 3, shootEffect = 4, effect = 7}
}

monster.defenses = {
	defense = 25,
    armor = 35,
    {name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 1000, minDamage = 400, maxDamage = 700, shootEffect = 4, effect = 13}
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)