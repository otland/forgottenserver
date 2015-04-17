-- a testing layout
local test = NewMonsterType('Test')
local monster = {}
monster.description = 'a Test'
monster.experience = 5000
monster.outfit = {
    lookType = 698,
    lookMount = 368,
    lookAddons = 3
}

monster.health = 5000
monster.maxHealth = monster.health
monster.race = 'fire'
monster.skull = "black"

monster.flags = {
    attackable = true,
    hiddenHealth = false,
    convinceable = true
}

monster.voices = {
    {
		text = 'Works', 
		interval = 5000, 
		chance = 10000, 
		yell = false
	},
    {
		text = 'I can talk', 
		interval = 5000, 
		chance = 10000, 
		yell = false
	},
    {
		text = 'Test', 
		interval = 5000, 
		chance = 10000, 
		yell = false
	}
}

test:register(monster)





