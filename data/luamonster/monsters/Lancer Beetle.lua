local mType = Game.createMonsterType("Lancer Beetle")
local monster = {}
monster.description = "a lancer beetle"
monster.experience = 275
monster.outfit = {
	lookType = 348
}

monster.health = 400
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 11375
monster.speed = 200
monsters.runHealth = 30
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
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Crump!", yell = true}
}

monster.loot = {
    {id = "gold coin", maxCount = 61, chance = 60000},
	{id = "gold coin", maxCount = 79, chance = 60000},
	{id = "lancer beetle shell", chance = 16666},
	{id = "poisonous slime", chance = 8333},
	{id = "lump of dirt", chance = 4166},
	{id = "beetle necklace", chance = 1123},
	{id = "small amethyst", chance = 247}
}

monster.attacks = {
	{name = "melee",  attack = 46, skill = 40, minDamage = 0, maxDamage = -116, interval = 2000, effect = 0},
	{name = "poisonfield",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, radius = 4, effect = 21},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -90, length = 7, effect = 17},
	{name = "condition", type = CONDITION_POISON,  chance = 10, interval = 2000, minDamage = -40, maxDamage = -80, range = 7, shootEffect = 15, effect = 17},
	{name = "lancer beetle curse",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, range = 5, shootEffect = 15, effect = 17}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "invisible",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 15, effect = 35}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
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