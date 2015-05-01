local mType = Game.createMonsterType("Lizard Legionnaire")
local monster = {}
monster.description = "a lizard legionnaire"
monster.experience = 1100
monster.outfit = {
	lookType = 338
}

monster.health = 1400
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 11276
monster.speed = 230
monsters.runHealth = 10
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
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Tssss!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 44000},
	{id = "gold coin", maxCount = 65, chance = 54000},
	{id = "legionnaire flags", chance = 1940},
	{id = "lizard scale", maxCount = 3, chance = 980},
	{id = "Zaoan halberd", chance = 960},
	{id = "strong health potion", chance = 3880},
	{id = "broken halberd", chance = 14940},
	{id = "lizard leather", chance = 970},
	{id = "drakinata", chance = 710},
	{id = "red lantern", chance = 530},
	{id = "bunch of ripe rice", chance = 1950},
	{id = "small diamond", maxCount = 2, chance = 1001},
	{id = "Zaoan shoes", chance = 460},
	{id = "lizard trophy", chance = 20},
	{id = "Zaoan armor", chance = 70}
}

monster.attacks = {
	{name = "melee",  attack = 60, skill = 50, minDamage = 0, maxDamage = -180, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 40, interval = 2000, minDamage = 0, maxDamage = -200, range = 7, shootEffect = 1}
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
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)