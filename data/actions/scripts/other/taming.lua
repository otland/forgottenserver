local ACTION_RUN, ACTION_BREAK, ACTION_NONE, ACTION_ALL = 1, 2, 3, 4
local TYPE_MONSTER, TYPE_NPC, TYPE_ACTION, TYPE_UNIQUE = 1, 2, 3, 4

local config = {
        [5907] =        {NAME = 'Bear',                                 ID = 3,         TYPE = TYPE_MONSTER,    CHANCE = 20,    FAIL_MSG = { {1, "The bear ran away."}, {2, "Oh no! The slingshot broke."}, {3, "The bear is trying to hit you with its claws."} }, SUCCESS_MSG = "You have tamed the bear."},
        [13295] =       {NAME = 'Black Sheep',                  ID = 4,         TYPE = TYPE_MONSTER,    CHANCE = 25,    FAIL_MSG = { {1, "The black sheep ran away."}, {2, "Oh no! The reins were torn."}, {3, "The black sheep is trying to run away."} }, SUCCESS_MSG = "You have tamed the sheep."},
        [13293] =       {NAME = 'Midnight Panther',             ID = 5,         TYPE = TYPE_MONSTER,    CHANCE = 40,    FAIL_MSG = { {1, "The panther has escaped."}, {2, "The whip broke."} }, SUCCESS_MSG = "You have tamed the panther."},
        [13298] =       {NAME = 'Terror Bird',                  ID = 2,         TYPE = TYPE_MONSTER,    CHANCE = 15,    FAIL_MSG = { {1, "The bird ran away."}, {3, "The terror bird is pecking you."} }, SUCCESS_MSG = "You have tamed the bird."},
        [13247] =       {NAME = 'Boar',                                 ID = 10,        TYPE = TYPE_MONSTER,    CHANCE = 40,    FAIL_MSG = { {1, "The boar has run away"}, {3, "The boar attacks you."} }, SUCCESS_MSG = "You have tamed the boar."},
        [13305] =       {NAME = 'Crustacea Gigantica',  ID = 7,         TYPE = TYPE_MONSTER,    CHANCE = 40,    FAIL_MSG = { {1, "The crustacea has run away."}, {2, "The crustacea ate the shrimp."} }, SUCCESS_MSG = "You have tamed the crustacea."},
        [13291] =       {NAME = 'Undead Cavebear',              ID = 12,        TYPE = TYPE_MONSTER,    CHANCE = 40,    FAIL_MSG = { {1, "The undead bear has run away."} }, SUCCESS_MSG = "You have tamed the skeleton."},
        [13307] =       {NAME = 'Wailing Widow',                ID = 1,         TYPE = TYPE_MONSTER,    CHANCE = 40,    FAIL_MSG = { {1, "The widow has run away."}, {2, "The widow has eaten the sweet bait."} }, SUCCESS_MSG = "You have tamed the widow."},
        [13292] =       {NAME = 'Tin Lizzard',                  ID = 8,         TYPE = TYPE_NPC,                CHANCE = 40,    FAIL_MSG = { {2, "The key broke inside."} }, SUCCESS_MSG = "You have started the Tin Lizzard!"},
        [13294] =       {NAME = 'Draptor',                      ID = 6,         TYPE = TYPE_MONSTER,    CHANCE = 40,    FAIL_MSG = { {1, "The draptor has run away."}, {3, "The draptor has fled."} }, SUCCESS_MSG = "You have tamed the draptor."},
        [13536] =       {NAME = 'Crystal Wolf',                 ID = 16,        TYPE = TYPE_MONSTER,    CHANCE = 40,    FAIL_MSG = { {1, "The wolf has run away."} }, SUCCESS_MSG = "You have tamed the wolf."},
        [13539] =       {NAME = 'White Deer',                   ID = 18,        TYPE = TYPE_MONSTER,    CHANCE = 40,    FAIL_MSG = { {2, "The cone broke."}, {3, "The deer has fled in fear."} }, SUCCESS_MSG = "You have tamed the deer."},
        [13538] =       {NAME = 'Panda',                                ID = 19,        TYPE = TYPE_MONSTER,    CHANCE = 40,    FAIL_MSG = { {4, "Panda ate the leaves and ran away."} }, SUCCESS_MSG = "You have tamed the panda."},
        [13535] =       {NAME = 'Dromedary',                    ID = 20,        TYPE = TYPE_MONSTER,    CHANCE = 40,    FAIL_MSG = { {1, "Dromedary has run away."} }, SUCCESS_MSG = "You have tamed the dromedary."},
        [13498] =       {NAME = 'Sandstone Scorpion',   ID = 21,        TYPE = TYPE_MONSTER,    CHANCE = 40,    FAIL_MSG = { {1, "The scorpion has vanished."}, {2, "Scorpion broken the sceptre."} }, SUCCESS_MSG = "You have tamed the scorpion"},
        [13537] =       {NAME = 'Donkey',                       ID = 13,        TYPE = TYPE_MONSTER,    CHANCE = 40,    FAIL_MSG = { {1, "The witch has escaped!"} }, SUCCESS_MSG = "You have tamed the mule."},
        [13938] =       {NAME = 'Uniwheel',                     ID = 15,        TYPE = TYPE_NPC,                CHANCE = 40,    FAIL_MSG = { {2, "The oil is having no effect."} }, SUCCESS_MSG = "You have found an Uniwheel."},
        [13508] =       {NAME = 'Slug',                                 ID = 14,        TYPE = TYPE_MONSTER,    CHANCE = 40,    FAIL_MSG = { {1, "The slug has run away."}, {3, "The drug had no effect."} }, SUCCESS_MSG = "You have tamed the slug."},
        [13939] =       {NAME = 'War Horse',                    ID = 23,        TYPE = TYPE_MONSTER,    CHANCE = 15,    FAIL_MSG = { {1, "The horse runs away."}, {2, "The horse ate the oats."} }, SUCCESS_MSG = "You have tamed the horse."},
        [15545] =       {NAME = 'Manta Ray',                    ID = 28,        TYPE = TYPE_MONSTER,    CHANCE = 30,    FAIL_MSG = { {1, "The manta ray fled."}, {3, "The manta ray is trying to escape."} }, SUCCESS_MSG = "You have tamed the manta ray."},
        [15546] =       {NAME = 'Ladybug',                      ID = 27,        TYPE = TYPE_MONSTER,    CHANCE = 30,    FAIL_MSG = { {1, "The ladybug got scared and ran away."}, {3, "The ladybug is trying to nibble."} }, SUCCESS_MSG = "You tame a ladybug."},
        [20138] =       {NAME = 'Water Buffalo',                ID = 35,        TYPE = TYPE_MONSTER,    CHANCE = 30,    FAIL_MSG = { {1, "The water buffalo got scared and ran away."}, {3, "The water buffalo is trying to nibble."} }, SUCCESS_MSG = "You tamed a water buffalo."}
}

