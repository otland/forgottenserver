local mType = Game.createMonsterType("Feverish Citizen")
local monster = {}
monster.description = "a feverish citizen"
monster.experience = 30
monster.outfit = {
	lookType = 425
}

monster.health = 125
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 20395
monster.speed = 185
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Aaaaargh!", yell = false},
	{text = "I am the king of the world!", yell = false},
	{text = "Die Ferumbras!", yell = false},
	{text = "Tigerblood is running through my veins!", yell = false},
	{text = "You! It's you again!", yell = false},
	{text = "Stand still you tasty morsel!", yell = false},
	{text = "<giggle>", yell = false},
	{text = "Burn heretic! Burn!", yell = false},
	{text = "Harrr!", yell = false},
	{text = "This is Venore!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 35, chance = 30300},
	{id = "worm", maxCount = 3, chance = 25730},
	{id = "bone", chance = 4920},
	{id = "luminous piece of cloth", chance = 2290},
	{id = "obvious piece of cloth", chance = 2006},
	{id = "dubious piece of cloth", chance = 1720},
	{id = "voluminous piece of cloth", chance = 2290},
	{id = "ominous piece of cloth", chance = 1660},
	{id = "ludicrous piece of cloth", chance = 2060},
	{id = "plague bell", chance = 100},
	{id = "plague mask", chance = 100}
}

monster.attacks = {
	{name = "melee",  attack = 10, skill = 25, minDamage = 0, maxDamage = -18, interval = 2000, effect = 0},
	{name = "drunk",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, length = 3, spread = 2, effect = 21}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "outfit",  chance = 1, interval = 2000, minDamage = 0, maxDamage = 0, radius = 3, effect = 9}
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