Redis = {};
Redis.__events = {};

Redis.on = function(eventName, done)
	Redis.__events[eventName] = Redis.__events[eventName] or {};

	local eventsGroup = Redis.__events[eventName];

	if (#eventsGroup == 0) then
		redisSubscribe(eventName);
	end

	table.insert(eventsGroup, function(data, eventName)
		local parsed = JSON:decode(data);

		if (not parsed.__lua) then
			if (parsed.__message) then
				return done(parsed.__message, eventName);
			end

			return done(parsed, eventName);
		end
	end);
end

Redis.emit = function(channel, data)
	if (type(data) == "table") then
		data.__lua = true;
	end

	if (type(data) == "string") then
		local tmp = {
			__message = data,
			__lua = true
		};
		data = tmp;
	end

	local json = JSON:encode(data);

	return redisEmit(channel, json);
end

redisEventCatcher = function(eventName, value)
	local events = Redis.__events[eventName];

	for key, event in pairs(events) do
		event(value, eventName); 
	end
end