local function doFailAction(cid, mount, pos, item, itemEx)
        local action, effect = mount.FAIL_MSG[math.random(1, table.maxn(mount.FAIL_MSG))], CONST_ME_POFF
        if(action[1] == ACTION_RUN) then
                doRemoveCreature(itemEx.uid)
        elseif(action[1] == ACTION_BREAK) then
                effect = CONST_ME_BLOCKHIT
                doRemoveItem(item.uid, 1)
        elseif(action[1] == ACTION_ALL) then
                doRemoveCreature(itemEx.uid)
                doRemoveItem(item.uid, 1)
        end

        doSendMagicEffect(pos, effect)
        doCreatureSay(cid, action[2], TALKTYPE_ORANGE_1)
        return action
end

function onUse(cid, item, fromPosition, itemEx, toPosition)
        local mount = config[item.itemid]
        if(mount == nil or getPlayerMount(cid, mount.ID)) then
                return false
        end

        local rand = math.random(1, 100)
        --Monster Mount
        if(isMonster(itemEx.uid) and not isSummon(itemEx.uid) and mount.TYPE == TYPE_MONSTER) then
                if(mount.NAME == getCreatureName(itemEx.uid)) then
                        if(rand > mount.CHANCE) then
                                doFailAction(cid, mount, toPosition, item, itemEx)
                                return true
                        end

                        doPlayerAddMount(cid, mount.ID)
                        doPlayerSendTextMessage(cid, MESSAGE_STATUS_CONSOLE_ORANGE, mount.SUCCESS_MSG)

                        doCreatureSay(cid, mount.SUCCESS_MSG, TALKTYPE_ORANGE_1)
                        doRemoveCreature(itemEx.uid)

                        doSendMagicEffect(toPosition, CONST_ME_POFF)
                        doRemoveItem(item.uid, 1)
                        return true
                end
        --NPC Mount
        elseif(isNpc(itemEx.uid) and mount.TYPE == TYPE_NPC) then
                if(mount.NAME == getCreatureName(itemEx.uid)) then
                        if(rand > mount.CHANCE) then
                                doFailAction(cid, mount, toPosition, item, itemEx)
                                return true
                        end

                        doPlayerAddMount(cid, mount.ID)
                        doPlayerSendTextMessage(cid, MESSAGE_STATUS_CONSOLE_ORANGE, mount.SUCCESS_MSG)

                        doCreatureSay(cid, mount.SUCCESS_MSG, TALKTYPE_ORANGE_1)

                        doSendMagicEffect(toPosition, CONST_ME_MAGIC_GREEN)
                        doRemoveItem(item.uid, 1)
                        return true
                end
        --Action Mount
        elseif(itemEx.actionid > 0 and mount.TYPE == TYPE_ACTION) then
                if(mount.NAME == itemEx.actionid) then
                        if(rand > mount.CHANCE) then
                                doFailAction(cid, mount, toPosition, item, itemEx)
                                return true
                        end

                        doPlayerAddMount(cid, mount.ID)
                        doPlayerSendTextMessage(cid, MESSAGE_STATUS_CONSOLE_ORANGE, mount.SUCCESS_MSG)

                        doCreatureSay(cid, mount.SUCCESS_MSG, TALKTYPE_ORANGE_1)
                        doSendMagicEffect(toPosition, CONST_ME_MAGIC_GREEN)

                        doRemoveItem(item.uid, 1)
                        return true
                end
        --Unique Mount
        elseif(itemEx.uid <= 65535 and mount.TYPE == TYPE_UNIQUE) then
                if(mount.NAME == itemEx.uid) then
                        if(rand > mount.CHANCE) then
                                doFailAction(cid, mount, toPosition, item, itemEx)
                                return true
                        end

                        doPlayerAddMount(cid, mount.ID)
                        doPlayerSendTextMessage(cid, MESSAGE_STATUS_CONSOLE_ORANGE, mount.SUCCESS_MSG)

                        doCreatureSay(cid, mount.SUCCESS_MSG, TALKTYPE_ORANGE_1)
                        doSendMagicEffect(toPosition, CONST_ME_MAGIC_GREEN)

                        doRemoveItem(item.uid, 1)
                        return true
                end
        end
        return false
end
