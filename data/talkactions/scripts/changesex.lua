local config = {
	premiumDaysCost = 3
}

function onSay(cid, words, param)
	if getPlayerAccess(cid) > 0 then
		doPlayerSetSex(cid, getPlayerSex(cid) == PLAYERSEX_FEMALE and PLAYERSEX_MALE or PLAYERSEX_FEMALE)
		doPlayerSendTextMessage(cid, MESSAGE_INFO_DESCR, "You have changed your sex.")
		return false
	end

	if getPlayerPremiumDays(cid) >= config.premiumDaysCost then
		doPlayerRemovePremiumDays(cid, config.premiumDaysCost)
		doPlayerSetSex(cid, getPlayerSex(cid) == PLAYERSEX_FEMALE and PLAYERSEX_MALE or PLAYERSEX_FEMALE)
		doPlayerSendTextMessage(cid, MESSAGE_INFO_DESCR, "You have changed your sex for ".. config.premiumDaysCost .." days of your premium account.")
	else
		doPlayerSendCancel(cid, "You do not have enough premium days, changing sex costs ".. config.premiumDaysCost .." days of your premium account.")
		doSendMagicEffect(getPlayerPosition(cid), CONST_ME_POFF)
	end
	return false
end
