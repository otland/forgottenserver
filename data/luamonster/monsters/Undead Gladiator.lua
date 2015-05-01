local mType = Game.createMonsterType("Undead Gladiator")
local monster = {}
monster.description = "a undead gladiator"
monster.experience = 800
monster.outfit = {
	lookType = 306
}

monster.health = 1000
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 9823
monster.speed = 180
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
	{text = "Let's battle it out in a duel!", yell = false},
	{text = "Bring it!", yell = false},
	{text = "I'll fight here in eternity and beyond.", yell = false},
	{text = "I will not give up!", yell = false},
	{text = "Another foolish adventurer who tries to beat me.", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 44000},
	{id = "gold coin", maxCount = 48, chance = 50500},
	{id = "belted cape", chance = 5000},
	{id = "brass armor", chance = 4700},
	{id = "brass legs", chance = 5500},
	{id = "hunting spear", chance = 4200},
	{id = "meat", maxCount = 2, chance = 15000},
	{id = "scimitar", chance = 11280},
	{id = "throwing star", maxCount = 18, chance = 15700},
	{id = "plate legs", chance = 2444},
	{id = "plate armor", chance = 1700},
	{id = "protection amulet", subType = 250, chance = 2200},
	{id = "two handed sword", chance = 1900},
	{id = "stealth ring", chance = 30},
	{id = "dark helmet", chance = 1460},
	{id = "crusader helmet", chance = 100},
	{id = "health potion", chance = 350},
	{id = "knight axe", chance = 280},
	{id = "flask of warrior's sweat", chance = 210},
	{id = "broken gladiator shield", chance = 5200}
}

monster.attacks = {
	{name = "melee",  attack = 50, skill = 90, minDamage = 0, maxDamage = -250, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 20, interval = 2000, minDamage = 0, maxDamage = -135, range = 7, shootEffect = 25}
}

monster.defenses = {
	defense = 45,
    armor = 45,
    {name = "invisible",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 25, effect = 13}
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