-- If you don't intend to use talkactions.xml, you can delete this file.

local function configureTalkActionEvent(node)
	local talkactionWords = node:attribute("words")
	if not talkactionWords then
		io.write("[Error] Missing attribute words for talkaction.\n")
		return nil
	end

	local words = {}
	for word in talkactionWords:gmatch("([^;]+)") do
		table.insert(words, word)
	end

	local separator = node:attribute("separator")

	local script = node:attribute("script")
	if not script then
		io.write("[Error] Missing attribute script, check data/talkactions/scripts/" .. filename .. "\n")
		return nil
	end

	local scriptFile = "data/talkactions/scripts/" .. script

	local env = {}
	for k, v in pairs(_G) do
		env[k] = v
	end

	local f = assert(loadfile(scriptFile, "bt", env))
	f()

	if not env.onSay then
		io.write("[Error] Can not load talkaction script, check " .. scriptFile .. " for a missing onSay callback\n")
		return nil
	end

	local talkaction = TalkAction(table.unpack(words))
	if separator then
		talkaction:separator(separator)
	end
	talkaction.onSay = env.onSay
	return talkaction
end

local function loadXMLTalkActions()
	local doc = XMLDocument("data/talkactions/talkactions.xml")
	if not doc then
		io.write("[Warning - Scripts::XML::loadXMLTalkActions] Could not load talkactions.xml.\n")
		return
	end

	local talkactions = doc:child("talkactions")

    io.write(">> Loading legacy XML talkactions from data/talkactions/talkactions.xml...\n")

	local loaded, start = 0, os.mtime()
	for node in talkactions:children() do
		local talkaction = configureTalkActionEvent(node)
		if talkaction then
			talkaction:register()
			loaded = loaded + 1
		end
	end

    io.write(">> Loaded " .. loaded .. " talkactions in " .. os.mtime() - start .. "ms.\n")
end

loadXMLTalkActions()
