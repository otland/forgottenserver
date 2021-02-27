local ta = TalkAction('!tutor')

function ta.onSay(player, words, param)
	return false
end

ta:access(true)
ta:accountType(ACCOUNT_TYPE_TUTOR)
ta:separator(' ')
ta:register()
