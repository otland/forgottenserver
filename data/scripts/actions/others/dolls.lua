local dollsTable = {
	[5080] = { -- panda teddy
		"Hug me!"
	},
	[5669] = { -- mysterious voodoo skull
		"It's not winning that matters, but winning in style.",
		"Today's your lucky day. Probably.",
		"Do not meddle in the affairs of dragons, for you are crunchy and taste good with ketchup.",
		"That is one stupid question.",
		"You'll need more rum for that.",
		"Do or do not. There is no try.",
		"You should do something you always wanted to.",
		"If you walk under a ladder and it falls down on you it probably means bad luck.",
		"Never say 'oops'. Always say 'Ah, interesting!'",
		"Five steps east, fourteen steps south, two steps north and seventeen steps west!"
	},
	[6566] = { -- stuffed dragon
		"Fchhhhhh!",
		"Zchhhhhh!",
		"Grooaaaaar*cough*",
		"Aaa... CHOO!",
		"You... will.... burn!!"
	},
	[6388] = { -- christmas card
		"Merry Christmas |PLAYERNAME|."
	},
	[6512] = { -- santa doll
		"Ho ho ho",
		"Jingle bells, jingle bells...",
		"Have you been naughty?",
		"Have you been nice?",
		"Merry Christmas!",
		"Can you stop squeezing me now... I'm starting to feel a little sick."
	},
	[8974] = { -- oracle figurine
		"ARE YOU PREPARED TO FACE YOUR DESTINY?"
	},
	[8977] = { -- encyclopedia
		"Weirdo, you're a weirdo! Actually all of you are!",
		"Pie for breakfast, pie for lunch and pie for dinner!",
		"All hail the control panel!",
		"I own, god owns, perfect match!",
		"Hug me! Feed me! Hail me!"
	},
	[8981] = { -- golden newspaper
		"It's news to me.",
		"News, updated as infrequently as possible!",
		"Extra! Extra! Read all about it!",
		"Fresh off the press!"
	},
	[8982] = { -- norseman doll
		"Hail!",
		"So cold.",
		"Run, mammoth!"
	},
	[13030] = { -- draken doll
		"For zze emperor!",
		"Hail!",
		"Hail |PLAYERNAME|!"
	},
	[13559] = { -- dread doll
		"Mhausheausheu! What a FAIL! Mwahaha!",
		"Hail |PLAYERNAME|! You are wearing old socks!",
		"You are so unpopular even your own shadow refuses to follow you.",
		"Have fun!"
	},
	[10063] = { -- Epaminondas doll
		"Hail |PLAYERNAME|! Hail!",
		"Hauopa!",
		"WHERE IS MY HYDROMEL?!",
		"Yala Boom"
	},
	[16107] = { -- doll of Durin The Almighty
		"My powers are limitless!",
		"Hail!"
	},
	[20624] = { -- bookworm doll
		"Hail!",
		"Shhhhhh, please be quiet!",
		"Books are great!! Aren't they?"
	},
	[23806] = { -- black knight doll
		"I can hear their whisperings... Revenge!",
		"You shall feel pain and terror, |PLAYERNAME|",
		"I do not need a sword to slaughter you",
		"My sword is broken, but my spirit is not dead",
		"I can say 469 and more...",
		"My dark magic lies on the world"
	},
	[24316] = { -- midnight panther doll
		"Hail!",
		"Don't be afraid of the darkness!",
		"Feel lucky, |PLAYERNAME|!",
		"Purrrrrrr!"
	},
	[24331] = { -- assassin doll
		"Hail! (União&Força)",
		"Hail |PLAYERNAME|! (União&Força)",
		"Only the real killers can touch me!",
		"The path of assassin is found in death, DIE!",
		"<FART...> Ahhh... silent and deadly..."
	},
	[24776] = { -- Little Adventurer Doll
		"Silence! I smell something!",
		"Let me guide you, |PLAYERNAME|!",
		"I have a bad feeling about this.",
		"Watch your steps - we found the pit latrine."
	}
}

local dolls = Action()

function dolls.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local sounds = dollsTable[item:getId()]
	if not sounds then
		return false
	end

	if fromPosition.x == CONTAINER_POSITION then
		fromPosition = player:getPosition()
	end

	local chance = math.random(#sounds)
	local sound = sounds[chance]
	if item:getId() == 5669 then -- mysterious voodoo skull
		player:addAchievementProgress("Superstitious", 100)
		fromPosition:sendMagicEffect(CONST_ME_MAGIC_RED)
		item:transform(item:getId() + 1)
		item:decay()
	elseif item:getId() == 6388 then -- christmas card
		fromPosition:sendMagicEffect(CONST_ME_SOUND_YELLOW)
	elseif item:getId() == 6566 then -- stuffed dragon
		if chance == 3 then
			fromPosition:sendMagicEffect(CONST_ME_POFF)
		elseif chance == 4 then
			fromPosition:sendMagicEffect(CONST_ME_FIREAREA)
		elseif chance == 5 then
			doTargetCombat(0, player, COMBAT_PHYSICALDAMAGE, -1, -1, CONST_ME_EXPLOSIONHIT)
		end
	elseif item:getId() == 10063 then -- Epaminondas doll
		item:transform(item:getId() + 1)
		item:decay()
	elseif item:getId() == 13559 then -- dread doll
		item:transform(13581)
		item:decay()
	elseif item:getId() == 16107 then -- doll of Durin The Almighty
		item:transform(item:getId() + 1)
		item:decay()
	elseif item:getId() == 23806 then -- black knight doll
		item:transform(item:getId() + 1)
		item:decay()
	elseif item:getId() == 24776 then -- Little Adventurer Doll
		item:transform(item:getId() + 1)
		item:decay()
	end

	sound = sound:gsub("|PLAYERNAME|", player:getName())
	player:say(sound, TALKTYPE_MONSTER_SAY, false, 0, fromPosition)
	return true
end

for k, v in pairs(dollsTable) do
	dolls:id(k)
end
dolls:register()
