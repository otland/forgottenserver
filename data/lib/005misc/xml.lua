local boolTrue = {"1", "y", "Y", "t", "T"}

function tobool(str)
	return str and table.contains(boolTrue, str[0])
end

function XMLNode.children(self)
	local node = self:firstChild()
	return function()
		if not node then
			return nil
		end

		local prevNode = node
		node = node:nextSibling()
		return prevNode
	end
end
