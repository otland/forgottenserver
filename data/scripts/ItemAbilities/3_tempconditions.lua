g_tempConditions = g_tempConditions or {}

TempCondition = {}

setmetatable(TempCondition, {
	__call = function(self, def)
		local obj = {
			data = {},
			type = def.type,
			id = def.id,
			value = def.value or 0,
			player = def.player,
			active = false,
			callback = def.callback
		}
		obj = setmetatable(obj, {__index = TempCondition})
		local cid = obj.player:getId()
		if not g_tempConditions[obj.player:getId()] then
			g_tempConditions[cid] = {}
		end
		g_tempConditions[cid][#g_tempConditions[cid] + 1] = obj
		return obj
	end
})

function TempCondition.setActivity(self, active)
	self.active = active
end

function TempCondition.getValue(self)
	return self.value
end

function TempCondition.setValue(self, value)
	if value == 0 then
		self.value = 0
		self:clear()
		return
	end
	if self.active then
		self:clear()
	end
	self.value = value
	if self.active then
		self:execute()
	end
end

function TempCondition.changeValue(self, delta)
	self:setValue(self:getValue() + delta)
end

function TempCondition.execute(self)
	self.callback(self.player, self.value)
	self:setActivity(true)
end

function TempCondition.clear(self)
	self.callback(self.player, -self.value)
	self:setActivity(false)
end

function TempCondition.reset(self)
	self:clear()
	self:execute()
end

function TempCondition.delete(self)
	local conditions = g_tempConditions[self.player:getId()]
	if conditions then
		for k, condition in pairs(conditions) do
			if condition == self then
				condition:clear()
				g_tempConditions[self.player:getId()][k] = nil
			end
		end
	end
end

----------------------------------------------------------------------------------

function Player.getConditionValue(self, conditionType, id)
	local cid = self:getId()
	if g_tempConditions[cid] then
		local sum = 0
		for i, condition in pairs(g_tempConditions[cid]) do
			if condition.active then
				sum = sum + condition.value
			end
		end
		return sum
	end
	return 0
end

function Player.getTempCondition(self, conditionType, id)
	local cid = self:getId()
	if g_tempConditions[cid] then
		for i, condition in pairs(g_tempConditions[cid]) do
			if condition.type == conditionType then
				if id and condition.id == id or not id then
					return condition
				end
			end
		end
	end
	return nil
end

function Player.addTempCondition(self, tempCondition)
	tempCondition:execute()
end

function Player.removeTempCondition(self, tempCondition)
	tempCondition:clear()
end

function Player.deleteTempConditions(self)
	local conditions = g_tempConditions[self:getId()]
	if conditions then
		for k, condition in pairs(conditions) do
			condition:delete()
		end
	end
end

-----------------------------------------------------------

local login = CreatureEvent('tmp_c_login')
login:type('login')

function login.onLogin(player)
	player:registerEvent('tmp_c_logout')
	player:registerEvent('tmp_c_death')
	return true
end

login:register()

local logout = CreatureEvent('tmp_c_logout')
logout:type('logout')

function logout.onLogout(player)
	player:deleteTempConditions()
	return true
end

logout:register()

local death = CreatureEvent('tmp_c_death')
death:type('death')

function death.onDeath(player)
	player:deleteTempConditions()
	return true
end

death:register()