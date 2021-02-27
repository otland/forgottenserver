local ta = TalkAction('!st')

function ta.onSay(player, words, param)
	return false
end

ta:access(true)
ta:accountType(ACCOUNT_TYPE_SENIORTUTOR)
ta:separator(' ')
ta:register()
