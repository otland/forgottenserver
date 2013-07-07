function onUse(cid, item, fromPosition, itemEx, toPosition)
	if item.itemid == ITEM_BLUEBERRYBUSH then
		doTransformItem(item.uid, ITEM_BUSH)
		doCreateItem(ITEM_BLUEBERRY, 3, fromPosition)
		addEvent(transformBack, 300000, {position = fromPosition})
	end
	return TRUE
end

function transformBack(parameters)
	parameters.position.stackpos = STACKPOS_TOP_MOVEABLE_ITEM_OR_CREATURE
	local topThing = getThingfromPos(parameters.position)
	if topThing.itemid == ITEM_BLUEBERRY then
		addEvent(transformBack, 300000, parameters)
	else
		for i = STACKPOS_FIRST_ITEM_ABOVE_GROUNDTILE, STACKPOS_TOP_MOVEABLE_ITEM_OR_CREATURE do
			parameters.position.stackpos = i
			topThing = getThingfromPos(parameters.position)
			if topThing.itemid == ITEM_BUSH then
				doTransformItem(topThing.uid, ITEM_BLUEBERRYBUSH)
				break
			end
		end
	end
end