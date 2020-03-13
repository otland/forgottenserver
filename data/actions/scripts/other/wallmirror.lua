local messages = {
	"You could win a beauty contest today!",
	"You rarely looked better.",
	"Well, you can't look good every day.",
	"You should think about a makeover.",
	"Is that the indication of a potbelly looming under your clothes?",
	"You look irresistible.",
	"You look tired.",
	"You look awesome!",
	"You nearly don't recognize yourself.",
	"You look fabulous.",
	"Surprise, surprise, you don't see yourself."
}

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if player:getStorageValue(PlayerStorageKeys.delayWallMirror) <= os.time() then
		player:say(messages[math.random(1, #messages)], TALKTYPE_MONSTER_SAY)
		player:setStorageValue(PlayerStorageKeys.delayWallMirror, os.time() + 72000)
	else
		player:say("Don't be so vain about your appearance.", TALKTYPE_MONSTER_SAY)
	end
	return true
end
