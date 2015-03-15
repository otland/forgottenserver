function onUse(cid, item, fromPosition, target, toPosition, isHotkey)
	if target.itemid == 2782 then
		local iEx = Item(target.uid)
		iEx:transform(2781)
		iEx:decay()
		return true
	end

	return destroyItem(cid, target, toPosition)
end
