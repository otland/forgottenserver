-- If you don't intend to use quests.xml, you can delete this file.

local function loadXMLQuests()
	local doc = XMLDocument("data/XML/quests.xml")
	if not doc then
		io.write("[Warning - Scripts::XML::loadXMLQuests] Could not load quests.xml.\n")
		return true
	end

	local quests = doc:child("quests")
	for questNode in quests:children() do
		local missions = {}
		for missionNode in questNode:children() do
			local mission = {
				name = missionNode:attribute("name") or "",
				storageId = tonumber(missionNode:attribute("storageid")),
				startValue = tonumber(missionNode:attribute("startvalue")),
				endValue = tonumber(missionNode:attribute("endvalue")),
				ignoreEndValue = tobool(missionNode:attribute("ignoreendvalue")),
				description = missionNode:attribute("description")
			}

			if not mission.description then
				local description = {}
				for missionState in missionNode:children() do
					description[tonumber(missionState:attribute("id"))] =
						missionState:attribute("description") or ""
				end

				mission.description = description
			end

			missions[#missions + 1] = mission
		end

		Game.createQuest(questNode:attribute("name"), {
			storageId = tonumber(questNode:attribute("startstorageid")),
			storageValue = tonumber(questNode:attribute("startstoragevalue")),
			missions = missions
		}):register()
	end
end

loadXMLQuests()
