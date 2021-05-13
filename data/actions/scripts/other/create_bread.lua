local liquidContainers = {
	1775, -- trough
	2005, -- bucket
	2006, -- vial
	2007, -- bottle
	2008, -- vase
	2009, -- green flask
	2011, -- elven vase
	2012, -- mug
	2013, -- cup
	2014, -- jug
	2015, -- brown flask
	2023, -- amphora
	2031, -- waterskin
	2032, -- bowl
	2033 -- golden mug
}

local millstones = {1381, 1382, 1383, 1384} -- millstone

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if item:getId() == 2692 then -- flour
		if target.type == 1 and table.contains(liquidContainers, target:getId()) then
			item:remove(1)
			player:addItem(2693, 1) -- lump of dough
			target:transform(target:getId(), 0)
			return true
		end
	elseif table.contains(millstones, target:getId()) then
		item:remove(1)
		player:addItem(2692, 1) -- flour
		return true
	end
	return false
end
