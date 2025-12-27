function Game:onStartup()
	if Event.onGameStartup then
		Event.onGameStartup()
	end
end

function Game:onShutdown()
	if Event.onGameShutdown then
		Event.onGameShutdown()
	end
end

function Game:onSave()
	if Event.onGameSave then
		Event.onGameSave()
	end
end
