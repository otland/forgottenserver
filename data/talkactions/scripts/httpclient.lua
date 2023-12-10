local function isJson(contentType)
	return contentType and string.find(contentType:lower(), "json")
end

local function parseJson(bodyData)
	if not bodyData then
		print("parseJson: bodyData is invalid")
		return nil
	end

	local status, data =
						pcall(
						function()
								return json.decode(bodyData)
						end
				)

	if not status then
		print("parseJson: failed to parse json data")
		return nil
	end

	if not data then
		print("parseJson: data is invalid")
		return nil
	end
	
	return data
end

local function httpCallback(param)
	
	if isJson(param["contentType"]) then
		jsonData = parseJson(param["bodyData"])

		if type(jsonData) == "table" then
			print(dump(jsonData))
		else
			print(jsonData)
		end
	else
		if type(param) == "table" then
			print(dump(param))
		else
			print(param)
		end
	end

	print(" I am http callback being called")
end 

local function connect(httpClientRequest)
	local url = "https://www.example.com"
	httpClientRequest:connect(url, httpCallback)
end

local function trace(httpClientRequest)
	local url = "https://www.example.com"
	httpClientRequest:trace(url, httpCallback)
end

local function options(httpClientRequest)
	local url = "http://httpbin.org"
	httpClientRequest:options(url, httpCallback)
end

local function head(httpClientRequest)
	local url = "http://httpbin.org/get"
	httpClientRequest:head(url, httpCallback)
end

local function delete(httpClientRequest)
	local url = "https://httpbin.org/delete"
	local token = "abcdef123456789"
	local data = "I want to delete something"
	local fields = {
		accept = "application/json", 
		authorization = "Bearer " .. token
	}

	httpClientRequest:delete(url, httpCallback, fields, data)
end

local function get(httpClientRequest)
	local url = "https://httpbin.org/get"
	local token = "abcdef123456789"
	local fields = {
		accept = "application/json", 
		authorization = "Bearer " .. token
	}

	httpClientRequest:get(url, httpCallback, fields)
end

local function post(httpClientRequest)
	local url = "https://httpbin.org/post"
	local token = "abcdef123456789"
	local data = "I want to post something"
	local fields = {
		accept = "application/json", 
		authorization = "Bearer " .. token
	}

	httpClientRequest:post(url, httpCallback, fields, data)
end

local function patch(httpClientRequest)
	local url = "https://httpbin.org/patch"
	local token = "abcdef123456789"
	local data = "I want to patch something"
	local fields = {
		accept = "application/json", 
		authorization = "Bearer " .. token
	}

	httpClientRequest:patch(url, httpCallback, fields, data)
end

local function put(httpClientRequest)
	local url = "https://httpbin.org/patch"
	local token = "abcdef123456789"
	local data = "I want to put something"
	local fields = {
		accept = "application/json", 
		authorization = "Bearer " .. token
	}

	httpClientRequest:put(url, httpCallback, fields, data)
end

function onSay(player, words, param)
	local httpClientRequest = HttpClientRequest()
	local httpmethod = param

	if httpmethod == 'connect' then
		connect(httpClientRequest)
	elseif httpmethod == 'trace' then
		trace(httpClientRequest)
	elseif httpmethod == 'options' then
		options(httpClientRequest)
	elseif httpmethod == 'head' then
		head(httpClientRequest)
	elseif httpmethod == 'delete' then
		delete(httpClientRequest)
	elseif httpmethod == 'get' then
		get(httpClientRequest)
	elseif httpmethod == 'post' then
		post(httpClientRequest)
	elseif httpmethod == 'patch' then
		patch(httpClientRequest)
	elseif httpmethod == 'put' then
		put(httpClientRequest)
	else
		player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, string.format("Invalid http method. Available: connect, trace, options, head, delete, get, post, patch and put", words))
		return false
	end

	return true
end
