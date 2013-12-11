
function Player:onLook(thing, position, distance)
	return true
end

function Player:onLookInTrade(partner, item, distance)
	return true
end

function Player:onLookInShop(itemType, count)
	return true
end
