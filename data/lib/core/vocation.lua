function Vocation.getBase(self)
	local base = self
	while base:getDemotion() do
		base = base:getDemotion()
	end
	return base
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
