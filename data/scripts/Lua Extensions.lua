-----------------------------------------[ Math ]-----------------------------------------
function math.fact(n)
	if (n == 0) then
		return 1
	else
		return n * factorial(n - 1)
	end
end

function math.permutations(n, r)
	return factorial(n) / factorial(n - r)
end

function math.logn(n, base)
	return math.log(n) / math.log(base)
end

-----------------------------------------[ Table ]-----------------------------------------
function table.transfer(dest, src)
	for key, value in pairs(src) do
		dest[key] = value
	end
	return dest
end

function table.contains(array, value)
	for k, v in pairs(array) do
		if v == value then
			return true
		end
	end
	return false
end

function table.execute(self, f, ...)
	for k, v in pairs(self) do
		f(v, ...)
	end
end

function table.indexOf(t, val)
	for k, v in pairs(t) do
		if val == v then
			return k
		end
	end
	return nil
end

function table.lown(t)
	local index = #t
	while t[index] do
		index = index - 1
	end
	return index + 1
end

function table.size(t, deepcount)
	local total = 0
	local function count(t)
		for k, v in pairs(t) do
			total = total + 1
			if type(v) == "table" and deepcount then
				count(v)
			end
		end
	end
	count(t)
	return total
end

function printfmt(fmt, ...)
	print(string.format(fmt, ...))
end

function table._print(t)
	local cache = {}
	printfmt("{ %s (elements: %d)", t, table.size(t))
	local function output(t, tab)
		tab = tab or 1
		local indent = ("    "):rep(tab)
		for k, v in pairs(t) do
			local key = type(k) == "string" and "'%s'" or "%s"
			if type(v) == "table" and not cache[tostring(v)] then
				cache[tostring(v)] = true
				printfmt("%s[".. key .."] = { %s (elements: %d)", indent, k, v, table.size(v))
				output(v, tab + 1)
				printfmt("%s}", indent)
			else
				printfmt("%s[".. key .."] = %s", indent, k, v)
			end
		end
	end
	output(t)
	print("}")
end

