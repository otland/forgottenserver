function onCastSpell(cid, var)
	local pos = getPlayerPosition(cid)
	doSendMagicEffect(pos, CONST_ME_MAGIC_BLUE)
	return doSetCreatureLight(cid, 6, 215, (6*60+10)*1000)
end
