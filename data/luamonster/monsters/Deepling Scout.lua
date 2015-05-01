local mType = Game.createMonsterType("Deepling Scout")
local monster = {}
monster.description = "a deepling scout"
monster.experience = 160
monster.outfit = {
	lookType = 413
}

monster.health = 240
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 13839
monster.speed = 200
monsters.runHealth = 50
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
	canPushCreatures = true,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Njaaarh!!", yell = false},
	{text = "Begjone, intrjuder!!", yell = false},
	{text = "Djon't djare stjare injo the eyes of the djeep!", yell = false},
	{text = "Ljeave this sjacred pljace while you cjan", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 50, chance = 60000},
	{id = "hunting spear", maxCount = 3, chance = 14285},
	{id = "life ring", chance = 2127},
	{id = "rusty armor", chance = 925},
	{id = "heavy trident", chance = 505},
	{id = "eye of a deepling", chance = 310},
	{id = "fish fin", chance = 310},
	{id = "small emerald", chance = 121},
	{id = "flask of rust remover", chance = 111}
}

monster.attacks = {
	{name = "melee",  attack = 40, skill = 40, minDamage = 0, maxDamage = -100, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DROWNDAMAGE,  chance = 15, interval = 2000, minDamage = -40, maxDamage = -100, range = 7, shootEffect = 1, effect = 2}
}

monster.elements = {
    {type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = -10}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
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
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)