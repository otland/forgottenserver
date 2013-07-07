function destroyItem(cid, itemEx, toPosition)
	if itemEx.uid <= 65535 or itemEx.actionid > 0 then
		return FALSE
	end

	if (itemEx.itemid >= 1724 and itemEx.itemid <= 1741) or (itemEx.itemid >= 2581 and itemEx.itemid <= 2588) or itemEx.itemid == 1770 or itemEx.itemid == 2098 or itemEx.itemid == 1774 or itemEx.itemid == 1775 or itemEx.itemid == 2064 or (itemEx.itemid >= 1747 and itemEx.itemid <= 1753) or (itemEx.itemid >= 1714 and itemEx.itemid <= 1717) or (itemEx.itemid >= 1650 and itemEx.itemid <= 1653) or (itemEx.itemid >= 1666 and itemEx.itemid <= 1677) or (itemEx.itemid >= 1614 and itemEx.itemid <= 1616) or (itemEx.itemid >= 3813 and itemEx.itemid <= 3820) or (itemEx.itemid >= 3807 and itemEx.itemid <= 3810) or (itemEx.itemid >= 2080 and itemEx.itemid <= 2085) or (itemEx.itemid >= 2116 and itemEx.itemid <= 2119) or itemEx.itemid == 2094 or itemEx.itemid == 2095 or itemEx.itemid == 1619 or itemEx.itemid == 2602 or itemEx.itemid == 3805 or itemEx.itemid == 3806 then
		if math.random(1, 7) == 1 then
			if itemEx.itemid == 1738 or itemEx.itemid == 1739 or (itemEx.itemid >= 2581 and itemEx.itemid <= 2588) or itemEx.itemid == 1770 or itemEx.itemid == 2098 or itemEx.itemid == 1774 or itemEx.itemid == 1775 or itemEx.itemid == 2064 then
				doCreateItem(2250, 1, toPosition)
			elseif (itemEx.itemid >= 1747 and itemEx.itemid <= 1749) or itemEx.itemid == 1740 then
				doCreateItem(2251, 1, toPosition)
			elseif (itemEx.itemid >= 1714 and itemEx.itemid <= 1717) then
				doCreateItem(2252, 1, toPosition)
			elseif (itemEx.itemid >= 1650 and itemEx.itemid <= 1653) or (itemEx.itemid >= 1666 and itemEx.itemid <= 1677) or (itemEx.itemid >= 1614 and itemEx.itemid <= 1616) or (itemEx.itemid >= 3813 and itemEx.itemid <= 3820) or (itemEx.itemid >= 3807 and itemEx.itemid <= 3810) then
				doCreateItem(2253, 1, toPosition)
			elseif (itemEx.itemid >= 1724 and itemEx.itemid <= 1737) or (itemEx.itemid >= 2080 and itemEx.itemid <= 2085) or (itemEx.itemid >= 2116 and itemEx.itemid <= 2119) or itemEx.itemid == 2094 or itemEx.itemid == 2095 then
				doCreateItem(2254, 1, toPosition)
			elseif (itemEx.itemid >= 1750 and itemEx.itemid <= 1753) or itemEx.itemid == 1619 or itemEx.itemid == 1741 then
				doCreateItem(2255, 1, toPosition)
			elseif itemEx.itemid == 2602 then
				doCreateItem(2257, 1, toPosition)
			elseif itemEx.itemid == 3805 or itemEx.itemid == 3806 then
				doCreateItem(2259, 1, toPosition)
			end
			doRemoveItem(itemEx.uid, 1)
		end
		doSendMagicEffect(toPosition, CONST_ME_POFF)
		return TRUE
	end
	return FALSE
end