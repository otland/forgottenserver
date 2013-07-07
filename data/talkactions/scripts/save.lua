local savingEvent = 0

function onSay(cid, words, param)
	if getPlayerAccess(cid) ~= 0 then
		if isNumber(param) == TRUE then
			stopEvent(savingEvent)
			save(tonumber(param) * 60 * 1000)
		else
			saveData()
		end
	end
end

function save(delay)
	saveData()
	if delay > 0 then
		savingEvent = addEvent(save, delay, delay)
	end
end