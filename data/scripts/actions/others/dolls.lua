local dollsTable = {
	[5080] = {"Hug me!"},
	[5668] = {
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
	[6387] = {"Merry Christmas |PLAYERNAME|."},
	[6511] = {
		"Ho ho ho",
		"Jingle bells, jingle bells...",
		"Have you been naughty?",
		"Have you been nice?",
		"Merry Christmas!",
		"Can you stop squeezing me now... I'm starting to feel a little sick."
	},
	[8146] = {"ARE YOU PREPARED TO FACE YOUR DESTINY?"},
	[8149] = {
		"Weirdo, you're a weirdo! Actually all of you are!",
		"Pie for breakfast, pie for lunch and pie for dinner!",
		"All hail the control panel!",
		"I own, god owns, perfect match!",
		"Hug me! Feed me! Hail me!"
	},
	[8153] = {
		"It's news to me.",
		"News, updated as infrequently as possible!",
		"Extra! Extra! Read all about it!",
		"Fresh off the press!"
	},
	[8154] = {
		"Hail!",
		"So cold.",
		"Run, mammoth!"
	},
	[9144] = {
		"Hail |PLAYERNAME|! Hail!",
		"Hauopa!",
		"WHERE IS MY HYDROMEL?!",
		"Yala Boom"
	},
	[12043] = {
		"For zze emperor!",
		"Hail!",
		"Hail |PLAYERNAME|!"
	},
	[12904] = {
		"Mhausheausheu! What a FAIL! Mwahaha!",
		"Hail |PLAYERNAME|! You are wearing old socks!",
		"You are so unpopular even your own shadow refuses to follow you.",
		"Have fun!"
	},
	[14764] = {
		"My powers are limitless!",
		"Hail!"
	},
	[18343] = {
		"Hail!",
		"Shhhhhh, please be quiet!",
		"Books are great!! Aren't they?"
	},
	[21435] = {
		"I can hear their whisperings... Revenge!",
		"You shall feel pain and terror, |PLAYERNAME|",
		"I do not need a sword to slaughter you",
		"My sword is broken, but my spirit is not dead",
		"I can say 469 and more...",
		"My dark magic lies on the world"
	},
	[21947] = {
		"Hail!",
		"Don't be afraid of the darkness!",
		"Feel lucky, |PLAYERNAME|!",
		"Purrrrrrr!"
	},
	[21962] = {
		"Hail! (União&Força)",
		"Hail |PLAYERNAME|! (União&Força)",
		"Only the real killers can touch me!",
		"The path of assassin is found in death, DIE!",
		"<FART...> Ahhh... silent and deadly..."
	},
	[22120] = {
		"Silence! I smell something!",
		"Let me guide you, |PLAYERNAME|!",
		"I have a bad feeling about this.",
		"Watch your steps - we found the pit latrine."
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
	if item.itemid == 5668 then
		fromPosition:sendMagicEffect(CONST_ME_MAGIC_RED)
		item:transform(item.itemid + 1)
		item:decay()
	elseif item.itemid == 6387 then
		fromPosition:sendMagicEffect(CONST_ME_SOUND_YELLOW)
	elseif item.itemid == 6566 then
		if chance == 3 then
			fromPosition:sendMagicEffect(CONST_ME_POFF)
		elseif chance == 4 then
			fromPosition:sendMagicEffect(CONST_ME_FIREAREA)
		elseif chance == 5 then
			player:sendMagicEffect(CONST_ME_EXPLOSIONHIT)
			player:addHealth(-1)
		end
	elseif item.itemid == 9144 then
		item:transform(item.itemid + 1)
		item:decay()
	elseif item.itemid == 12904 then
		item:transform(12905)
		item:decay()
	elseif item.itemid == 14764 then
		item:transform(item.itemid + 1)
		item:decay()
	elseif item.itemid == 21435 then
		item:transform(item.itemid + 1)
		item:decay()
	elseif item.itemid == 22120 then
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
