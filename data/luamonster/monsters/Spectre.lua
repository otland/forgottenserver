local mType = Game.createMonsterType("Spectre")
local monster = {}
monster.description = "a spectre"
monster.experience = 2100
monster.outfit = {
	lookHead = 20,
	lookType = 235
}

monster.health = 1350
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6348
monster.speed = 230
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
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Revenge ... is so ... sweet!", yell = false},
	{text = "Life...force! Feed me your... lifeforce!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 33000},
	{id = "gold coin", maxCount = 100, chance = 33000},
	{id = "gold coin", maxCount = 97, chance = 33000},
	{id = "blank rune", maxCount = 2, chance = 30310},
	{id = "platinum coin", maxCount = 7, chance = 3850},
	{id = "soul orb", chance = 6005},
	{id = "stealth ring", chance = 190},
	{id = "wand of cosmic energy", chance = 9800},
	{id = "demonic essence", chance = 6270},
	{id = "lyre", chance = 9620},
	{id = "relic sword", chance = 700},
	{id = "silver brooch", chance = 850},
	{id = "white piece of cloth", chance = 3800},
	{id = "death ring", chance = 280},
	{id = "great mana potion", chance = 920},
	{id = "silver brooch", chance = 110},
	{id = "demonbone amulet", chance = 110},
	{id = "shiny stone", chance = 1000}
}

monster.attacks = {
	{name = "melee",  attack = 82, skill = 65, minDamage = 0, maxDamage = -308, interval = 2000, effect = 0},
	{name = "drunk",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, radius = 4, effect = 23},
	{name = "spectre drown",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, effect = 23},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 15, interval = 2000, minDamage = -100, maxDamage = -400, range = 7, effect = 23},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 20, interval = 2000, minDamage = -300, maxDamage = -550, range = 7, effect = 14}
}

monster.defenses = {
	defense = 35,
    armor = 35,
    {name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 2000, minDamage = 100, maxDamage = 250, effect = 13},
	{name = "combat", type = COMBAT_HEALING,  chance = 10, interval = 2000, minDamage = 370, maxDamage = 700, effect = 13},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 290, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = 90},
	{type = COMBAT_ENERGYDAMAGE, percent = -8},
	{type = COMBAT_FIREDAMAGE, percent = -8},
	{type = COMBAT_ICEDAMAGE, percent = 1}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = true, combat = false},
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