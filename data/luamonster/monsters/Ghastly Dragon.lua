local mType = Game.createMonsterType("Ghastly Dragon")
local monster = {}
monster.description = "a ghastly dragon"
monster.experience = 4600
monster.outfit = {
	lookType = 351
}

monster.health = 7800
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 11362
monster.speed = 320
monster.runHealth = 366
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 5
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
	staticAttack = 70
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "EMBRACE MY GIFTS!", yell = true},
	{text = "I WILL FEAST ON YOUR SOUL!", yell = true}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 33725},
	{id = "gold coin", maxCount = 100, chance = 33725},
	{id = "gold coin", maxCount = 66, chance = 33725},
	{id = "platinum coin", maxCount = 2, chance = 29840},
	{id = "ghastly dragon head", chance = 6650},
	{id = "ultimate health potion", chance = 24700},
	{id = "jade hat", chance = 810},
	{id = "soul orb", chance = 12170},
	{id = "Zaoan halberd", chance = 15020},
	{id = "great spirit potion", maxCount = 2, chance = 29460},
	{id = "great mana potion", maxCount = 2, chance = 30560},
	{id = "Zaoan armor", chance = 870},
	{id = "undead heart", chance = 19830},
	{id = "demonic essence", chance = 8920},
	{id = "rusty armor", chance = 180},
	{id = "Zaoan legs", chance = 1400},
	{id = "twin hooks", chance = 15100},
	{id = "drakinata", chance = 1470},
	{id = "spellweaver's robe", chance = 690},
	{id = "shiny stone", chance = 860},
	{id = "guardian boots", chance = 200},
	{id = "Zaoan sword", chance = 100},
	{id = "Zaoan shoes", chance = 870},
	{id = "Zaoan helmet", chance = 150},
	{id = "terra legs", chance = 3130},
	{id = "terra boots", chance = 9510}
}

monster.attacks = {
	{name = "melee",  attack = 90, skill = 124, minDamage = 0, maxDamage = -603, interval = 2000, effect = 0},
	{name = "ghastly dragon curse",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, range = 5},
	{name = "condition", type = CONDITION_POISON,  chance = 10, interval = 2000, minDamage = -920, maxDamage = -1280, range = 5, effect = 39},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 15, interval = 2000, minDamage = -80, maxDamage = -230, range = 7, effect = 14},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 10, interval = 2000, minDamage = -120, maxDamage = -250, length = 8, spread = 3, effect = 2},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 15, interval = 2000, minDamage = -110, maxDamage = -180, radius = 4, effect = 18},
	{name = "speed",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, duration = 30000, speedChange = -800, range = 7, effect = 39}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE, percent = -15},
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