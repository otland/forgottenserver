function onAddItem(moveitem, tileitem, position)
	if moveitem:getId() == 2693 then -- timber wall
		moveitem:transform(2689) -- bread
		position:sendMagicEffect(CONST_ME_HITBYFIRE)
	elseif moveitem:getId() == 6277 then -- lump of cake dough
		moveitem:transform(2687, 12) -- cookie
		position:sendMagicEffect(CONST_ME_HITBYFIRE)
	end
	return true
end
