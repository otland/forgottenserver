function destroyItem(cid, itemEx, toPosition)
	if itemEx.uid <= 65535 or itemEx.actionid > 0 then
		return false
	end
	--spiderweb
	if itemEx.itemid == 7538 or itemEx.itemid == 7539 then 
		if math.random(1, 7) == 1 then
			if itemEx.itemid == 7538 then
				Item(itemEx.uid):transform(7544)
			elseif itemEx.itemid == 7539 then
				Item(itemEx.uid):transform(7545)
			end
			Item(itemEx.uid):decay()
		end
		toPosition:sendMagicEffect(CONST_ME_POFF)
		return true
	end
	--wooden bar
	if itemEx.itemid == 3798 or itemEx.itemid == 3799 then 
		if math.random(1, 3) == 1 then
			if itemEx.itemid == 3798 then
				Item(itemEx.uid):transform(3959)
			elseif itemEx.itemid == 3799 then
				Item(itemEx.uid):transform(3958)
			end
			Item(itemEx.uid):decay()
		end
		toPosition:sendMagicEffect(CONST_ME_POFF)
		return true
	end

	if toPosition.x == CONTAINER_POSITION then
		Player(cid):sendCancelMessage(Game.getReturnMessage(RETURNVALUE_NOTPOSSIBLE))
		return true
	end
	--chest,crate,barrel...
	if ItemType(itemEx.itemid):getDestroyId() > -1 then
		if math.random(1, 7) == 1 then
			Item(itemEx.uid):destroy(true)
		end
		toPosition:sendMagicEffect(CONST_ME_POFF)
		return true
	end
	return false
end
