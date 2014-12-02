function destroyItem(targetEx, toPosition)
	if targetEx:getUniqueId() <= 65535 or targetEx:getActionId() > 0 then
		return false
	end
	--chest,crate,barrel...
	local id = targetEx:getId()
	if (id >= 1724 and id <= 1741) or (id >= 2581 and id <= 2588) or id == 1770 or id == 2098 or id == 1774 or id == 1775 or id == 2064 or (id >= 1747 and id <= 1753) or (id >= 1714 and id <= 1717) or (id >= 1650 and id <= 1653) or (id >= 1666 and id <= 1677) or (id >= 1614 and id <= 1616) or (id >= 3813 and id <= 3820) or (id >= 3807 and id <= 3810) or (id >= 2080 and id <= 2085) or (id >= 2116 and id <= 2119) or id == 2094 or id == 2095 or id == 1619 or id == 2602 or id == 3805 or id == 3806 then
		if math.random(1, 7) == 1 then
			if id == 1738 or id == 1739 or (id >= 2581 and id <= 2588) or id == 1770 or id == 2098 or id == 1774 or id == 1775 or id == 2064 then
				Game.createItem(2250, 1, toPosition)
			elseif (id >= 1747 and id <= 1749) or id == 1740 then
				Game.createItem(2251, 1, toPosition)
			elseif (id >= 1714 and id <= 1717) then
				Game.createItem(2252, 1, toPosition)
			elseif (id >= 1650 and id <= 1653) or (id >= 1666 and id <= 1677) or (id >= 1614 and id <= 1616) or (id >= 3813 and id <= 3820) or (id >= 3807 and id <= 3810) then
				Game.createItem(2253, 1, toPosition)
			elseif (id >= 1724 and id <= 1737) or (id >= 2080 and id <= 2085) or (id >= 2116 and id <= 2119) or id == 2094 or id == 2095 then
				Game.createItem(2254, 1, toPosition)
			elseif (id >= 1750 and id <= 1753) or id == 1619 or id == 1741 then
				Game.createItem(2255, 1, toPosition)
			elseif id == 2602 then
				Game.createItem(2257, 1, toPosition)
			elseif id == 3805 or id == 3806 then
				Game.createItem(2259, 1, toPosition)
			end
			targetEx:remove(1)
		end
		toPosition:sendMagicEffect(CONST_ME_POFF)
		return true
	end
	--spiderweb
	if id == 7538 or id == 7539 then 
		if math.random(1, 7) == 1 then
			if id == 7538 then
				targetEx:transform(7544)
			elseif id == 7539 then
				targetEx:transform(7545)
			end
			targetEx:decay()
		end
		toPosition:sendMagicEffect(CONST_ME_POFF)
		return true
	end
	--wooden bar
	if id == 3798 or id == 3799 then 
		if math.random(1, 3) == 1 then
			if id == 3798 then
				targetEx:transform(3959)
			elseif id == 3799 then
				targetEx:transform(3958)
			end
			targetEx:decay()
		end
		toPosition:sendMagicEffect(CONST_ME_POFF)
		return true
	end
	return false
end
