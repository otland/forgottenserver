function Item.getActionId(self)
	return self.actionid
end

function Item.getId(self)
	return self.itemid
end

function Item.getSubType(self)
	return self.type
end

function Item.getType(self)
	return ItemType(self.itemid)
end

function Item.getUniqueId(self)
	return self.uid
end

function Item.isContainer(self)
	return false
end

function Item.isCreature(self)
	return false
end

function Item.isPlayer(self)
	return false
end

function Item.isTeleport(self)
	return false
end

function Item.isTile(self)
	return false
end
