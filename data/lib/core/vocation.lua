---Returns the base vocation of a promoted vocation, no matter how many promotions it has.
---@param self Vocation
---@return Vocation
function Vocation.getBase(self)
	local base = self
	while base:getDemotion() do
		base = base:getDemotion()
	end
	return base
end

---Returns one vocation down the promotion chain.
---@param self Vocation
---@return Vocation
function Vocation.getFromVocation(self)
	local demotion = self:getDemotion()
	if demotion then
		return demotion
	end
	return self
end

---Retrusn all related vocations in the promotion chain.
---@param self Vocation
---@return table<number, Vocation>
function Vocation.getRelated(self)
	local vocations = {}
	local related = self
	repeat
		vocations[#vocations + 1] = related
		related = related:getPromotion()
	until not related
	return vocations
end
