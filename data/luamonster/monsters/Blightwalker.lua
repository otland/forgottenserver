local mType = Game.createMonsterType("Blightwalker")
local monster = {}
monster.description = "a blightwalker"
monster.experience = 5850
monster.outfit = {
	lookType = 246
}

monster.health = 8900
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6354
monster.speed = 250
monster.runHealth = 800
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "I can see you decaying!", yell = false},
	{text = "Let me taste your mortality!", yell = false},
	{text = "Your lifeforce is waning!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 197, chance = 100000},
	{id = "platinum coin", maxCount = 5, chance = 100000},
	{id = "bunch of wheat", chance = 50000},
	{id = "great mana potion", maxCount = 3, chance = 31360},
	{id = "demonic essence", chance = 28000},
	{id = "blank rune", maxCount = 2, chance = 26250},
	{id = "soul orb", chance = 23720},
	{id = "bundle of cursed straw", chance = 15000},
	{id = "ultimate health potion", maxCount = 2, chance = 14720},
	{id = "hailstorm rod", chance = 10000},
	{id = "assassin star", maxCount = 10, chance = 5900},
	{id = "gold ingot", chance = 5270},
	{id = "giant shimmering pearl", chance = 4450},
	{id = "giant shimmering pearl", chance = 4450},
	{id = "seeds", chance = 4300},
	{id = "scythe", chance = 3000},
	{id = "terra legs", chance = 2500},
	{id = "garlic necklace", subType = 150, chance = 2050},
	{id = "gold ring", chance = 1870},
	{id = "skull staff", chance = 1520},
	{id = "death ring", chance = 1410},
	{id = "terra mantle", chance = 1050},
	{id = "golden sickle", chance = 350},
	{id = "amulet of loss", subType = 1, chance = 120}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -490, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 20, interval = 2000, minDamage = -220, maxDamage = -405, target = true, range = 7, radius = 1, shootEffect = 15},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -65, maxDamage = -135, radius = 4, shootEffect = 15, effect = 15},
	{name = "drunk",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, shootEffect = 15, effect = 17},
	{name = "blightwalker curse",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 15, effect = 17},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 30000, speedChange = -300, range = 7, shootEffect = 15, effect = 17}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 50},
	{type = COMBAT_HOLYDAMAGE, percent = -30},
	{type = COMBAT_ICEDAMAGE, percent = 50}
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
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)