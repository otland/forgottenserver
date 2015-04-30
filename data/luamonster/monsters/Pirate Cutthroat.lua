local mType = Game.createMonsterType("Pirate Cutthroat")
local monster = {}
monster.description = "a pirate cutthroat"
monster.experience = 175
monster.outfit = {
	lookType = 96
}

monster.health = 325
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20479
monster.speed = 210
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 15
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Give up!", yell = false},
	{text = "Hiyaa!", yell = false},
	{text = "Plundeeeeer!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 50, chance = 78000},
	{id = "scale armor", chance = 3000},
	{id = "steel shield", chance = 2800},
	{id = "peg leg", chance = 500},
	{id = "hook", chance = 550},
	{id = "eye patch", chance = 450},
	{id = "dice", chance = 110},
	{id = "pirate knee breeches", chance = 980},
	{id = "rum flask", chance = 90},
	{id = "light shovel", chance = 2000},
	{id = "treasure map", chance = 1000},
	{id = "pirate bag", chance = 1000},
	{id = "compass", chance = 10120}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 75, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -95, target = true, range = 7, radius = 1, shootEffect = 41, effect = 5}
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