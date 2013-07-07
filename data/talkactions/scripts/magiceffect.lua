function onSay(cid, words, param)
	if isPlayer(cid) == TRUE and param ~= "" and getPlayerAccess(cid) > 0 then
		doSendMagicEffect(getCreaturePosition(cid), param)
	end
end