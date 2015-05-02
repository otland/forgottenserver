local mType = Game.createMonsterType("Gravedigger")
local monster = {}
monster.description = "a gravedigger"
monster.experience = 950
monster.outfit = {
	lookType = 558
}

monster.health = 1500
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 21279
monster.speed = 230
monster.runHealth = 200
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
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 70
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "High Five!", yell = false},
	{text = "scrabble", yell = false},
	{text = "Put it there!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 137, chance = 100000},
	{id = "platinum coin", chance = 24470},
	{id = "unholy bone", chance = 9570},
	{id = "sudden death rune", subType = 3, chance = 7300},
	{id = "safety pin", chance = 6000},
	{id = "wand of inferno", chance = 5590},
	{id = "pile of grave earth", chance = 6650},
	{id = "strong health potion", maxCount = 2, chance = 2260},
	{id = "strong mana potion", maxCount = 2, chance = 3600},
	{id = "yellow gem", chance = 800},
	{id = "death ring", chance = 800},
	{id = "skull staff", chance = 130},
	{id = "lump of dirt", chance = 100},
	{id = "mysterious voodoo skull", chance = 100},
	{id = "hardened bone", chance = 50}
}

monster.attacks = {
	{name = "melee",  attack = 80, skill = 70, minDamage = 0, maxDamage = -320, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_MANADRAIN,  chance = 10, interval = 2000, minDamage = -40, maxDamage = -250, range = 1, effect = 13},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 15, interval = 2000, minDamage = -175, maxDamage = -300, range = 1, shootEffect = 11, effect = 13},
	{name = "drunk",  chance = 10, interval = 2000, minDamage = 0, maxDamage = 0, radius = 5, shootEffect = 11, effect = 39}
}

monster.defenses = {
	defense = 20,
    armor = 20,
    {name = "invisible",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, shootEffect = 11, effect = 14},
	{name = "combat", type = COMBAT_HEALING,  chance = 20, interval = 2000, minDamage = 100, maxDamage = 250, shootEffect = 11, effect = 13},
	{name = "speed",  chance = 15, interval = 2000, minDamage = 0, maxDamage = 0, duration = 5000, speedChange = 420, shootEffect = 11, effect = 14}
}

monster.immunities = {
	{type = "drown", condition = false, combat = false},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = false},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = true, combat = true},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = true, combat = true},
	{type = "earth", condition = false, combat = false},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)