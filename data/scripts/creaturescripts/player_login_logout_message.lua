local loginMessage = CreatureEvent("loginMessage")

function loginMessage.onLogin(player)
    print(player:getName(), " has logged in.")
end

loginMessage:register()

local logoutMessage = CreatureEvent("logoutMessage")

function logoutMessage.onLogout(player)
    print(player:getName(), " has logged out.")
end

logoutMessage:register()
