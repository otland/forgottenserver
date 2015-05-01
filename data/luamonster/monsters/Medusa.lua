local mType = Game.createMonsterType("Medusa")
local monster = {}
monster.description = "a medusa"
monster.experience = 4050
monster.outfit = {
	lookType = 330
}

monster.health = 4500
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 10524
monster.speed = 280
monsters.runHealth = 600
monster.maxSummons = 0

monster.changeTarget = {
    interval = 4000,
	chance = 20
}

monster.flags = {
	summonable = false,
    attackable = true,
	hostile = true,
	convinceable = false,
    illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 80
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "You will make ssuch a fine ssstatue!", yell = false},
	{text = "There isss no chhhanccce of esscape", yell = false},
	{text = "Jusssst look at me!", yell = false},
	{text = "Are you tired or why are you moving thhat sslow<chuckle>", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 100, chance = 50000},
	{id = "gold coin", maxCount = 90, chance = 50000},
	{id = "platinum coin", maxCount = 6, chance = 74810},
	{id = "small emerald", maxCount = 4, chance = 3770},
	{id = "terra amulet", subType = 200, chance = 4060},
	{id = "ultimate health potion", maxCount = 2, chance = 9290},
	{id = "medusa shield", chance = 3040},
	{id = "great mana potion", maxCount = 2, chance = 10000},
	{id = "strand of medusa hair", chance = 9900},
	{id = "terra legs", chance = 420},
	{id = "titan axe", chance = 1160},
	{id = "terra mantle", chance = 870},
	{id = "sacred tree amulet", subType = 5, chance = 850},
	{id = "knight armor", chance = 1840},
	{id = "rusty armor", chance = 500}
}

monster.attacks = {
	{name = "melee",  attack = 100, skill = 80, minDamage = 0, maxDamage = -450, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_LIFEDRAIN,  chance = 20, interval = 2000, minDamage = -21, maxDamage = -350, range = 7, shootEffect = 30, effect = 47},
	{name = "combat", type = COMBAT_EARTHDAMAGE,  chance = 15, interval = 2000, minDamage = -250, maxDamage = -500, length = 8, spread = 3, shootEffect = 30, effect = 47},
	{name = "speed",  chance = 25, interval = 2000, minDamage = 0, maxDamage = 0, target = true, speedChange = -900, radius = 7, shootEffect = 30, effect = 3},
	{name = "outfit",  chance = 1, interval = 2000, minDamage = 0, maxDamage = 0, range = 7, shootEffect = 30, effect = 3}
}

monster.defenses = {
	defense = 30,
    armor = 30,
    {name = "combat", type = COMBAT_HEALING,  chance = 25, interval = 2000, minDamage = 150, maxDamage = 300, shootEffect = 30, effect = 13}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -5}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
	{type = "heal", condition = false, combat = false},
	{type = "lifedrain", condition = false, combat = true},
	{type = "paralyze", condition = true, combat = false},
	{type = "drunk", condition = false, combat = false},
	{type = "invisible", condition = true, combat = false},
	{type = "outfit", condition = false, combat = false},
	{type = "manadrain", condition = false, combat = false},
	{type = "energy", condition = false, combat = false},
	{type = "undefined", condition = false, combat = false},
	{type = "physical", condition = false, combat = false},
	{type = "bleed", condition = false, combat = false},
	{type = "death", condition = false, combat = false},
	{type = "earth", condition = true, combat = true},
	{type = "holy", condition = false, combat = false},
	{type = "fire", condition = false, combat = false},
	{type = "ice", condition = false, combat = false}
}

mType:register(monster)