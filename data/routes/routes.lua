local Authenticator = {}

function Authenticator:hasPermissions(session, permissions)
	for _, permission in pairs(permissions) do
		if (session.permissions[permission] == nil) then
			return false
		end
	end

	return true
end

function Authenticator:onSessionClose(session)
end

Router.authenticator = Authenticator;

local function registerRoutes()
	local json = require 'json'
	local stdApi = require('data.routes.scripts.standard_ot_api')
	stdApi.register{router = Router, json = json}
end

registerRoutes()
