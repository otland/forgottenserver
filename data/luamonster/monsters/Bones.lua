local mType = Game.createMonsterType("Bones")
local monster = {}
monster.description = "bones"
monster.experience = 3750
monster.outfit = {
	lookType = 231
}

monster.health = 9500
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6306
monster.speed = 300
monsters.runHealth = 1
monster.maxSummons = 0

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
    {name = "", chance = 0, interval = 0}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Your new name is breakfast.", yell = false},
	{text = "Keep that dog away!", yell = false},
	{text = "Out Fluffy! Out! Bad dog!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "gold coin", maxCount = 90, chance = 50000},
	{id = "skull helmet", chance = 50000},
	{id = "demonic essence", chance = 1538},
	{id = "surprise bag", maxCount = 3, chance = 5538},
	{id = "surprise bag", chance = 1538},
	{id = "magic plate armor", chance = 2000},
	{id = "death ring", chance = 4000},
	{id = "soul orb", chance = 10000},
	{id = "spectral stone", chance = 800},
	{id = "sword ring", chance = 10000},
	{id = "dragonbone staff", chance = 50000},
	{id = "broadsword", chance = 4000}
}

monster.attacks = {
	{name = "melee",  attack = 130, skill = 120, minDamage = 0, maxDamage = -845, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 13, interval = 1000, minDamage = -400, maxDamage = -600, target = true, radius = 1},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 34, interval = 3000, minDamage = -180, maxDamage = -500, target = true, range = 1, radius = 1, shootEffect = 11}
}

monster.defenses = {
	defense = 55,
    armor = 50,
    {name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 5000, minDamage = 60, maxDamage = 100, shootEffect = 11, effect = 13}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = true, combat = false},
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