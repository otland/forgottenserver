
function Player:onLook(thing, position, distance)
	return true
end

function Player:onLookInBattleList(creature, distance)
	return true
end

function Player:onLookInTrade(partner, item, distance)
	return true
end

function Player:onLookInShop(itemType, count)
	return true
end

function Player:onMoveItem(item, count, fromPosition, toPosition)
	return true
end

function Player:onMoveCreature(creature, fromPosition, toPosition)
	return true
end
