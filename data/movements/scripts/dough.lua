function onAddItem(moveitem, tileitem, position)
	if moveitem:getId() == 2693 then -- lump of dough
		moveitem:transform(2689) -- bread
		position:sendMagicEffect(CONST_ME_HITBYFIRE)
	elseif moveitem:getId() == 6277 then -- lump of cake dough
		moveitem:transform(6278) -- cake
		position:sendMagicEffect(CONST_ME_HITBYFIRE)
	elseif moveitem:getId() == 9113 then -- lump of garlic dough
		moveitem:transform(9111) -- garlic bread
		position:sendMagicEffect(CONST_ME_HITBYFIRE)
	elseif moveitem:getId() == 8846 then -- lump of chocolate dough
		moveitem:transform(8847) -- chocolate cake
		position:sendMagicEffect(CONST_ME_HITBYFIRE)
	elseif moveitem:getId() == 8848 then -- baking tray (cookie)
		moveitem:transform(2687, 12) -- cookie
		position:sendMagicEffect(CONST_ME_HITBYFIRE)
	elseif moveitem:getId() == 9115 then -- baking tray (garlic cookie)
		moveitem:transform(9116, 12) -- garlic cookie
		position:sendMagicEffect(CONST_ME_HITBYFIRE)
	elseif moveitem:getId() == 2048 then -- cake (decorated)
		moveitem:transform(6280) -- party cake
		tileitem:remove()
	end
	return true
end
