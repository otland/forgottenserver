local mType = Game.createMonsterType("Parrot")
local monster = {}
monster.description = "a parrot"
monster.experience = 0
monster.outfit = {
	lookType = 217
}

monster.health = 25
monster.maxHealth = monster.health
monster.race = "blood"
monster.corpse = 6056
monster.speed = 260
monsters.runHealth = 25
monster.maxSummons = 0

monster.changeTarget = {
    interval = 5000,
	chance = 0
}

monster.flags = {
	summonable = true,
    attackable = true,
	hostile = false,
	convinceable = true,
    illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	targetDistance = 1,
	staticAttack = 90
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "BR? PL? SWE?", yell = false},
	{text = "Screeech!", yell = false},
	{text = "Neeeewbiiieee!", yell = false},
	{text = "You advanshed, you advanshed!", yell = false},
	{text = "Hope you die and loooosh it!", yell = false},
	{text = "Hunterrr ish PK!", yell = false},
	{text = "Shhtop whining! Rrah", yell = false},
	{text = "I'm heeerrre! Screeeech!", yell = false},
	{text = "Leeave orrr hunted!!", yell = false},
	{text = "Blesshhh my stake! Screeech!", yell = false},
	{text = "Tarrrrp?", yell = false},
	{text = "You are corrrrupt! Corrrrupt!", yell = false},
	{text = "You powerrrrrrabuserrrrr!", yell = false},
	{text = "You advanshed, you advanshed!", yell = false}
}

monster.attacks = {
	{name = "melee",  attack = 5, skill = 10, minDamage = 0, maxDamage = -5, interval = 2000, effect = 0}
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