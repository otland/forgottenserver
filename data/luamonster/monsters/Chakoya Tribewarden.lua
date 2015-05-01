local mType = Game.createMonsterType("Chakoya Tribewarden")
local monster = {}
monster.description = "a chakoya tribewarden"
monster.experience = 40
monster.outfit = {
	lookType = 259
}

monster.health = 68
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 7320
monster.speed = 200
monster.maxSummons = 0

monster.changeTarget = {
    interval = 60000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Quisavu tukavi!", yell = false},
	{text = "Si siyoqua jamjam!", yell = false},
	{text = "Achuq! jinuma!", yell = false},
	{text = "Si ji jusipa!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 30, chance = 60000},
	{id = "fish", chance = 20000},
	{id = "bone shield", chance = 1000},
	{id = "mammoth whopper", chance = 300},
	{id = "rainbow trout", chance = 130},
	{id = "green perch", chance = 190},
	{id = "northern pike", chance = 90},
	{id = "short sword", chance = 4680}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 20, minDamage = 0, maxDamage = -30, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -15},
	{type = COMBAT_HOLYDAMAGE, percent = 10},
	{type = COMBAT_DEATHDAMAGE, percent = -5},
	{type = COMBAT_FIREDAMAGE, percent = 25}
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