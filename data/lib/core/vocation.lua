function Vocation.getBase(self)
	local base = self
	while base:getDemotion() do
		base = base:getDemotion()
	end
	return base
end

function Vocation.getFromVocation(self)
	local demotion = self:getDemotion()
	if demotion then
		return demotion
	end
	return self
end

function Vocation.getRelated(self)
	local vocations = {}
	local related = self
	repeat
		vocations[#vocations + 1] = related
		related = related:getPromotion()
	until not related
	return vocations
end
