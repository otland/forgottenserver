local mType = Game.createMonsterType("Orc Warlord")
local monster = {}
monster.description = "a orc warlord"
monster.experience = 670
monster.outfit = {
	lookType = 2
}

monster.health = 950
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6008
monster.speed = 240
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
	{text = "Ranat Ulderek!", yell = false},
	{text = "Orc buta bana!", yell = false},
	{text = "Ikem rambo zambo!", yell = false},
	{text = "Futchi maruk buta!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 45, chance = 18000},
	{id = "throwing star", maxCount = 18, chance = 13920},
	{id = "fish", maxCount = 2, chance = 10800},
	{id = "scimitar", chance = 3450},
	{id = "meat", maxCount = 2, chance = 9000},
	{id = "orcish axe", chance = 5400},
	{id = "plate armor", chance = 5210},
	{id = "protection amulet", subType = 250, chance = 2190},
	{id = "plate legs", chance = 4280},
	{id = "hunting spear", chance = 5260},
	{id = "brass legs", chance = 9000},
	{id = "brass armor", chance = 740},
	{id = "dark helmet", chance = 1260},
	{id = "two handed sword", chance = 1680},
	{id = "dragon hammer", chance = 320},
	{id = "crusader helmet", chance = 280},
	{id = "stealth ring", chance = 90},
	{id = "health potion", chance = 470},
	{id = "orc trophy", chance = 50},
	{id = "gold ring", chance = 30},
	{id = "magma boots", chance = 280},
	{id = "orc leather", chance = 20620},
	{id = "orc tooth", chance = 9640},
	{id = "skull belt", chance = 4610},
	{id = "broken helmet", chance = 24350}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 90, minDamage = 0, maxDamage = -250, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -200, range = 7, shootEffect = 8}
}

monster.defenses = {
	defense = 35,
    armor = 35,
    {name = "invisible",  chance = 5, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 8, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 80},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_HOLYDAMAGE, percent = 10}
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