local mType = Game.createMonsterType("Betrayed Wraith")
local monster = {}
monster.description = "a betrayed wraith"
monster.experience = 3500
monster.outfit = {
	lookType = 233
}

monster.health = 4200
monster.maxHealth = monster.health
monster.race = "undead"
monster.corpse = 6316
monster.speed = 230
monsters.runHealth = 300
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 15
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
	{text = "Rrrah!", yell = false},
	{text = "Gnarr!", yell = false},
	{text = "Tcharrr!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 40000},
	{id = "gold coin", maxCount = 100, chance = 40000},
	{id = "gold coin", maxCount = 20, chance = 20000},
	{id = "platinum coin", maxCount = 8, chance = 100000},
	{id = "concentrated demonic blood", chance = 65250},
	{id = "power bolt", maxCount = 5, chance = 50000},
	{id = "assassin star", maxCount = 5, chance = 10780},
	{id = "soul orb", chance = 10000},
	{id = "mercenary sword", chance = 1890},
	{id = "great mana potion", maxCount = 3, chance = 15000},
	{id = "death ring", chance = 390},
	{id = "bloody edge", chance = 80},
	{id = "golden figurine", chance = 160},
	{id = "skull helmet", chance = 390},
	{id = "demonic essence", chance = 19430},
	{id = "small diamond", maxCount = 4, chance = 11800},
	{id = "orichalcum pearl", maxCount = 2, chance = 8000},
	{id = "unholy bone", chance = 18410},
	{id = "ultimate health potion", chance = 15410},
	{id = "amulet of loss", subType = 1, chance = 130}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 80, minDamage = 0, maxDamage = -450, interval = 2000, effect = 0},
	{name = "betrayed wraith skill reducer",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0},
	{name = "speed",  chance = 20, interval = 2000, minDamage = 0, maxDamage = 0, duration = 3000, speedChange = -600, range = 7, shootEffect = 32, effect = 39}
}

monster.defenses = {
	defense = 55,
    armor = 55,
    {name = "combat", type = COMBAT_HEALING,  chance = 30, interval = 2000, minDamage = 350, maxDamage = 600, shootEffect = 32, effect = 13},
	{name = "invisible",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 32, effect = 1},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 460, shootEffect = 32, effect = 14}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = true, combat = true},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)