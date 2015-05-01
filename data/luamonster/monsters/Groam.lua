local mType = Game.createMonsterType("Groam")
local monster = {}
monster.description = "a groam"
monster.experience = 180
monster.outfit = {
	lookType = 413
}

monster.health = 400
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 13839
monster.speed = 560
monsters.runHealth = 60
monster.maxSummons = 0

monster.changeTarget = {
    interval = 2000,
	chance = 50
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
	staticAttack = 95
}

monster.voices = {
	interval = 3000,
	chance = 200,
	{text = "Njaaarh!!", yell = false},
	{text = "Begjone, intrjuder!!", yell = false},
	{text = "Djon't djare stjare injo the eyes of the djeep!", yell = false},
	{text = "Ljeave this sjacred pljace while you cjan", yell = false}
}

monster.loot = {
    {id = "gold coin", maxCount = 50, chance = 100000},
	{id = "life ring", chance = 50000},
	{id = "hunting spear", maxCount = 4, chance = 25000},
	{id = "rusty armor", chance = 14285}
}

monster.attacks = {
	{name = "melee",  attack = 45, skill = 34, minDamage = 0, maxDamage = -99, interval = 2000, effect = 0},
	{name = "combat", type = COMBAT_DROWNDAMAGE,  chance = 11, interval = 1000, minDamage = -15, maxDamage = -100, range = 5, shootEffect = 1, effect = 2}
}

monster.elements = {
    {type = COMBAT_PHYSICALDAMAGE, percent = -20},
	{type = COMBAT_ENERGYDAMAGE, percent = -20},
	{type = COMBAT_EARTHDAMAGE, percent = -20},
	{type = COMBAT_DEATHDAMAGE, percent = -20}
}

monster.immunities = {
	{type = "drown", condition = true, combat = true},
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
	{type = "fire", condition = true, combat = true},
	{type = "ice", condition = true, combat = true}
}

mType:register(monster)