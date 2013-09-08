local holes = {468, 481, 483}

function onUse(cid, item, fromPosition, itemEx, toPosition)
	doPlayerSendTextMessage(cid, MESSAGE_INFO_DESCR, sayer)
	if isInArray(holes, itemEx.itemid) == TRUE then
		doTransformItem(itemEx.uid, itemEx.itemid + 1)
		doDecayItem(itemEx.uid)
	elseif itemEx.itemid == 231 then
		local rand = math.random(1, 100)
		if(itemEx.actionid  == 100 and rand <= 20) then
			doTransformItem(itemEx.uid, 489)
			doDecayItem(itemEx.uid)
		elseif rand == 1 then
			doCreateItem(2159, 1, toPosition)
		elseif rand > 95 then
			doSummonCreature("Scarab", toPosition)
		end
		doSendMagicEffect(toPosition, CONST_ME_POFF)
	else
		return FALSE
	end
	return TRUE
end
