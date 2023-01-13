-- Advanced NPC System by Jiddo

if not KeywordHandler then
	KeywordNode = {
		keywords = nil,
		callback = nil,
		parameters = nil,
		children = nil,
		parent = nil
	}

	-- Created a new keywordnode with the given keywords, callback function and parameters and without any childNodes.
	function KeywordNode:new(keys, func, param)
		local obj = {}
		obj.keywords = keys
		obj.callback = func
		obj.parameters = param
		obj.children = {}
		setmetatable(obj, self)
		self.__index = self
		return obj
	end

	-- Calls the underlying callback function if it is not nil.
	function KeywordNode:processMessage(cid, message)
		return (not self.callback or self.callback(cid, message, self.keywords, self.parameters, self))
	end

	-- Returns true if message contains all patterns/strings found in keywords.
	function KeywordNode:checkMessage(message)
		if self.keywords.callback then
			return self.keywords.callback(self.keywords, message)
		end

		for _, v in ipairs(self.keywords) do
			if type(v) == 'string' then
				local a, b = string.find(message, v)
				if not a or not b then
					return false
				end
			end
		end
		return true
	end

	-- Returns the parent of this node or nil if no such node exists.
	function KeywordNode:getParent()
		return self.parent
	end

	-- Returns an array of the callback function parameters assosiated with this node.
	function KeywordNode:getParameters()
		return self.parameters
	end

	-- Returns an array of the triggering keywords assosiated with this node.
	function KeywordNode:getKeywords()
		return self.keywords
	end

	-- Adds a childNode to this node. Creates the childNode based on the parameters (k = keywords, c = callback, p = parameters)
	function KeywordNode:addChildKeyword(keywords, callback, parameters)
		local new = KeywordNode:new(keywords, callback, parameters)
		return self:addChildKeywordNode(new)
	end

	-- Adds an alias keyword. Should be used if you have to answer the same thing to several keywords.
	function KeywordNode:addAliasKeyword(keywords)
		if #self.children == 0 then
			print('KeywordNode:addAliasKeyword no previous node found')
			return false
		end

		local prevNode = self.children[#self.children]
		local new = KeywordNode:new(keywords, prevNode.callback, prevNode.parameters, prevNode.condition, prevNode.action)
		for i = 1, #prevNode.children do
			new:addChildKeywordNode(prevNode.children[i])
		end
		return self:addChildKeywordNode(new)
	end

	-- Adds a pre-created childNode to this node. Should be used for example if several nodes should have a common child.
	function KeywordNode:addChildKeywordNode(childNode)
		self.children[#self.children + 1] = childNode
		childNode.parent = self
		return childNode
	end

	KeywordHandler = {
		root = nil,
		lastNode = nil
	}

	-- Creates a new keywordhandler with an empty rootnode.
	function KeywordHandler:new()
		local obj = {}
		obj.root = KeywordNode:new(nil, nil, nil)
		obj.lastNode = {}
		setmetatable(obj, self)
		self.__index = self
		return obj
	end

	-- Resets the lastNode field, and this resetting the current position in the node hierarchy to root.
	function KeywordHandler:reset(cid)
		if self.lastNode[cid] then
			self.lastNode[cid] = nil
		end
	end

	-- Makes sure the correct childNode of lastNode gets a chance to process the message.
	function KeywordHandler:processMessage(cid, message)
		local node = self:getLastNode(cid)
		if not node then
			error('No root node found.')
			return false
		end

		local ret = self:processNodeMessage(node, cid, message)
		if ret then
			return true
		end

		if node:getParent() then
			node = node:getParent() -- Search through the parent.
			local ret = self:processNodeMessage(node, cid, message)
			if ret then
				return true
			end
		end

		if node ~= self:getRoot() then
			node = self:getRoot() -- Search through the root.
			local ret = self:processNodeMessage(node, cid, message)
			if ret then
				return true
			end
		end
		return false
	end

	-- Tries to process the given message using the node parameter's children and calls the node's callback function if found.
	--	Returns the childNode which processed the message or nil if no such node was found.
	function KeywordHandler:processNodeMessage(node, cid, message)
		local messageLower = string.lower(message)
		for i, childNode in pairs(node.children) do
			if childNode:checkMessage(messageLower) then
				local oldLast = self.lastNode[cid]
				self.lastNode[cid] = childNode
				childNode.parent = node -- Make sure node is the parent of childNode (as one node can be parent to several nodes).
				if childNode:processMessage(cid, message) then
					return true
				end
				self.lastNode[cid] = oldLast
			end
		end
		return false
	end

	-- Returns the root keywordnode
	function KeywordHandler:getRoot()
		return self.root
	end

	-- Returns the last processed keywordnode or root if no last node is found.
	function KeywordHandler:getLastNode(cid)
		return self.lastNode[cid] or self:getRoot()
	end

	-- Adds a new keyword to the root keywordnode. Returns the new node.
	function KeywordHandler:addKeyword(keys, callback, parameters)
		return self:getRoot():addChildKeyword(keys, callback, parameters)
	end

	-- Adds an alias keyword for the previous node.
	function KeywordHandler:addAliasKeyword(keys)
		return self:getRoot():addAliasKeyword(keys)
	end

	-- Moves the current position in the keyword hierarchy steps upwards. Steps default value = 1.
	function KeywordHandler:moveUp(cid, steps)
		if not steps or type(steps) ~= "number" then
			steps = 1
		end

		for i = 1, steps do
			if not self.lastNode[cid] then
				return nil
			end
			self.lastNode[cid] = self.lastNode[cid]:getParent() or self:getRoot()
		end
		return self.lastNode[cid]
	end

	function KeywordHandler:addSpellKeyword(keys, parameters)
		-- This function adds a new spell to a character.
		--
		-- @param keys - table of keywords that will trigger the function.
		-- @param parameters - table containing the spellName, price, level and vocation.
		-- @param parameters.npcHandler - NpcHandler object that the spellKeyword will be added to.
		-- @param parameters.spellName - string name of the spell to be added.
		-- @param parameters.price - integer cost of the spell.
		-- @param parameters.level - integer level required to learn the spell.
		-- @param parameters.vocation - integer or table of integers representing the vocation(s) allowed to learn the spell.
		-- Example: keywordHandler:addSpellKeyword({"find person"}, {npcHandler = npcHandler, spellName = "Find Person", price = 80, level = 8, vocation = VOCATION_KNIGHT})

		local keys = keys
		keys.callback = FocusModule.messageMatcherDefault

		local npcHandler, spellName, price, vocationId = parameters.npcHandler, parameters.spellName, parameters.price, parameters.vocation
		local spellKeyword = self:addKeyword(keys, StdModule.say, {npcHandler = npcHandler, text = string.format("Do you want to learn the spell %s for %s?", spellName, price > 0 and price .. " gold" or "free")},
			function(player)
				local baseVocationId = player:getVocation():getBase():getId()
				if type(vocationId) == 'table' then
					-- Using a more efficient way to check if the player meets the vocation requirements
					return table.find(vocationId, baseVocationId) ~= nil
				else
					return vocationId == baseVocationId
				end
			end
		)

		-- It is not necessary to check if the player already has the spell, the check is done in modules.lua
		spellKeyword:addChildKeyword({"yes"}, StdModule.learnSpell, {npcHandler = npcHandler, spellName = spellName, level = parameters.level, price = price})
		spellKeyword:addChildKeyword({"no"}, StdModule.say, {npcHandler = npcHandler, text = "Maybe next time.", reset = true})
	end
end
