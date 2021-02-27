local ta = TalkAction('!god')

function ta.onSay(player, words, param)
	return false
end

ta:access(true)
ta:accountType(ACCOUNT_TYPE_GOD)
ta:separator(' ')
ta:register()
