local ta = TalkAction('/commands','!commands')

local config = {
	path = 'data/scripts/cache/commands/',
	ext = '.txt',
	dialogIcon = 1950
}

function ta.onSay(player, words, param)
	local accType = player:getAccountType()
	local commands = ''

	for i = accType, ACCOUNT_TYPE_NORMAL, -1 do
		local file = config.path .. i .. config.ext
		if file_exists(file) then
			for line in io.lines(file) do
				commands = commands .. line .. '\n'
			end
		end
	end

	player:showTextDialog(config.dialogIcon, commands)
	return false
end

ta:access(false)
ta:accountType(ACCOUNT_TYPE_NORMAL)
ta:separator(' ')
ta:register()

function file_exists(file)
	local f = io.open(file, 'r')
	if f then f:close() end
	return f ~= nil
end
