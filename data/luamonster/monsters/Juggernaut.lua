local mType = Game.createMonsterType("Juggernaut")
local monster = {}
monster.description = "a juggernaut"
monster.experience = 14000
monster.outfit = {
	lookType = 244
}

monster.health = 20000
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6336
monster.speed = 290
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
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 60
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "RAAARRR!", yell = false},
	{text = "GRRRRRR!", yell = false},
	{text = "WAHHHH!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 100000},
	{id = "gold coin", maxCount = 100, chance = 100000},
	{id = "gold coin", maxCount = 100, chance = 100000},
	{id = "gold coin", maxCount = 100, chance = 100000},
	{id = "platinum coin", maxCount = 15, chance = 100000},
	{id = "ham", maxCount = 8, chance = 60000},
	{id = "demonic essence", chance = 45333},
	{id = "soul orb", chance = 33333},
	{id = "concentrated demonic blood", maxCount = 4, chance = 25000},
	{id = "great health potion", chance = 32000},
	{id = "small ruby", maxCount = 4, chance = 20000},
	{id = "assassin star", maxCount = 10, chance = 25000},
	{id = "great mana potion", chance = 35000},
	{id = "small emerald", maxCount = 5, chance = 20000},
	{id = "rusty armor", chance = 3000},
	{id = "onyx arrow", maxCount = 15, chance = 11111},
	{id = "red gem", chance = 13850},
	{id = "gold ingot", maxCount = 2, chance = 7692},
	{id = "spiked squelcher", chance = 7761},
	{id = "violet gem", chance = 830},
	{id = "demonbone amulet", chance = 550},
	{id = "golden armor", chance = 550},
	{id = "green gem", chance = 869},
	{id = "closed trap", chance = 280},
	{id = "knight armor", chance = 4990},
	{id = "mastermind shield", chance = 800},
	{id = "dragon hammer", chance = 9000},
	{id = "titan axe", chance = 4430},
	{id = "golden legs", chance = 500},
	{id = "broken pottery", chance = 600},
	{id = "skullcracker armor", chance = 400},
	{id = "heavy mace", chance = 400},
	{id = "war axe", chance = 400}
}

monster.attacks = {
	{name = "melee",  attack = 140, skill = 200, minDamage = 0, maxDamage = -1470, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 15, interval = 2000, minDamage = 0, maxDamage = -780, range = 7, shootEffect = 12}
}

monster.defenses = {
	defense = 60,
    armor = 60,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 520, shootEffect = 12, effect = 14},
	{name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 400, maxDamage = 900, shootEffect = 12, effect = 13}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 40},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 30},
	{type = COMBAT_HOLYDAMAGE, percent = -5},
	{type = COMBAT_ICEDAMAGE, percent = 10}
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