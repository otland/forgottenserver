local mType = Game.createMonsterType("Elf Scout")
local monster = {}
monster.description = "a elf scout"
monster.experience = 75
monster.outfit = {
	lookType = 64
}

monster.health = 160
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6012
monster.speed = 170
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Tha'shi Ab'Dendriel!", yell = false},
	{text = "Feel the sting of my arrows!", yell = false},
	{text = "Thy blood will quench the soil's thirst!", yell = false},
	{text = "Evicor guide my arrow.", yell = false},
	{text = "Your existence will end here!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 25, chance = 75000},
	{id = "arrow", maxCount = 12, chance = 30710},
	{id = "poison arrow", maxCount = 4, chance = 15400},
	{id = "grapes", chance = 17750},
	{id = "waterskin", chance = 1350},
	{id = "elven scouting glass", chance = 9750},
	{id = "sandals", chance = 1180},
	{id = "elvish talisman", chance = 5200},
	{id = "bow", chance = 4000},
	{id = "heaven blossom", chance = 1130},
	{id = "elvish bow", chance = 140}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 20, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -80, range = 7, shootEffect = 3}
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)