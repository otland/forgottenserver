function onUse(cid, item, fromPosition, itemEx, toPosition)
	local twentyfour = TRUE
	local tibiantime = TRUE
	if tibiantime == FALSE then
		if twentyfour == TRUE then
			time = os.date('%H:%M')
		else
			time = os.date('%I:%M %p')
		end
	else
		varh = (os.date('%M') * 60 + os.date('%S')) / 150
		tibH = math.floor(varh)
		tibM = math.floor(60 * (varh-tibH))
		if tibH < 10 then tibH = '0'..tibH end
		if tibM < 10 then tibM = '0'..tibM end
		time = (tibH..':'..tibM)
	end
	doPlayerSendTextMessage(cid, MESSAGE_INFO_DESCR, 'The time is ' ..time.. '.')
	return TRUE
end