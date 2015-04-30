local mType = Game.createMonsterType("Chizzoron the Distorter")
local monster = {}
monster.description = "chizzoron the distorter"
monster.experience = 7500
monster.outfit = {
	lookType = 340
}

monster.health = 8500
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 11316
monster.speed = 260
monster.maxSummons = 2

monster.changeTarget = {
    interval = 2000,
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

monster.summons = {
    {name = "Lizard Dragon Priest", chance = 10, interval = 2000}
}

monster.voices = {
	interval = 2000,
	chance = 10,
	{text = "Humanzzz! Leave Zzaion at onzzzze!", yell = false},
	{text = "I pray to my mazzterzz, the mighty dragonzzz!", yell = false},
	{text = "You are not worzzy to touch zzizz zzacred ground!", yell = false}
}

monster.loot = {
    {id = "gold ingot", maxCount = 2, chance = 71550},
	{id = "gold coin", maxCount = 100, chance = 69825},
	{id = "gold coin", maxCount = 10, chance = 69825},
	{id = "small emerald", chance = 5750},
	{id = "lizard scale", chance = 100000},
	{id = "green gem", chance = 16300},
	{id = "time ring", chance = 11025},
	{id = "great health potion", chance = 5750},
	{id = "dragon scale mail", chance = 5750}
}

monster.attacks = {
	{name = "melee",  attack = 130, skill = 60, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -430, target = true, range = 7, radius = 1, shootEffect = 15, effect = 21},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -874, radius = 1, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = -300, maxDamage = -646, radius = 3, shootEffect = 4, effect = 3},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -148, maxDamage = -250, range = 7, radius = 3, shootEffect = 4, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE, percent = 10},
	{type = COMBAT_DEATHDAMAGE, percent = 30},
	{type = COMBAT_ICEDAMAGE, percent = 20}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)