local event = GlobalEvent("HouseRent")

function event.onStartup()
	local configRentPeriod = configManager.getString(configKeys.HOUSE_RENT_PERIOD)
	local rentPeriod = Game.getRentPeriodHouse(configRentPeriod:lower())
	Game.payHouses(rentPeriod)
	return true
end

event:register()
