local mType = Game.createMonsterType("Hacker")
local monster = {}
monster.description = "a hacker"
monster.experience = 45
monster.outfit = {
	lookType = 8
}

monster.health = 430
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 5980
monster.speed = 250
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 8
}

monster.flags = {
	summonable = false,
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
	{text = "Feel the wrath of me dos attack!", yell = false},
	{text = "You're next!", yell = false},
	{text = "Gimme free gold!", yell = false},
	{text = "Me sooo smart!", yell = false},
	{text = "Me have a cheating link for you!", yell = false},
	{text = "Me is GM!", yell = false},
	{text = "Gimme your password!", yell = false},
	{text = "Me just need the code!", yell = false},
	{text = "Me not stink!", yell = false},
	{text = "Me other char is highlevel!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 12, chance = 100000},
	{id = "ham", chance = 50000},
	{id = "surprise bag", chance = 5538},
	{id = "surprise bag", chance = 1538},
	{id = "lamp", chance = 6666},
	{id = "halberd", chance = 10000},
	{id = "axe", chance = 10000},
	{id = "war hammer", chance = 5000},
	{id = "battle axe", chance = 5000}
}

monster.attacks = {
	{name = "melee",  attack = 55, skill = 20, interval = 1000, effect = 0}
}

monster.defenses = {
	defense = 12,
    armor = 15,
    {name = "speed",  chance = 15, interval = 1000, minDamage = 0, maxDamage = 0, duration = 6000, speedChange = 290, effect = 14},
	{name = "outfit",  chance = 15, interval = 10000, minDamage = 0, maxDamage = 0, effect = 14}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = false, combat = false},
	{type = "drunk", condition = true, combat = false},
	{type = "invisible", condition = false, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = true, combat = true},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)