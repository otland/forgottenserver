local keywordHandler = KeywordHandler:new()
local npcHandler = NpcHandler:new(keywordHandler)
NpcSystem.parseParameters(npcHandler)

function onCreatureAppear(cid)              npcHandler:onCreatureAppear(cid)            end
function onCreatureDisappear(cid)           npcHandler:onCreatureDisappear(cid)         end
function onCreatureSay(cid, type, msg)      npcHandler:onCreatureSay(cid, type, msg)    end
function onThink()                          npcHandler:onThink()                        end

local voices = {
    {text = "Pst, hey you over there!"},
    {text = "Wanna help me kill some of these dwarves?"}
}

npcHandler:addModule(VoiceModule:new(voices))

local npcMissions = {
    [1] = {name = "Doorman",
        arrayName = {"Doorman", "doorman", "Door man", "Door Man", "door man"},
        storageMission = 43100,
        storageRequired = {nil},
        messageTaskInfo = "I need you to bring back the Dwarf Doorman's key so I can make a copy of it. Oh, and kill him too.",
        messageTaskComplete = "Awesome. Now go take back that mine!",
        --To Kill--
        monsters = {
            [1] = {name = "Dwarven Doorman", amount = 1, storage = 41100},
        },
        --To Collect--
        collectItems = {
            [1] = {itemid = 14344, amount = 1}
        },
        --Rewards--
        exp = 100,
        rewardKey = {
            [1] = {itemid = 2088, amount = 1}
        },
        setStorage = {nil}
    }
}

local npcStory = { -- This story is told when the player first talks to the npc.
    "A group of Dwarves took our mine years ago and we have gotten absolutely no {help} from the kingdom.",
    "If you're feeling up to a mission, I need help killing this dwarf {doorman} so we can gain access to the mine.",
}

local MESSAGES_GREET = { -- Messages are based on the npcStorage for the player. This table handles all greeting interaction between the npc and player.
  --Storage / message
  -- The player gets his storage for the npc set as soon as he talks to the npc
  -- Everytime a player accepts and completes one of the tasks/missions his npcStorage is increased by 1. That is how the code keeps track of how the npc should respond to the player.
    [1] = "Hello again |PLAYERNAME|. Did you get the key from the {doorman} yet?",
    [2] = "Hello again |PLAYERNAME|. Did you get the key from the {doorman} yet?",
    [3] = "Hey there |PLAYERNAME|! How is our mine doing?"
}

local MESSAGES_GOODBYE = { -- This works the same as MESSAGES_GREET. Depending on how many tasks the player has done for the npc. The npc will say different things when he says goodbye to the player.
    [1] = "Thanks for nothing |PLAYERNAME|.",
    [2] = "Thank you for your help, |PLAYERNAME|.",
    [3] = "Goodbye |PLAYERNAME|! Thank you for everything you've done."
}

local arrayGreetings = {"hi", "Hi", "hello", "Hello", "hey", "Hey", "oi", "Oi", "hola", "Hola"}
local arrayFarewell = {"bye", "Bye", "goodbye", "Goodbye", "good-bye", "Good-Bye", "Good-bye", "cya", "adios", "Adios"}

local messageHearStoryAgain = "story"
local messageCheckTasks = {"help", "Help", "task", "Task", "tasks", "Tasks"}
local messageShowTasks = "Here is what I need help with:"

local messageCompletedAllTasks = "Thank you for all you have done. I dont need anymore help now that the mine is back in our name."

local npcStorage = 46000

