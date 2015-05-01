local mType = Game.createMonsterType("Enraged White Deer")
local monster = {}
monster.description = "an enraged white deer"
monster.experience = 165
monster.outfit = {
	lookType = 400
}

monster.health = 255
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 13513
monster.speed = 210
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "*wheeze*", yell = false},
	{text = "ROOOAAARR!!", yell = false},
	{text = "*sniff*", yell = false},
	{text = "*bell*", yell = false}
}

monster.loot = {
    {id = "ham", maxCount = 3, chance = 19850},
	{id = "white deer skin", chance = 20280},
	{id = "white deer antlers", chance = 19520}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, minDamage = 0, maxDamage = -45, interval = 2000, effect = 0}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 4000, minDamage = 10, maxDamage = 40, effect = 13}
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