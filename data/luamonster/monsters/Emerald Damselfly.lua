local mType = Game.createMonsterType("Emerald Damselfly")
local monster = {}
monster.description = "emerald damselfly"
monster.experience = 35
monster.outfit = {
	lookType = 528
}

monster.health = 90
monster.maxHealth = monster.health
monster.race = "venom"
monster.corpse = 19706
monster.speed = 220
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 0
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
	{text = "Bzzzzz!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 18, chance = 90000},
	{id = "arrow", maxCount = 5, chance = 7570},
	{id = "health potion", chance = 1780},
	{id = "mana potion", chance = 2520},
	{id = "insectoid eggs", chance = 10830},
	{id = "marsh stalker feather", chance = 8610},
	{id = "rope", chance = 4300}
}

monster.attacks = {
	{name = "melee",  attack = 5, skill = 5, minDamage = 0, maxDamage = -4, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -12, range = 7, shootEffect = 15}
}

monster.defenses = {
	defense = 10,
    armor = 10,
    {name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 4, maxDamage = 10, shootEffect = 15, effect = 13}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = false, combat = false},
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