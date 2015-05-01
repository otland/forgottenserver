local mType = Game.createMonsterType("Salamander")
local monster = {}
monster.description = "a salamander"
monster.experience = 25
monster.outfit = {
	lookType = 529
}

monster.health = 70
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 19707
monster.speed = 220
monsters.runHealth = 10
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
	{text = "Chrrch!", yell = false},
	{text = "Chhh!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 15, chance = 60000},
	{id = "arrow", maxCount = 3, chance = 7890},
	{id = "health potion", chance = 840},
	{id = "rope", chance = 4700},
	{id = "bow", chance = 1090},
	{id = "mace", chance = 3780},
	{id = "brass helmet", chance = 2180},
	{id = "scimitar", chance = 420},
	{id = "dead corym vanguard", maxCount = 5, chance = 32830}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 30, minDamage = 0, maxDamage = -60, interval = 2000, effect = 0}
}

monster.defenses = {
	defense = 5,
    armor = 5,
    {name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 6000, minDamage = 0, maxDamage = 4, effect = 13}
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