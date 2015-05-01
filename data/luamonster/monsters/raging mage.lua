local mType = Game.createMonsterType("raging mage")
local monster = {}
monster.description = "a raging mage"
monster.experience = 3250
monster.outfit = {
	lookType = 416
}

monster.health = 3500
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 13834
monster.speed = 200
monsters.runHealth = 300
monster.maxSummons = 1

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
	staticAttack = 80
}

monster.summons = {
    {name = "golden servant", chance = 50, interval = 2000}
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Behold the all permeating powers I draw from this gate!!", yell = false},
	{text = "ENERGY!!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 69, chance = 47000},
	{id = "platinum coin", maxCount = 8, chance = 77400},
	{id = "assassin star", maxCount = 7, chance = 1830},
	{id = "red piece of cloth", chance = 31100},
	{id = "great mana potion", maxCount = 5, chance = 23170},
	{id = "great health potion", maxCount = 5, chance = 26830},
	{id = "dark mushroom", maxCount = 3, chance = 3600},
	{id = "mind stone", chance = 9760},
	{id = "bullseye potion", chance = 6710},
	{id = "focus cape", chance = 4880},
	{id = "stealth ring", chance = 4880},
	{id = "luminous orb", chance = 4270},
	{id = "ring of the sky", chance = 1830},
	{id = "small sapphire", maxCount = 5, chance = 1830},
	{id = "royal tapestry", chance = 1220},
	{id = "dead minotaur", chance = 1220},
	{id = "spellbook of mind control", chance = 610},
	{id = "skull helmet", chance = 610},
	{id = "boots of haste", chance = 610}
}

monster.attacks = {
	{name = "melee",  attack = 37, skill = 32, minDamage = 0, maxDamage = -78, interval = 2000, effect = 0},
	{name = "thunderstorm",  chance = 35, interval = 2000, minDamage = -100, maxDamage = -200, range = 7},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 15, interval = 2000, minDamage = -100, maxDamage = -200, range = 7},
	{name = "energyfield",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, target = true, range = 7, radius = 2, shootEffect = 5},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 20, interval = 2000, minDamage = -101, maxDamage = -300, length = 8, shootEffect = 5, effect = 41}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -25},
	{type = COMBAT_DEATHDAMAGE, percent = 15}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = true, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = true, combat = true},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)