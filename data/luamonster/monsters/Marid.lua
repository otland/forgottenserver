local mType = Game.createMonsterType("Marid")
local monster = {}
monster.description = "a marid"
monster.experience = 410
monster.outfit = {
	lookType = 104
}

monster.health = 550
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6033
monster.speed = 180
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.summons = {
    {name = "blue djinn", chance = 10, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Simsalabim", yell = false},
	{text = "Feel the power of my magic, tiny mortal!", yell = false},
	{text = "Wishes can come true.", yell = false},
	{text = "Djinns will soon again be the greatest!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 70, chance = 60000},
	{id = "gold coin", maxCount = 30, chance = 60000},
	{id = "blueberry", maxCount = 29, chance = 65000},
	{id = "royal spear", maxCount = 3, chance = 15500},
	{id = "heavy machete", chance = 4530},
	{id = "small oil lamp", chance = 110},
	{id = "blue tapestry", chance = 2560},
	{id = "small sapphire", chance = 6200},
	{id = "strong mana potion", chance = 9800},
	{id = "blue piece of cloth", chance = 3750},
	{id = "wooden flute", chance = 5000},
	{id = "seeds", chance = 2400},
	{id = "hailstorm rod", chance = 770},
	{id = "mystic turban", chance = 290},
	{id = "noble turban", chance = 530},
	{id = "jewelled belt", chance = 7880},
	{id = "magma monocle", chance = 320},
	{id = "blue gem", chance = 110}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 50, interval = 2000, effect = 13},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 10, interval = 2000, minDamage = -100, maxDamage = -250, range = 7, radius = 1, shootEffect = 36, effect = 13},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -30, maxDamage = -90, range = 7, radius = 1, shootEffect = 36, effect = 14},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 1500, speedChange = -650, range = 7, radius = 1, shootEffect = 36, effect = 14},
	{name = "drunk",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, radius = 1, shootEffect = 5, effect = 14},
	{name = "outfit",  chance = 1, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, radius = 1, shootEffect = 5, effect = 13},
	{name = "djinn electrify",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, range = 5, radius = 1, shootEffect = 5, effect = 13},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 15, interval = 2000, minDamage = -30, maxDamage = -90, radius = 3, shootEffect = 5, effect = 12}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 50, maxDamage = 80, radius = 3, shootEffect = 5, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 60},
	{type = COMBAT_EARTHDAMAGE, percent = 1},
	{type = COMBAT_FIREDAMAGE, percent = 90},
	{type = COMBAT_HOLYDAMAGE, percent = 1},
	{type = COMBAT_DEATHDAMAGE, percent = -1},
	{type = COMBAT_ICEDAMAGE, percent = -1}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
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
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)