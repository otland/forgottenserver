function onAddItem(moveitem, tileitem, position)
	if moveitem:getId() == 2048 and tileitem:getId() == 6279 then
		tileitem:remove()
		moveitem:transform(6280)
		position:sendMagicEffect(CONST_ME_MAGIC_RED)
	elseif moveitem:getId() == 2679 and tileitem:getId() == 6278 then
		tileitem:remove()
		moveitem:transform(6279)
		position:sendMagicEffect(CONST_ME_MAGIC_RED)
	end
end
