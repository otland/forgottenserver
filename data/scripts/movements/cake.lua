local cake = MoveEvent()

function cake.onAddItem(moveitem, tileitem, position)
	if moveitem.itemid ~= 2048 then -- has to be a candle
		return true
	end

	moveitem:remove()
	tileitem:transform(6280)
	position:sendMagicEffect(CONST_ME_MAGIC_RED)
	return true
end

cake:type("additem")
cake:id(6279)
cake:register()
