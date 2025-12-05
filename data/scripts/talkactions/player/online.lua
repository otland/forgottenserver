local talkaction = TalkAction("!online", "/online")

local maxPlayersPerMessage = 10

function talkaction.onSay(player, words, param)
	local players = Game.getPlayers()
	local onlineList = {}

	for _, targetPlayer in ipairs(players) do
		if player:canSeeCreature(targetPlayer) then
			table.insert(onlineList, ("%s [%d]"):format(targetPlayer:getName(), targetPlayer:getLevel()))
		end
	end

	local playersOnline = #onlineList
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, ("%d players online."):format(playersOnline))

	for i = 1, playersOnline, maxPlayersPerMessage do
		local j = math.min(i + maxPlayersPerMessage - 1, playersOnline)
		local msg = table.concat(onlineList, ", ", i, j) .. "."
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, msg)
	end
	return false
end

talkaction:register()
