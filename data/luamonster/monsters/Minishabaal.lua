local mType = Game.createMonsterType("Minishabaal")
local monster = {}
monster.description = "minishabaal"
monster.experience = 4000
monster.outfit = {
	lookType = 237
}

monster.health = 3500
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6364
monster.speed = 700
monster.maxSummons = 3

monster.changeTarget = {
    interval = 5000,
	chance = 8
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
    {name = "Diabolic Imp", chance = 40, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "I had Princess Lumelia as breakfast!", yell = false},
	{text = "Naaa-Nana-Naaa-Naaa!", yell = false},
	{text = "My brother will come and get you for this!", yell = false},
	{text = "Get them Fluffy!", yell = false},
	{text = "He He He!", yell = false},
	{text = "Pftt, Ferumbras such an upstart!", yell = false},
	{text = "My dragon is not that old, it's just second hand!", yell = false},
	{text = "My other dragon is a red one!", yell = false},
	{text = "When I am big I want to become the ruthless eighth!", yell = false},
	{text = "WHERE'S FLUFFY?", yell = false},
	{text = "Muahaha!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 20, chance = 100000},
	{id = "small amethyst", maxCount = 2, chance = 1428},
	{id = "pitchfork", chance = 2857},
	{id = "fire axe", chance = 666},
	{id = "soul orb", chance = 909},
	{id = "demon shield", chance = 200},
	{id = "demonic essence", maxCount = 2, chance = 1000},
	{id = "golden legs", chance = 180},
	{id = "gold coin", maxCount = 20, chance = 100000},
	{id = "soul orb", chance = 909},
	{id = "crown legs", chance = 800},
	{id = "guardian shield", chance = 1333},
	{id = "demonbone amulet", chance = 909},
	{id = "tempest shield", chance = 500}
}

monster.attacks = {
	{name = "melee",  attack = 95, skill = 70, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 10, interval = 2000, minDamage = -80, maxDamage = -350, target = true, range = 7, radius = 4, shootEffect = 4, effect = 7},
	{name = "combat", type = COMBAT_FIREDAMAGE,  chance = 34, interval = 3000, minDamage = -120, maxDamage = -500, target = true, range = 7, radius = 2, shootEffect = 4, effect = 7}
}

monster.defenses = {
	defense = 25,
    armor = 25,
    {name = "combat", type = COMBAT_HEALING,  chance = 50, interval = 1000, minDamage = 155, maxDamage = 255, radius = 2, shootEffect = 4, effect = 13},
	{name = "speed",  chance = 12, interval = 1000, minDamage = 0, maxDamage = 0, duration = 4000, speedChange = 320, radius = 2, shootEffect = 4, effect = 14},
	{name = "invisible",  chance = 50, interval = 4000, minDamage = 0, maxDamage = 0, radius = 2, shootEffect = 4, effect = 14}
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