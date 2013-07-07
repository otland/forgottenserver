function onSay(cid, words, param)
	if getTileHouseInfo(getPlayerPosition(cid)) ~= FALSE then
		if getHouseOwner(getTileHouseInfo(getPlayerPosition(cid))) == getPlayerGUID(cid) then
			setHouseOwner(getTileHouseInfo(getPlayerPosition(cid)), 0)
			doPlayerSendTextMessage(cid, MESSAGE_INFO_DESCR, "You have successfully left your house.")
		else
			doPlayerSendCancel(cid, "You are not the owner of this house.")
			doSendMagicEffect(getPlayerPosition(cid), CONST_ME_POFF)
		end
	else
		doPlayerSendCancel(cid, "You are not inside a house.")
		doSendMagicEffect(getPlayerPosition(cid), CONST_ME_POFF)
	end
end