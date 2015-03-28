function destroyItem(cid, target, toPosition)
	if target == nil or not target:isItem() then
		return false
	end

	if target:hasAttribute(ITEM_ATTRIBUTE_UNIQUEID) or target:hasAttribute(ITEM_ATTRIBUTE_ACTIONID) then
		return false
	end

	local targetId = itemEx:getId()
	if (targetId >= 1724 and targetId <= 1741) or (targetId >= 2581 and targetId <= 2588) or targetId == 1770 or targetId == 2098 or targetId == 1774 or targetId == 1775 or targetId == 2064 or (targetId >= 1747 and targetId <= 1753) or (targetId >= 1714 and targetId <= 1717) or (targetId >= 1650 and targetId <= 1653) or (targetId >= 1666 and targetId <= 1677) or (targetId >= 1614 and targetId <= 1616) or (targetId >= 3813 and targetId <= 3820) or (targetId >= 3807 and targetId <= 3810) or (targetId >= 2080 and targetId <= 2085) or (targetId >= 2116 and targetId <= 2119) or targetId == 2094 or targetId == 2095 or targetId == 1619 or targetId == 2602 or targetId == 3805 or targetId == 3806 then
		if math.random(1, 7) == 1 then
			target:remove(1)
			if targetId == 1738 or targetId == 1739 or (targetId >= 2581 and targetId <= 2588) or targetId == 1770 or targetId == 2098 or targetId == 1774 or targetId == 1775 or targetId == 2064 then
				Game.createItem(2250, 1, toPosition)
			elseif (targetId >= 1747 and targetId <= 1749) or targetId == 1740 then
				Game.createItem(2251, 1, toPosition)
			elseif (targetId >= 1714 and targetId <= 1717) then
				Game.createItem(2252, 1, toPosition)
			elseif (targetId >= 1650 and targetId <= 1653) or (targetId >= 1666 and targetId <= 1677) or (targetId >= 1614 and targetId <= 1616) or (targetId >= 3813 and targetId <= 3820) or (targetId >= 3807 and targetId <= 3810) then
				Game.createItem(2253, 1, toPosition)
			elseif (targetId >= 1724 and targetId <= 1737) or (targetId >= 2080 and targetId <= 2085) or (targetId >= 2116 and targetId <= 2119) or targetId == 2094 or targetId == 2095 then
				Game.createItem(2254, 1, toPosition)
			elseif (targetId >= 1750 and targetId <= 1753) or targetId == 1619 or targetId == 1741 then
				Game.createItem(2255, 1, toPosition)
			elseif targetId == 2602 then
				Game.createItem(2257, 1, toPosition)
			elseif targetId == 3805 or targetId == 3806 then
				Game.createItem(2259, 1, toPosition)
			end
		end
		toPosition:sendMagicEffect(CONST_ME_POFF)
		return true
	end

	--spider web
	if targetId == 7538 or targetId == 7539 then 
		if math.random(1, 7) == 1 then
			if targetId == 7538 then
				target:transform(7544)
			elseif targetId == 7539 then
				target:transform(7545)
			end
			target:decay()
		end
		toPosition:sendMagicEffect(CONST_ME_POFF)
		return true
	end

	--wooden bar
	if targetId == 3798 or targetId == 3799 then 
		if math.random(1, 3) == 1 then
			if targetId == 3798 then
				target:transform(3959)
			elseif targetId == 3799 then
				target:transform(3958)
			end
			target:decay()
		end
		toPosition:sendMagicEffect(CONST_ME_POFF)
		return true
	end
	return false
end
