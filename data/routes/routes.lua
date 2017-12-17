-- Required to find the json library
package.path = package.path .. ';ext/luajson/lua/?.lua'

-- The Authenticator manages access to HTTP URLs. By default it allows access to all clients.
-- To restrict access you will need to specify what permissions are required for a particular page
-- in the router::register() call e.g. GET = {} means that no permissions are required to perform
-- a GET request on a partiular HTTP resource. It is completely up to the user how these are stored
-- in the table for each object and how the Authenticator manages them.
-- Please make sure that resources are released in Authenticator:onSessionClose() if any resources
-- are allocated during authentication.
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
	--uses the luajson library
	local json = require 'json'
	local stdApi = require('data.routes.scripts.standard_ot_api')
	stdApi.register({router = Router, json = json})
end

registerRoutes()
