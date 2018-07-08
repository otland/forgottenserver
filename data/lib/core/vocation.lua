function Vocation.getBase(self)
	local v = self
	while v:getDemotion() do
		v = v:getDemotion()
	end
	return v
end
