function onSay(player, words, param)
	if not player:isLiveCasting() then
		return true
	end

	if words == "!start" then
		player:startLiveCasting(param)
	elseif words == "!pause" then
		player:pauseLiveCasting(param) -- param is reason
	elseif words == "!kick" then -- where param is spectator name
		player:kickCastSpectator(param)
	elseif words == "!ban" then
		player:banCastSpectator(param)
	elseif words == "!unban" then
		player:unBanCastSpectator(param)
	elseif words == "!mute" then
		player:muteCastSpectator(param)
	elseif words == "!unmute" then
		player:unMuteCastSpectator(param)
	end
	
	return false
end