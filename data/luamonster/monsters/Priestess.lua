local mType = Game.createMonsterType("Priestess")
local monster = {}
monster.description = "a priestess"
monster.experience = 420
monster.outfit = {
	lookType = 58
}

monster.health = 390
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20491
monster.speed = 200
monster.maxSummons = 2

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
	canPushCreatures = true,
	targetDistance = 4,
	staticAttack = 90
}

monster.summons = {
    {name = "ghoul", chance = 10, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Your energy is mine.", yell = false},
	{text = "Now, your life has come to an end, hahahha!", yell = false},
	{text = "Throw the soul on the altar!", yell = false}
}

monster.loot = {
    {id = "black shield", chance = 210},
	{id = "book", chance = 890},
	{id = "clerical mace", chance = 1500},
	{id = "crystal ball", chance = 1250},
	{id = "crystal necklace", chance = 640},
	{id = "goat grass", chance = 11720},
	{id = "hailstorm rod", chance = 1100},
	{id = "mana potion", chance = 850},
	{id = "powder herb", chance = 5900},
	{id = "red apple", maxCount = 2, chance = 7500},
	{id = "sling herb", chance = 13200},
	{id = "talon", chance = 750},
	{id = "wood mushroom", chance = 3240},
	{id = "wooden flute", chance = 1410},
	{id = "piggy bank", chance = 90},
	{id = "dark rosary", chance = 9840},
	{id = "cultish robe", chance = 1800},
	{id = "black hood", chance = 5230},
	{id = "wooden flute", chance = 1400}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 65, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 20, interval = 2000, minDamage = -55, maxDamage = -120, range = 7, shootEffect = 32, effect = 18},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 15, interval = 2000, minDamage = -2, maxDamage = -170, range = 7, shootEffect = 32, effect = 18},
	{name = "condition", type = CONDITION_POISON,  chance = 15, interval = 2000, minDamage = -200, maxDamage = 0, range = 7, shootEffect = 15, effect = 18}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 50, maxDamage = 80, shootEffect = 15, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 70},
	{type = COMBAT_FIREDAMAGE, percent = 40},
	{type = COMBAT_DEATHDAMAGE, percent = 5},
	{type = COMBAT_HOLYDAMAGE, percent = -10}
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