--If you don't intend to use quests.xml, you can delete this file.
local globalEvent = GlobalEvent("LoadQuestsXml")

function globalEvent.onStartup()
	local questXml = XMLDocument("data/XML/quests.xml")
	if not questXml then
		io.write("[Warning - GlobalEvent::onStartup] Cannot load quests.xml!")
		return true
	end

	local child = questXml:child("quests")
	local firstChild = child:firstChild()

	while firstChild do
		local missionNode = firstChild:firstChild()
		local missions = {}
		while missionNode do
			local mission = {
				name = tostring(missionNode:attribute("name")),
				storageId = tonumber(missionNode:attribute("storageid")),
				startValue = tonumber(missionNode:attribute("startvalue")),
				endValue = tonumber(missionNode:attribute("endvalue")),
				ignoreEndValue = table.contains({'1', 'y', 't'}, tostring(missionNode:attribute("ignoreendvalue"):sub(1, 1):lower())),
				description = missionNode:attribute("description")
			}

			if not mission.description then
				local missionState = missionNode:firstChild()
				local description = {}
				while missionState do
					description[tonumber(missionState:attribute("id"))] = missionState:attribute("description") or ""
					missionState = missionState:nextSibling()
				end

				mission.description = description
			end

			missions[#missions + 1] = mission
			missionNode = missionNode:nextSibling()
		end

		Game.createQuest(firstChild:attribute("name"), {
			storageId = tonumber(firstChild:attribute("startstorageid")),
			storageValue = tonumber(firstChild:attribute("startstoragevalue")),
			missions = missions
		}):register()

		firstChild = firstChild:nextSibling()
	end
	return true
end

globalEvent:register()
