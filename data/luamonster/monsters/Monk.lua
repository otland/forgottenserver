local mType = Game.createMonsterType("Monk")
local monster = {}
monster.description = "a monk"
monster.experience = 200
monster.outfit = {
	lookType = 57
}

monster.health = 240
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20559
monster.speed = 200
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = true,
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
	{text = "Repent Heretic!", yell = false},
	{text = "A prayer to the almighty one!", yell = false},
	{text = "I will punish the sinners!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 18, chance = 15000},
	{id = "bread", chance = 20000},
	{id = "scroll", chance = 2000},
	{id = "lamp", chance = 880},
	{id = "book of prayers", chance = 4930},
	{id = "brown flask", chance = 820},
	{id = "rope belt", chance = 2950},
	{id = "safety pin", chance = 1001},
	{id = "sandals", chance = 710},
	{id = "staff", chance = 440},
	{id = "life crystal", chance = 1002},
	{id = "ankh", chance = 2240},
	{id = "power ring", chance = 100}
}

monster.attacks = {
	{name = "melee",  attack = 65, skill = 30, minDamage = 0, maxDamage = -130, interval = 2000, effect = 14}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 30, maxDamage = 50, effect = 13},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 300, effect = 14}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = 50},
	{type = COMBAT_HOLYDAMAGE, percent = 50}
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