function onCreatureSay(cid, type, msg)
    local player = Player(cid)
    local playerGreetMessage = MESSAGES_GREET[player:getStorageValue(npcStorage)]
    local playerGoodbyeMessage = MESSAGES_GOODBYE[player:getStorageValue(npcStorage)]
  
    if isInArray(arrayGreetings, msg) and not npcHandler:isFocused(cid) and player:getStorageValue(npcStorage) == -1 then
        npcHandler:addFocus(cid)
        npcHandler:say(npcStory, cid, false, true, 4000)
        player:setStorageValue(npcStorage, 1)
  
    elseif isInArray(arrayGreetings, msg) and not npcHandler:isFocused(cid) then
        npcHandler:addFocus(cid)
        if string.find(playerGreetMessage, "|PLAYERNAME|") then
            local newMsg = string.gsub(playerGreetMessage, "|PLAYERNAME|", player:getName())
            selfSay(newMsg, cid)
        else
            selfSay(playerGreetMessage, cid)
        end
      
    elseif isInArray(arrayFarewell, msg) and npcHandler:isFocused(cid) then
        if string.find(playerGoodbyeMessage, "|PLAYERNAME|") then
            local newMsg = string.gsub(playerGoodbyeMessage, "|PLAYERNAME|", player:getName())
            selfSay(newMsg, cid)
        else
            selfSay(playerGoodbyeMessage, cid)
        end
        npcHandler:releaseFocus(cid)
      
    elseif isInArray(messageCheckTasks, msg) and npcHandler:isFocused(cid) then
        local text = messageShowTasks
        local tmpTable = {}
      
        for i = 1, #npcMissions do
            if player:getStorageValue(npcMissions[i].storageMission) < 2 then
                if npcMissions[i].storageRequired then
                local showTask = true
                    for x = 1, #npcMissions[i].storageRequired do
                        if player:getStorageValue(npcMissions[i].storageRequired[x]) ~= 2 then
                            showTask = false
                            break
                        end
                    end
                  
                    if showTask then
                        tmpTable[#tmpTable + 1] = npcMissions[i].name
                    end
                else
                    tmpTable[#tmpTable + 1] = npcMissions[i].name
                end
            end
        end
      
        if #tmpTable > 0 then
            for i = 1, #tmpTable do
                if i == #tmpTable then
                    text = text.." {"..tmpTable[i].."}"
                else
                    text = text.." {"..tmpTable[i].."},"
                end
            end
            selfSay(text, cid)
        else
            selfSay(messageCompletedAllTasks, cid)
        end
  
    elseif msg and npcHandler:isFocused(cid) then
        local MISSION = nil
        for i = 1, #npcMissions do
            if isInArray(npcMissions[i].arrayName, msg) then
                MISSION = npcMissions[i]
                break
            end
        end
      
        if MISSION == nil then
            return false
        end
      
        if player:getStorageValue(MISSION.storageMission) == 2 then
            selfSay("You have already completed {"..MISSION.name.."}.", cid)
            return false
        end

        local canDoTask = true
        if MISSION.storageRequired then
            for i = 1, #MISSION.storageRequired do
                if player:getStorageValue(MISSION.storageRequired) ~= 2 then
                    canDoTask = false
                    break
                end
            end
        end
      
        if not canDoTask then
            selfSay("You are not ready for {"..MISSION.name.."}.", cid)
        return false
        end
      
        if player:getStorageValue(MISSION.storageMission) == 1 then
            local isTaskDone = true
            if MISSION.monsters then
                for i = 1, #MISSION.monsters do
                    if player:getStorageValue(MISSION.monsters[i].storage) ~= MISSION.monsters[i].amount then
                        isTaskDone = false
                        break
                    end
                end
            end
          
            if MISSION.collectItems then
                for i = 1, #MISSION.collectItems do
                    if player:getItemCount(MISSION.collectItems[i].itemid) < MISSION.collectItems[i].amount then
                        isTaskDone = false
                        break
                    end
                end
            end
          
            if isTaskDone then
                if MISSION.monsters then
                    for i = 1, #MISSION.monsters do
                        player:setStorageValue(MISSION.monsters[i].storage, -1)
                    end
                end
              
                if MISSION.collectItems then
                    for i = 1, #MISSION.collectItems do
                        player:removeItem(MISSION.collectItems[i].itemid, MISSION.collectItems[i].amount)
                    end
                end
              
                if MISSION.exp then
                    player:addExperience(MISSION.exp)
                end
              
                if MISSION.rewardItems then
                    for i = 1, #MISSION.rewardItems do
                        player:addItem(MISSION.rewardItems[i].itemid, MISSION.rewardItems[i].amount, true)
                    end
                end

                if MISSION.rewardKey then
                    for i = 1, #MISSION.rewardKey do
                        local key = player:addItem(MISSION.rewardKey[i].itemid, MISSION.rewardKey[i].amount, true)
                        key:setActionId(2001)
                    end
                end
              
                selfSay(MISSION.messageTaskComplete, cid)
                player:setStorageValue(npcStorage, player:getStorageValue(npcStorage) + 1)
                player:setStorageValue(MISSION.storageMission, 2)
                return false
            end
      
            local text = "["..MISSION.name.."]: "
            if MISSION.monsters then
                for i = 1, #MISSION.monsters do
                    if i == #MISSION.monsters then
                        text = text.."("..player:getStorageValue(MISSION.monsters[i].storage).."/"..MISSION.monsters[i].amount..") "..MISSION.monsters[i].name.." "
                    else
                        text = text.."("..player:getStorageValue(MISSION.monsters[i].storage).."/"..MISSION.monsters[i].amount..") "..MISSION.monsters[i].name..", "
                    end
                end
            end
          
            if MISSION.collectItems then
                for i = 1, #MISSION.collectItems do
                    if i == #MISSION.collectItems then
                        text = text.."("..player:getItemCount(MISSION.collectItems[i].itemid).."/"..MISSION.collectItems[i].amount..") "..ItemType(MISSION.collectItems[i].itemid):getName().." "
                    else
                        text = text.."("..player:getItemCount(MISSION.collectItems[i].itemid).."/"..MISSION.collectItems[i].amount..") "..ItemType(MISSION.collectItems[i].itemid):getName()..", "
                    end
                end
            end
            selfSay(text, cid)
            return false
        end
          
          
        selfSay(MISSION.messageTaskInfo, cid)
        player:setStorageValue(MISSION.storageMission, 1)
        player:setStorageValue(npcStorage, player:getStorageValue(npcStorage) + 1)

        if MISSION.monsters then
            for i = 1, #MISSION.monsters do
                player:setStorageValue(MISSION.monsters[i].storage, 0)
            end
        end
    end
return true
end

npcHandler:addModule(FocusModule:new())
