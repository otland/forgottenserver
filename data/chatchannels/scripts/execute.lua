local CHAT_ID = 9

function canJoin(player)
	return player:getAccountType() >= ACCOUNT_TYPE_GOD
end

local function InteractiveConsole(env)
	return coroutine.wrap(function(inp, out, err)
		local sandbox = setmetatable({ }, 
			{
				__index = function(t, index)
					return rawget(t, index) or env[index] or _G[index]
				end
			}
		)
		sandbox._G = sandbox
		sandbox.os = {
			time = function() return os.time() end
		}
		sandbox.error = err	
		sandbox.print = function(...)
			local r = {}
			for _, v in ipairs({...}) do
				table.insert(r, tostring(v))
			end
			local s = table.concat(r, string.rep(' ', 4))
			return out(#s > 0 and s or 'nil')
		end		

		local chunks = {}
		local level = 1

		while true do
			table.insert(chunks, coroutine.yield())

			local func, e = loadstring(table.concat(chunks, ' '), 'console')
			if func then			
				setfenv(func, sandbox)
				inp(string.rep('>', level) .. ' ' .. chunks[#chunks])
				local s, e = pcall(func)
				if not s then
					err(e)
				end

				chunks = { }
				level = 1
			else
				if not e:find('near \'<eof>\'$') then
					inp(string.rep('>', level) .. ' ' .. chunks[#chunks])
					chunks = { }
					level = 1
					err(e)
				else					
					inp(string.rep('>', level) .. ' ' .. chunks[#chunks])
					level = 2
				end
			end
		end
	end)
end

local consoles = {}
 
function onSpeak(player, type, message)
    if player:getAccountType() < ACCOUNT_TYPE_GOD then
        return false
    end
 
    local pid = player:getId()
   
    local console = consoles[pid]
    if not console then
        console = InteractiveConsole {
            player = player,
            cid = pid,
        }
 
        console(
            function(inp)
                local player = Player(pid)
                if player then
                    player:sendChannelMessage(player:getName(), inp, TALKTYPE_CHANNEL_Y, CHAT_ID)
                end
            end,
           
            function(s)
                local player = Player(pid)
                if player then
                    player:sendChannelMessage(nil, s, TALKTYPE_CHANNEL_O, CHAT_ID)
                end
            end,
 
            function(e)
                local player = Player(pid)
                if player then
                    player:sendChannelMessage(nil, e, TALKTYPE_CHANNEL_R1, CHAT_ID)
                end
            end
        )
 
        consoles[pid] = console
    end
 
    console(message)
    return false
end