do
	-- Iterate in-order first, unordered keys last
	local function iterator(t)
		local indexes = {}
		local stringKeys = {}
		local unorderedKeys = {}
		-- Put all numeric keys at the top
		for k, v in pairs(t) do
			if type(k) == 'number' then
				indexes[#indexes+1] = setmetatable({k}, {nkey = true})
			end
		end

		table.sort(indexes, function(a, b)
			return a[1] < b[1]
		end)

		-- Filter out string keys & unordered keys
		for k, v in pairs(t) do
			if type(k) == 'string' then
				stringKeys[#stringKeys+1] = k
			elseif type(k) ~= 'number' then
				unorderedKeys[#unorderedKeys+1] = k
			end
		end

		-- Sort all string keys in alphabetical order
		table.sort(stringKeys)
		for i = 1, #stringKeys do
			indexes[#indexes+1] = stringKeys[i]
		end

		-- Put all the rest of the unsortable keys at the end
		for i = 1, #unorderedKeys do
			indexes[#indexes+1] = unorderedKeys[i]
		end

		local i = 0
		return function()
			i = i + 1
			local key = indexes[i]
			if type(key) == 'table' then
				local mt = getmetatable(key)
				if mt and mt.nkey then
					return key[1], t[key[1]], true
				end
			end
			return indexes[i], t[indexes[i]], false
		end
	end

	local function serializer(t, indent, buf, cache)
		indent = indent or 1
		buf = buf or {'{'}
		cache = cache or {}
		local tab = string.rep('    ', indent)
		for k, v, ordered in iterator(t) do
			local key = type(k) == 'string' and '"'.. k .. '"' or tostring(k)
			local value = type(v) == 'string' and '"'.. v ..'"' or tostring(v)
			if type(k) == 'table' then
				key = table.concat(serializer(k, indent + 1)) .. string.format('\n%s}', tab)
			end
			if type(v) == 'table' and not cache[v] then
				buf[#buf+1] = string.format('\n%s[%s] = {', tab, key)
				serializer(v, indent + 1, buf, cache)
				buf[#buf+1] = string.format('\n%s}', tab)
				cache[v] = true
			else
				buf[#buf+1] = string.format('\n%s[%s] = %s', tab, key, value)
			end
			buf[#buf+1] = ','
		end
		if buf[#buf] == ',' then
			buf[#buf] = nil -- remove the last comma in each table
		end
		return buf
	end

	function table.serialize(t)
		local buf = serializer(t)
		buf[#buf+1] = '\n}'
		return table.concat(buf)
	end

	function table.print(t)
		print(table.serialize(t))
	end
end

-----------------------------------------[ String ]-----------------------------------------
getmetatable("").__call = function(self)
	local locals = debug.getlocals(3, false)
	for expr in self:gmatch("{([^}]*)}") do
		local var = locals[expr] or _G[expr]
		if var then
			self = self:gsub("({".. expr:escape() .."})", tostring(var))
		else
			local function eval()
				local vars = {}
				for var, expr in self:gmatch("([%a_]*)%s*=%s*([^}]*)") do
					local f = loadstring("return ".. expr)
					locals[var] = f()
					vars[#vars + 1] = var
				end
				return next(vars) and unpack(vars) or nil
			end
			local f = loadstring("return ".. (eval() or expr))
			setfenv(f, table.transfer(getfenv(), locals))
			local result = f() or "<ERROR>"
			self = self:gsub("({".. expr:escape() .."})", tostring(result))
		end
	end
	return self
end

function string.escape(s)
	return s:gsub("([^%w])", "%%%1")
end

function string.lines(str)
	local lines = {}
	for line in str:gmatch("([^\n]*)\n") do
		lines[#lines+1] = line
	end
	local index = 0
	return function()
		index = index + 1
		return lines[index]
	end
end

function string.randomGenerator(length, chars, min, max)
	local chars = chars or "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
	local min = min or 0
	local max = max or 9
	local cache = {}
	local permutations = math.permutations(chars:len() + (max - min ) + 1, length)
	-- BIG MONEY PERFORMANCE SQUEEZE
	local random = math.random
	local sub = string.sub
	local len = string.len
	local concat = table.concat
	--------------------------------
	local function gen()
		if cache:isFull() then
			return print("String generator cache is full.")
		end
		local ret = {}
		local index = 1
		-- Generation
		for i = 1, length do
			local rand = random(2)
			if rand == 1 then
				local char = random(len(chars))
				ret[index] = sub(chars, char, char)
			else
				ret[index] = random(min, max)
			end
			index = index + 1
		end
		local s = concat(ret)
		-- Randomize characters if cache is hit
		while cache[s] do
			local rand = random(2)
			if rand == 1 then
				local char = random(chars:len())
				ret[random(#ret)] = chars:sub(char, char)
			else
				ret[random(#ret)] = random(min, max)
			end
			s = concat(ret)
		end
		cache[s] = true
		return s
	end
	-- Extra useful methods for information about the generator
	debug.setmetatable(gen, {__index = {
		getPermutations = function() return permutations end,
		getLength = function() return length end,
		empty = function()
			cache = setmetatable({["__size"] = 0}, {__index = {
				isFull = function() return permutations == cache.__size end
			}})
		end,
		isFull = function() return permutations == cache.__size end
	}})
	gen:empty()
	return gen
end

-----------------------------------------[ Debug ]-----------------------------------------
function debug.getlocals(level, ordered)
	local ret = {}
	local i = 1
	while true do
		local name, value = debug.getlocal(level, i)
		if name == nil then
			break
		end
		if ordered then
			ret[#ret + 1] = {name = name, value = value or tostring(value)}
		else
			ret[name] = value or tostring(value)
		end
		i = i + 1
	end
	return ret
end

function debug.getupvalues(f)
	local ret = {}
	local i = 1
	while true do
		local name, value = debug.getupvalue(f, i)
		if name == nil then
			break
		end
		ret[#ret + 1] = {name = name, value = value}
		i = i + 1
	end
	return ret
end