local mType = Game.createMonsterType("The Blightfather")
local monster = {}
monster.description = "the blightfather"
monster.experience = 600
monster.outfit = {
	lookType = 348
}

monster.health = 400
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 11375
monster.speed = 290
monster.runHealth = 80
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 12
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
	{text = "Crump!", yell = true}
}

monster.loot = {
    {id = "gold coin", maxCount = 61, chance = 2000},
	{id = "gold coin", maxCount = 60, chance = 2000},
	{id = "lump of dirt", chance = 17500},
	{id = "poisonous slime", chance = 12500},
	{id = "lancer beetle shell", chance = 7000},
	{id = "beetle necklace", chance = 400},
	{id = "small amethyst", chance = 800}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 40, minDamage = 0, maxDamage = -200, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, length = 8, spread = 3, effect = 21}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "invisible",  chance = 10, interval = 1000, minDamage = 0, maxDamage = 0, effect = 14}
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