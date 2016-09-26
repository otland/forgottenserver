Redis.__events = {};

Redis.on = function(eventName, done)
	Redis.__events[eventName] = Redis.__events[eventName] or {};

	local eventsGroup = Redis.__events[eventName];

	if (#eventsGroup == 0) then
		Redis.subscribe(eventName);
	end

	table.insert(eventsGroup, done);
end

Redis.__eventCatcher = function(eventName, value)
	local events = Redis.__events[eventName];

	for key, event in pairs(events) do
		event(value, eventName); 
	end
end

--[[

	Redis.emit("some-channel", "hello from redis");

	Redis.on("some-channel", function(msg, channelName)
		print(msg, channelName);	-- will print msg received from redis and "some-channel"
	end)

]]--