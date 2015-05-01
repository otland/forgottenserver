local mType = Game.createMonsterType("Grynch Clan Goblin")
local monster = {}
monster.description = "a grynch clan goblin"
monster.experience = 4
monster.outfit = {
	lookType = 61
}

monster.health = 80
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6002
monster.speed = 870
monster.runHealth = 80
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 0
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = false,
	convinceable = false,
    illusionable = false,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 11,
	staticAttack = 0
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "T'was not me hand in your pocket!", yell = false},
	{text = "Look! Cool stuff in house. Let's get it!", yell = false},
	{text = "Uhh! Nobody home!", yell = false},
	{text = "Me just borrowed it!", yell = false},
	{text = "Me no steal! Me found it!", yell = false},
	{text = "Me had it for five minutes. It's family heirloom now!", yell = false},
	{text = "Nice human won't hurt little, good goblin?", yell = false},
	{text = "Gimmegimme!", yell = false},
	{text = "Invite me in you lovely house plx!", yell = false},
	{text = "Other Goblin stole it!", yell = false},
	{text = "All presents mine!", yell = false},
	{text = "Me got ugly ones purse", yell = false},
	{text = "Free itans plz!", yell = false},
	{text = "Not me! Not me!", yell = false},
	{text = "Guys, help me here! Guys? Guys???", yell = false},
	{text = "That only much dust in me pocket! Honest!", yell = false},
	{text = "Can me have your stuff?", yell = false},
	{text = "Halp, Big dumb one is after me!", yell = false},
	{text = "Uh, So many shiny things!", yell = false},
	{text = "Utani hur hur hur!", yell = false},
	{text = "Mee? Stealing? Never!!!", yell = false},
	{text = "Oh what fun it is to steal a one-horse open sleigh!", yell = false},
	{text = "Must have it! Must have it!", yell = false},
	{text = "Where me put me lockpick?", yell = false},
	{text = "Catch me if you can!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 22, chance = 22500},
	{id = "christmas present bag", chance = 7000},
	{id = "snowball", maxCount = 5, chance = 7000},
	{id = "lump of cake dough", maxCount = 3, chance = 7000},
	{id = "orange", maxCount = 3, chance = 7000},
	{id = "cookie", maxCount = 5, chance = 7000},
	{id = "cherry", maxCount = 4, chance = 7000},
	{id = "red apple", maxCount = 3, chance = 700},
	{id = "egg", maxCount = 2, chance = 5000},
	{id = "candy cane", maxCount = 3, chance = 5000},
	{id = "lute", chance = 5000},
	{id = "blank rune", chance = 5000},
	{id = "magic light wand", chance = 4000},
	{id = "picture", chance = 4000},
	{id = "bronze amulet", subType = 200, chance = 4000},
	{id = "scroll", chance = 4000},
	{id = "gingerbreadman", maxCount = 2, chance = 4000},
	{id = "scarf", chance = 4000},
	{id = "chicken feather", maxCount = 5, chance = 4000},
	{id = "bat wing", maxCount = 3, chance = 4000},
	{id = "honeycomb", chance = 4000},
	{id = "explorer brooch", chance = 4000},
	{id = "walnut", maxCount = 5, chance = 3500},
	{id = "peanut", maxCount = 100, chance = 3500},
	{id = "valentine's cake", chance = 1500},
	{id = "very noble-looking watch", chance = 1500},
	{id = "broom", chance = 1500},
	{id = "piggy bank", chance = 1000},
	{id = "dice", chance = 1000},
	{id = "mirror", chance = 1000},
	{id = "blue pillow", chance = 750},
	{id = "red pillow", chance = 750},
	{id = "green pillow", chance = 750},
	{id = "yellow pillow", chance = 750},
	{id = "flower bowl", chance = 500},
	{id = "flask of cough syrup", chance = 500},
	{id = "scarab coin", maxCount = 2, chance = 500},
	{id = "orichalcum pearl", maxCount = 2, chance = 500},
	{id = "crystal coin", chance = 250},
	{id = "elvenhair rope", chance = 250},
	{id = "vampire shield", chance = 250}
}

monster.defenses = {
	defense = 12,
    armor = 10,
    {name = "speed",  chance = 15, interval = 1000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 500, effect = 14}
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)