local mType = Game.createMonsterType("Orc Berserker")
local monster = {}
monster.description = "a orc berserker"
monster.experience = 195
monster.outfit = {
	lookType = 8
}

monster.health = 210
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5980
monster.speed = 195
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = true,
	convinceable = true,
    illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "KRAK ORRRRRRK!", yell = true}
}

monster.loot = {
    {id = "gold coin", maxCount = 12, chance = 54000},
	{id = "ham", chance = 10400},
	{id = "orc tooth", chance = 3000},
	{id = "chain armor", chance = 890},
	{id = "lamp", chance = 830},
	{id = "halberd", chance = 7280},
	{id = "battle axe", chance = 6110},
	{id = "hunting spear", chance = 5000},
	{id = "orcish gear", chance = 9400},
	{id = "orc leather", chance = 4000}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 40, interval = 2000, effect = 0}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 6000, speedChange = 290, effect = 14}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 15},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE, percent = 10}
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
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)