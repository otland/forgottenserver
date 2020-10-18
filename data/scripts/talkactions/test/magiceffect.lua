local talk = TalkAction("!z")

function talk.onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	local effect = tonumber(param)
	if(effect ~= nil and effect > 0) then
		player:getPosition():sendMagicEffect(effect)
	end
	return false
end

talk:separator(" ")
talk:register()
