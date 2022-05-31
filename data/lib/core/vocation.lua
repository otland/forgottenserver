function Vocation.getBase(self)
	local base = self
	while base:getDemotion() do
		base = base:getDemotion()
	end
	return base
end

function Vocation.getPromotions(self)
	local ids = {}	
	local base = self:getBase()
	
	table.insert(ids, base:getId())
	while base:getPromotion() do
		base = base:getPromotion()
		table.insert(ids, base:getId())
	end	
	return ids
end
