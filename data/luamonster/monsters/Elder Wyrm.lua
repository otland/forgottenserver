local mType = Game.createMonsterType("Elder Wyrm")
local monster = {}
monster.description = "an elder wyrm"
monster.experience = 2500
monster.outfit = {
	lookType = 561
}

monster.health = 2700
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 21283
monster.speed = 260
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 15
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "GRRR!", yell = false},
	{text = "GROOOOAAAAAAAAR!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 174, chance = 100000},
	{id = "platinum coin", maxCount = 3, chance = 25150},
	{id = "wyrm scale", chance = 15980},
	{id = "soul orb", chance = 5980},
	{id = "dragon ham", maxCount = 2, chance = 32420},
	{id = "lightning robe", chance = 310},
	{id = "strong health potion", chance = 17710},
	{id = "wand of starstorm", chance = 410},
	{id = "wand of draconia", chance = 2000},
	{id = "small diamond", maxCount = 5, chance = 4000},
	{id = "strong mana potion", chance = 20930},
	{id = "crossbow", chance = 9690},
	{id = "shadow sceptre", chance = 310},
	{id = "lightning boots", chance = 310},
	{id = "focus cape", chance = 100},
	{id = "lightning pendant", subType = 200, chance = 520},
	{id = "lightning legs", chance = 930},
	{id = "shockwave amulet", subType = 5, chance = 100},
	{id = "dragonbone staff", chance = 100},
	{id = "composite hornbow", chance = 310}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 80, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 15, interval = 2000, minDamage = -90, maxDamage = -150, target = true, radius = 4, effect = 11},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 20, interval = 2000, minDamage = -140, maxDamage = -250, radius = 5, effect = 48},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -180, length = 8, radius = 5, effect = 4},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 10, interval = 2000, minDamage = -200, maxDamage = -300, length = 5, spread = 2, radius = 5, effect = 158}
}

monster.defenses = {
	defense = 45,
    armor = 45,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 100, maxDamage = 150, radius = 5, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 80},
	{type = COMBAT_FIREDAMAGE, percent = 25}
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
	{type = "energy", condition = true, combat = true},
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