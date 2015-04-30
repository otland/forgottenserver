local mType = Game.createMonsterType("Quara Mantassin Scout")
local monster = {}
monster.description = "a quara mantassin scout"
monster.experience = 100
monster.outfit = {
	lookType = 72
}

monster.health = 220
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6064
monster.speed = 220
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Zuerk Pachak!", yell = false},
	{text = "Shrrrr", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 30, chance = 94000},
	{id = "chain armor", chance = 4761},
	{id = "mantassin tail", chance = 7780},
	{id = "skull", chance = 920},
	{id = "two handed sword", chance = 580},
	{id = "fish fin", chance = 520},
	{id = "stealth ring", chance = 520},
	{id = "small sapphire", chance = 920}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 45, interval = 2000, effect = 0}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)