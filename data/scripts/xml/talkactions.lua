-- If you don't intend to use talkactions.xml, you can delete this file.

local function parseTalkAction(node)
	local talkactionWords = node:attribute("words")
	if not talkactionWords then
		io.write("[Error] Missing attribute words for talkaction.\n")
		return nil
	end

	-- local words = {}
	-- for word in talkactionWords:gmatch("([^;]+)") do
	-- 	table.insert(words, word)
	-- end

	local separator = node:attribute("separator")

	local script = node:attribute("script")
	if not script then
		io.write("[Error] Missing attribute script, check data/talkactions/" .. filename .. "\n")
		return nil
	end

	local scriptFile = "data/talkactions/scripts/" .. script
	dofile(scriptFile)
	if not onSay then
		io.write("[Error] Can not load talkaction script, check " .. scriptFile .. " for a missing onSay callback\n")
		return nil
	end

	local talkaction = TalkAction(talkactionWords:gmatch("([^;]+)"))
	if separator then
		talkaction:separator(separator)
	end
	talkaction.onSay = onSay

	-- let it be garbage collected
	onSay = nil

	return talkaction
end

local function loadXMLTalkActions()
	local doc = XMLDocument("data/talkactions/talkactions.xml")
	if not doc then
		io.write("[Warning - Scripts::XML::loadXMLTalkActions] Could not load talkactions.xml.\n")
		return true
	end

	local talkactions = doc:child("talkactions")
	for talkactionNode in talkactions:children() do
		local talkaction = parseTalkAction(talkactionNode)
		if talkaction then
			talkaction:register()
		end
	end
end

loadXMLTalkActions()
