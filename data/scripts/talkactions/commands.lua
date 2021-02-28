local ta = TalkAction('/commands','!commands')

local config = {
	dialogIcon = 2160
}

function ta.onSay(player, words, param)
	local access = player:getGroup():getAccess()
	local accType = player:getAccountType()
	local description = ''

	local talkActions = Game.getTalkactions()
	for i = 1, #talkActions do
		local tAccess  = talkActions[i]:getAccess()
		local tAccType = talkActions[i]:getAccountType()
		if (not tAccess or (access and tAccess)) and (tAccType <= accType) then
			description = description .. talkActions[i]:getWords() .. '\n'
		end
	end

	player:showTextDialog(config.dialogIcon, description)
	return false
end

ta:access(false)
ta:accountType(ACCOUNT_TYPE_NORMAL)
ta:separator(' ')
ta:register()
