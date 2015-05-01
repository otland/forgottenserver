local mType = Game.createMonsterType("Frost Giantess")
local monster = {}
monster.description = "a frost giantess"
monster.experience = 150
monster.outfit = {
	lookType = 265
}

monster.health = 275
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7330
monster.speed = 210
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 60
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Ymirs Mjalle!", yell = false},
	{text = "No run so much, must stay fat!", yell = false},
	{text = "Horre, Sjan Flan!", yell = false},
	{text = "Damned fast food.", yell = false},
	{text = "Come kiss the cook!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 40, chance = 80000},
	{id = "ham", maxCount = 2, chance = 20990},
	{id = "short sword", chance = 7960},
	{id = "frost giant pelt", chance = 4800},
	{id = "ice cube", chance = 2008},
	{id = "battle shield", chance = 1490},
	{id = "norse shield", chance = 320},
	{id = "small stone", maxCount = 3, chance = 10360},
	{id = "club ring", chance = 70},
	{id = "dark helmet", chance = 170},
	{id = "shard", chance = 100},
	{id = "mana potion", chance = 950}
}

monster.attacks = {
	{name = "melee",  attack = 30, skill = 30, minDamage = 0, maxDamage = -60, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 30, interval = 2000, minDamage = 0, maxDamage = -90, range = 7, shootEffect = 12}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 300, shootEffect = 12, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = 10},
	{type = COMBAT_DEATHDAMAGE, percent = -3},
	{type = COMBAT_FIREDAMAGE, percent = 20}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = false, combat = false},
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
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)