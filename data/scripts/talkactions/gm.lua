local ta = TalkAction('!gm')

function ta.onSay(player, words, param)
	return false
end

ta:access(true)
ta:accountType(ACCOUNT_TYPE_GAMEMASTER)
ta:separator(' ')
ta:register()
