local dollsTable = {
	[5080] = {"Hug me!"},
	[5669] = {
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
	[6566] = {
		"Fchhhhhh!",
		"Zchhhhhh!",
		"Grooaaaaar*cough*",
		"Aaa... CHOO!",
		"You... will.... burn!!"
	},
	[6388] = {"Merry Christmas |PLAYERNAME|."},
	[6512] = {
		"Ho ho ho",
		"Jingle bells, jingle bells...",
		"Have you been naughty?",
		"Have you been nice?",
		"Merry Christmas!",
		"Can you stop squeezing me now... I'm starting to feel a little sick."
	},
	[8974] = {"ARE YOU PREPARED TO FACE YOUR DESTINY?"},
	[8977] = {
		"Weirdo, you're a weirdo! Actually all of you are!",
		"Pie for breakfast, pie for lunch and pie for dinner!",
		"All hail the control panel!",
		"I own, god owns, perfect match!",
		"Hug me! Feed me! Hail me!"
	},
	[8981] = {
		"It's news to me.",
		"News, updated as infrequently as possible!",
		"Extra! Extra! Read all about it!",
		"Fresh off the press!"
	},
	[8982] = {
		"Hail!",
		"So cold.",
		"Run, mammoth!"
	},
	[23806] = {
		"I can hear their whisperings... Revenge!",
		"You shall feel pain and terror, |PLAYERNAME|",
		"I do not need a sword to slaughter you",
		"My sword is broken, but my spirit is not dead",
		"I can say 469 and more...",
		"My dark magic lies on the world"
	},
	[24331] = {
		"Hail! (União&Força)",
		"Hail |PLAYERNAME|! (União&Força)",
		"Only the real killers can touch me!",
		"The path of assassin is found in death, DIE!",
		"<FART...> Ahhh... silent and deadly..."
	},
	[20624] = {
		"Hail!",
		"Shhhhhh, please be quiet!",
		"Books are great!! Aren't they?"
	},
	[16107] = {
		"My powers are limitless!",
		"Hail!"
	},
	[13030] = {
		"For zze emperor!",
		"Hail!",
		"Hail |PLAYERNAME|!"
	},
	[13559] = {
		"Mhausheausheu! What a FAIL! Mwahaha!",
		"Hail |PLAYERNAME|! You are wearing old socks!",
		"You are so unpopular even your own shadow refuses to follow you.",
		"Have fun!"
	},
	[10063] = {
		"Hail |PLAYERNAME|! Hail!",
		"Hauopa!",
		"WHERE IS MY HYDROMEL?!",
		"Yala Boom"
	},
	[24776] = {
		"Silence! I smell something!",
		"Let me guide you, |PLAYERNAME|!",
		"I have a bad feeling about this.",
		"Watch your steps - we found the pit latrine."
	},
	[24316] = {
		"Hail!",
		"Don't be afraid of the darkness!",
		"Feel lucky, |PLAYERNAME|!",
		"Purrrrrrr!"
	}
}

local dolls = Action()

function dolls.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local sounds = dollsTable[item.itemid]
	if not sounds then
		return false
	end

	if fromPosition.x == CONTAINER_POSITION then
		fromPosition = player:getPosition()
	end

	local chance = math.random(#sounds)
	local sound = sounds[chance]
	if item.itemid == 6566 then
		if chance == 3 then
			fromPosition:sendMagicEffect(CONST_ME_POFF)
		elseif chance == 4 then
			fromPosition:sendMagicEffect(CONST_ME_FIREAREA)
		elseif chance == 5 then
			player:sendMagicEffect(CONST_ME_EXPLOSIONHIT)
			player:addHealth(-1)
		end
	elseif item.itemid == 5669 then
		fromPosition:sendMagicEffect(CONST_ME_MAGIC_RED)
		item:transform(item.itemid + 1)
		item:decay()
	elseif item.itemid == 6388 then
		fromPosition:sendMagicEffect(CONST_ME_SOUND_YELLOW)
	elseif item.itemid == 23806 then
		item:transform(item.itemid + 1)
		item:decay()
	elseif item.itemid == 16107 then
		item:transform(item.itemid + 1)
		item:decay()
	elseif item.itemid == 13559 then
		item:transform(13581)
		item:decay()
	elseif item.itemid == 10063 then
		item:transform(item.itemid + 1)
		item:decay()
	elseif item.itemid == 24776 then
		item:transform(item.itemid + 1)
		item:decay()
	end

	sound = sound:gsub('|PLAYERNAME|', player:getName())
	player:say(sound, TALKTYPE_MONSTER_SAY, false, 0, fromPosition)
	return true
end

for k,v in pairs(dollsTable) do
	dolls:id(k)
end
dolls:register()
