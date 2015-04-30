local mType = Game.createMonsterType("Lethal Lissy")
local monster = {}
monster.description = "lethal lissy"
monster.experience = 500
monster.outfit = {
	lookFeet = 132,
	lookLegs = 76,
	lookHead = 77,
	lookType = 155,
	lookAddons = 3
}

monster.health = 1450
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20438
monster.speed = 240
monster.maxSummons = 4

monster.changeTarget = {
    interval = 60000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 50
}

monster.summons = {
    {name = "Pirate Cutthroat", chance = 50, interval = 2000}
}

monster.loot = {
    {id = "the Lethal Lissy's shirt", chance = 100000},
	{id = "gold coin", maxCount = 40, chance = 100000},
	{id = "knight armor", chance = 1200},
	{id = "double axe", chance = 1500},
	{id = "plate armor", chance = 4000},
	{id = "small diamond", chance = 100000},
	{id = "skull of Ratha", chance = 100000}
}

monster.attacks = {
	{name = "melee",  attack = 0, skill = 0, interval = 2000, effect = 0}
}

monster.defenses = {
	defense = 50,
    armor = 35,
    {name = "combat", type = COMBAT_HEALING,  chance = 65, interval = 6000, minDamage = 200, maxDamage = 250, effect = 13}
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