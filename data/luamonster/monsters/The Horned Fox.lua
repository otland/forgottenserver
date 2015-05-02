local mType = Game.createMonsterType("The Horned Fox")
local monster = {}
monster.description = "the horned fox"
monster.experience = 200
monster.outfit = {
	lookType = 202
}

monster.health = 265
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5983
monster.speed = 210
monster.maxSummons = 6

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
    {name = "minotaur archer", chance = 13, interval = 1000},
	{name = "minotaur guard", chance = 13, interval = 1000},
	{name = "minotaur mage", chance = 13, interval = 1000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "You will never get me!", yell = false},
	{text = "I'll be back!", yell = false},
	{text = "Catch me, if you can!", yell = false},
	{text = "Help me, boys!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 30, chance = 100000},
	{id = "throwing knife", maxCount = 2, chance = 50000},
	{id = "scimitar", chance = 10000},
	{id = "plate shield", chance = 5000},
	{id = "fish", chance = 20000},
	{id = "plate legs", chance = 1818},
	{id = "broadsword", chance = 2857},
	{id = "worm", maxCount = 10, chance = 50000},
	{id = "nose ring", chance = 100000},
	{id = "wand of cosmic energy", chance = 1333},
	{id = "sword ring", chance = 2000},
	{id = "meat", maxCount = 2, chance = 33333},
	{id = "dagger", chance = 6666},
	{id = "warrior helmet", chance = 1000},
	{id = "longsword", chance = 10000},
	{id = "green tunic", chance = 2500},
	{id = "brass legs", chance = 2857},
	{id = "plate armor", chance = 1818}
}

monster.attacks = {
	{name = "melee",  attack = 38, skill = 54, minDamage = 0, maxDamage = -122, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 25, interval = 1000, minDamage = -50, maxDamage = -120, range = 7, shootEffect = 2},
	{name = "condition", type = CONDITION_POISON,  chance = 17, interval = 1000, minDamage = -50, maxDamage = -120, range = 7, shootEffect = 2}
}

monster.defenses = {
	defense = 16,
    armor = 17,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 1000, minDamage = 25, maxDamage = 75, shootEffect = 2, effect = 14},
	{name = "invisible",  chance = 10, interval = 1000, minDamage = 0, maxDamage = 0, shootEffect = 2, effect = 13}
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