local ta = TalkAction('/commands','!commands')

local config = {
	dialogIcon = 1950,
	ACCOUNT_TYPE_MAX = ACCOUNT_TYPE_GOD,
	ACCOUNT_TYPE_MIN = ACCOUNT_TYPE_NORMAL
}

function ta.onSay(player, words, param)
	local access = player:getGroup():getAccess()
	local accType = player:getAccountType()
	local description = ''

	local talkActions = Game.getTalkactions()
	talkActions = categorizeCommands(talkActions)

	for i = config.ACCOUNT_TYPE_MAX, config.ACCOUNT_TYPE_MIN, -1 do
		local talkActionPairs = talkActions[i]
		if talkActionPairs then
			for k, v in pairs(talkActionPairs) do
				local word = v[1]
				local talkaction = v[2]
				local tAccess  = talkaction:getAccess()
				local tAccType = talkaction:getAccountType()
				if (not tAccess or (access and tAccess)) and (tAccType <= accType) then
					description = description .. word .. '\n'
				end
			end
			if description ~= '' then
				description = description .. '\n'
			end
		end
	end

	player:showTextDialog(config.dialogIcon, description)
	return false
end

ta:access(false)
ta:accountType(ACCOUNT_TYPE_NORMAL)
ta:separator(' ')
ta:register()

--[[
	returns new table containing categorized talkactions
	{
		[1] = {
			{word, talkaction}
			{word, talkaction}
		},
		...
	}
]]
function categorizeCommands(talkactions)
	local sorted = {}
	for word, talkaction in pairs(talkactions) do
		local accType = talkaction:getAccountType()
		if not sorted[accType] then
			sorted[accType] = {{word, talkaction}}
		else
			table.insert(sorted[accType], {word, talkaction})
		end
	end
	return sorted
end
