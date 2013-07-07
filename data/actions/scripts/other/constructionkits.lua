local constructionKits = {[3901] = 1652, [3902] = 1658, [3903] = 1666, [3904] = 1670, [3905] = 3813, [3906] = 3817, [3907] = 3821, [3908] = 2602, [3909] = 1614, [3910] = 1615, [3911] = 1616, [3912] = 1619, [3913] = 3805, [3914] = 3807, [3915] = 1740,  [3917] = 2084, [3918] = 2095, [3919] = 3809, [3920] = 3811, [3921] = 1716, [3923] = 1774, [3926] = 2080, [3927] = 2098, [3928] = 2104, [3929] = 2101, [3931] = 2105, [3932] = 1724, [3933] = 1728, [3934] = 1732, [3935] = 1775, [3936] = 3832, [3937] = 2064, [3938] = 1750}
function onUse(cid, item, fromPosition, itemEx, toPosition)
	if fromPosition.x == CONTAINER_POSITION then
		doPlayerSendCancel(cid, "Put the construction kit on the floor first.")
	elseif getTileHouseInfo(fromPosition) == FALSE then
		doPlayerSendCancel(cid,"You may only construct this inside a house.")
	elseif constructionKits[item.itemid] ~= nil then
		doTransformItem(item.uid, constructionKits[item.itemid])
		doSendMagicEffect(fromPosition, CONST_ME_POFF)
	else
		return FALSE
	end
	return TRUE
end