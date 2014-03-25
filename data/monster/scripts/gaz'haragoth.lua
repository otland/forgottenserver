function onCreatureSay(cid, speak, talktype, text)
	local DoomText ="Gaz'haragoth begins to channel DEATH AND DOOM into the area! RUN!"
	if (text == DoomText and talktype == 35) then
			addEvent(DOOM, 4000, cid)
			
	end
end

DoomArea = createCombatArea{
{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
{0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
{0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
{1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1},
{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
{0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
{0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
{0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}
}

function DOOM(cid)
	if getCreatureTarget(cid) ~= 0 then
		doSendDistanceShoot(getCreaturePosition(cid), getCreaturePosition(getCreatureTarget(cid)), 47)
		doAreaCombatHealth(cid, 16, getCreaturePosition(getCreatureTarget(cid)), DoomArea, -30000, -30000, 47)
	end
end

local Running = false

function onThink(cid)
local minimo = (getCreatureHealth(cid)/getCreatureMaxHealth(cid))*100
	if (minimo < 12.5 and not Running) then
	Running = true
	addEvent(Uheal, 1000 * 7, cid)
	end
end

function Uheal(cid)
	Running = false
	doCreatureAddHealth(cid, 300000)
end