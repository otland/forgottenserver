local mType = Game.createMonsterType("Elf Arcanist")
local monster = {}
monster.description = "a elf arcanist"
monster.experience = 175
monster.outfit = {
	lookType = 63
}

monster.health = 220
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6011
monster.speed = 170
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
    illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	targetDistance = 4,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Feel my wrath!", yell = false},
	{text = "For the Daughter of the Stars!", yell = false},
	{text = "I'll bring balance upon you!", yell = false},
	{text = "Tha'shi Cenath!", yell = false},
	{text = "Vihil Ealuel!", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 47, chance = 37000},
	{id = "arrow", maxCount = 3, chance = 6000},
	{id = "scroll", chance = 31000},
	{id = "blank rune", chance = 18000},
	{id = "sandals", chance = 950},
	{id = "melon", chance = 22000},
	{id = "bread", chance = 14000},
	{id = "green tunic", chance = 7000},
	{id = "elven astral observer", chance = 7710},
	{id = "inkwell", chance = 1000},
	{id = "sling herb", chance = 5000},
	{id = "candlestick", chance = 2100},
	{id = "elven amulet", subType = 50, chance = 1999},
	{id = "holy orchid", chance = 2100},
	{id = "strong mana potion", chance = 3000},
	{id = "health potion", chance = 4000},
	{id = "wand of cosmic energy", chance = 1160},
	{id = "grave flower", chance = 880},
	{id = "life crystal", chance = 970},
	{id = "yellow gem", chance = 50},
	{id = "elvish talisman", chance = 10000}
}

monster.attacks = {
	{name = "melee",  attack = 20, skill = 25, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_PHYSICALDAMAGE,  chance = 10, interval = 2000, minDamage = 0, maxDamage = -70, range = 7, shootEffect = 3},
	{name = "combat", type = COMBAT_ENERGYDAMAGE,  chance = 10, interval = 2000, minDamage = -30, maxDamage = -50, range = 7, shootEffect = 5, effect = 12},
	{name = "combat", type = COMBAT_DEATHDAMAGE,  chance = 10, interval = 2000, minDamage = -70, maxDamage = -85, range = 7, shootEffect = 32, effect = 18}
}

monster.defenses = {
	defense = 15,
    armor = 15,
    {name = "combat", type = COMBAT_HEALING,  chance = 15, interval = 2000, minDamage = 40, maxDamage = 60, shootEffect = 32, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE, percent = -10},
	{type = COMBAT_DEATHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 50}
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