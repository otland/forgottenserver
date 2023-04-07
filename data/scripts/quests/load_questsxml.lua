local questXml = XMLDocument("data/XML/quests.xml")
local child = questXml:child("quests")
local firstChild = child:firstChild()

while firstChild do
	local missionNode = firstChild:firstChild()
	local missions = {}
	while missionNode do
		local mission = {
			name = missionNode:attribute("name"),
			storageId = missionNode:attribute("storageid"),
			startValue = missionNode:attribute("storagevalue"),
			endValue = missionNode:attribute("endvalue"),
			ignoreEndValue = missionNode:attribute("ignoreendvalue"),
			description = missionNode:attribute("description")
		}

		if not mission.description then
			local missionState = missionNode:firstChild()
			local description = {}
			while missionState do
				description[missionState:attribute("id")] = missionState:attribute("description")
				missionState = missionState:nextSibling()
			end

			mission.description = description
		end

		missions[#missions + 1] = mission
		missionNode = missionNode:nextSibling()
	end

	Game.createQuest(firstChild:attribute("name"), {
		storageId = firstChild:attribute("startstorageid"),
		storageValue = firstChild:attribute("startstoragevalue"),
		missions = missions
	}):register()

	firstChild = firstChild:nextSibling()
end
