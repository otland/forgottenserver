---@alias os.mtime fun(): number

---@alias table.create fun(arrayLength: number, keyLength: number): table
---@alias table.pack fun(...): table

---@class UserClientVersion
---@field version uint16 The version number of the client
---@field os OperatingSystem The operating system of the user 

---@class SpellsTable
---@field name string The name of the spell
---@field words string The words used to cast the spell
---@field level uint32 The level required to cast the spell
---@field mlevel uint32 The magic level required to cast the spell
---@field mana uint32 The mana cost of the spell
---@field manaPercent uint32 The mana percentage required to cast the spell
---@field params boolean Whether the spell has parameters or not

---@class MountInfoTable
---@field name string The name of the mount
---@field speed int32 The speed of the mount
---@field clientId uint16 The client ID of the mount
---@field id uint16 The ID of the mount
---@field premium boolean Whether the mount is requires a premium account or not

---@class GameClientVersion
---@field min number -- Mininum version of the client allowed (Current 1310)
---@field max number -- Maximum version of the client allowed (Current 1311)
---@field string string -- String representation of the version (Current "13.10")

---@class rawgetmetatable
---@field __index fun(self: table, key: any): any
rawgetmetatable = {}

---@class bit
---@source ../src/luascript.cpp:4117
---@field band fun(a: number, b: number): number
---@source ../src/luascript.cpp:4117
---@field bor fun(a: number, b: number): number
---@source ../src/luascript.cpp:4117
---@field bxor fun(a: number, b: number): number
---@source ../src/luascript.cpp:4117
---@field lshift fun(a: number, b: number): number
---@source ../src/luascript.cpp:4117
---@field rshift fun(a: number, b: number): number
bit = {}

---@alias Outfit_t table<string, integer>
---@alias MagicEffect_t table<string, integer>

---@class DBInsert
---@field __gc fun(self: DBInsert)
---@source ../src/luascript.cpp:4463
---@field addRow fun(self: DBInsert, row: string): boolean|nil
---@source ../src/luascript.cpp:4475
---@field execute fun(self: DBInsert): boolean|nil
DBInsert = {}

---@class DBTransaction
---@field __eq fun(self: DBTransaction, other: DBTransaction): boolean
---@field __gc fun(self: DBTransaction)
---@source ../src/luascript.cpp:4508
---@field begin fun(self: DBTransaction): boolean|nil
---@source ../src/luascript.cpp:4518
---@field commit fun(self: DBTransaction): boolean|nil
---@field rollback fun(self: DBTransaction)
DBTransaction = {}

---@class db
---@source ../src/luascript.cpp:4199
---@field query fun(query: string): any
---@source ../src/luascript.cpp:4206
---@field asyncQuery fun(query: string): boolean
---@source ../src/luascript.cpp:4237
---@field storeQuery fun(query: string): any
---@source ../src/luascript.cpp:4248
---@field asyncStoreQuery fun(query: string): boolean
---@source ../src/luascript.cpp:4283
---@field escapeString fun(value: string): string -- Escapes a string for query
---@source ../src/luascript.cpp:4290
---@field escapeBlob fun(value: string, length: uint32): string -- Escapes a binary stream for query
---@source ../src/luascript.cpp:4298
---@field lastInsertId fun(): uint64 -- Returns the last inserted row's id
db = {}

---@class result
---@source ../src/luascript.cpp:4369
---@field free fun(resultId: uint32)
---@source ../src/luascript.cpp:4357
---@field next fun(resultId: uint32): number
---@source ../src/luascript.cpp:4317
---@field getNumber fun(resultId: uint32, column: string): number
---@source ../src/luascript.cpp:4330
---@field getString fun(resultId: uint32, column: string): string
---@field getBoolean fun(resultId: uint32, column: string): boolean -- No source in luascript?
---@source ../src/luascript.cpp:4343
---@field getStream fun(resultId: uint32, column: string): string
result = {}

---@class configManager
---@source ../src/luascript.cpp:4182
---@field getBoolean fun(key: string): boolean
---@source ../src/luascript.cpp:4176
---@field getNumber fun(key: string): number
---@source ../src/luascript.cpp:4170
---@field getString fun(key: string): string
configManager = {}
-- These functions do not exist in luascript.cpp
-- -@field setBoolean fun(key: string, value: boolean): boolean
-- -@field setNumber fun(key: string, value: number): boolean
-- -@field setString fun(key: string, value: string): boolean
-- -@field remove fun(key: string): boolean
-- -@field getKeys fun(): table


---@class Game
---@source ../src/luascript.cpp:4541
---@field getSpectators fun(position: Position, multifloor?: boolean, onlyPlayers?: boolean, minRangeX?: int32, minRangeY?: int32, maxRangeX?: int32, maxRangeY?: int32): table<number, Creature> -- Returns a table of all creatures (spectators) in a defined range. Recommended whenever possible not define a min/max range, as it will default to maxViewPortX/maxViewPortY which is cached
---@source ../src/luascript.cpp:4567
---@field getPlayers fun(): table<number, Player> 
---@source ../src/luascript.cpp:4581
---@field getNpcs fun(): table<number, Npc>
---@source ../src/luascript.cpp:4595
---@field getMonsters fun(): table<number, Monster>
---@source ../src/luascript.cpp:4609
---@field loadMap fun(path: string)
---@source ../src/luascript.cpp:4624
---@field getExperienceStage fun(level: uint32): number
---@source ../src/luascript.cpp:4632
---@field getExperienceForLevel fun(level: uint32): uint64
---@source ../src/luascript.cpp:4644
---@field getMonsterCount fun(): number
---@source ../src/luascript.cpp:4651
---@field getPlayerCount fun(): number
---@source ../src/luascript.cpp:4658
---@field getNpcCount fun(): number
---@source ../src/luascript.cpp:4665
---@field getMonsterTypes fun(): table<number, MonsterType> -- Table of all monster types
---@source ../src/luascript.cpp:4679
---@field getBestiary fun(): table<string, table<number, MonsterType>> -- Table of all monsters in the bestiary. Sorted by {className = {index = monsterType, ...}, ...}
---@source ../src/luascript.cpp:4703
---@field getCurrencyItems fun(): table<number, ItemType>
---@source ../src/luascript.cpp:4719
---@field getItemTypeByClientId fun(clientId: uint16): ItemType
---@source ../src/luascript.cpp:4734
---@field getMountIdByLookType fun(lookType: uint16): number
---@source ../src/luascript.cpp:4750
---@field getTowns fun(): table<number, Town>
---@source ../src/luascript.cpp:4764
---@field getHouses fun(): table<number, House>
---@source ../src/luascript.cpp:4779
---@field getOutfits fun(sex: PlayerSexes): table<number, Outfit>
---@source ../src/luascript.cpp:4805
---@field getMounts fun(): table<number, MountInfoTable>
---@source ../src/luascript.cpp:4827
---@field getVocations fun(): table<number, Vocation>
---@source ../src/luascript.cpp:4843
---@field getGameState fun(): GameStates
---@source ../src/luascript.cpp:4850
---@field setGameState fun(state: GameStates): boolean -- Will always return true
---@source ../src/luascript.cpp:4859
---@field getWorldType fun(): WorldTypes
---@source ../src/luascript.cpp:4866
---@field setWorldType fun(type: WorldTypes): boolean -- Will always return true
---@source ../src/luascript.cpp:4875
---@field getReturnMessage fun(value: ReturnValues): string
---@source ../src/luascript.cpp:4883
---@field getItemAttributeByName fun(attribute: string): ItemAttributes
---@source ../src/luascript.cpp:4890
---@field createItem fun(itemInfo: uint16|string, count?: uint16, position?: Position): Item
---@source ../src/luascript.cpp:4936
---@field createContainer fun(containerInfo: uint16|string, size: number, position?: Position): Container
---@source ../src/luascript.cpp:4977
---@field createMonster fun(name: string, position: Position, extended?: boolean, force?: boolean, magicEffect?: MagicEffectConsts): Monster
---@source ../src/luascript.cpp:5005
---@field createNpc fun(name: string, position: Position, extended?: boolean, force?: boolean, magicEffect?: MagicEffectConsts): Npc
---@source ../src/luascript.cpp:5028
---@field createTile fun(position: Position, isDynamic?: boolean): Tile -- Position can either be given as three params x, y, z or as a Position, Position(x,y,z) 
---@source ../src/luascript.cpp:5060
---@field createMonsterType fun(name: string): MonsterType
---@source ../src/luascript.cpp:5097
---@field startEvent fun(eventName: string): boolean
---@source ../src/luascript.cpp:5111
---@field getClientVersion fun(): GameClientVersion
---@source ../src/luascript.cpp:5121
---@field reload fun(reloadType: number): boolean
Game = {}

---@class Variant
---@source ../src/luascript.cpp:5138
---@field create fun(variantInfo: uint32|Thing|string|Position): Variant
---@source ../src/luascript.cpp:5157
---@field getNumber fun(self: Variant): uint32
---@source ../src/luascript.cpp:5169
---@field getString fun(self: Variant): string
---@source ../src/luascript.cpp:5181
---@field getPosition fun(self: Variant): Position
Variant = {}

---@class Position
---@source ../src/luascript.cpp:5196
---@field create fun(positionX: uint16, positionY: uint16, positionZ: uint8, stackPos?: uint32): Position -- Can be passed x, y, z, stackPos, or another Position
---@source ../src/luascript.cpp:5220
---@field isSightClear fun(self: Position, positionEx: Position, sameFloor?: boolean): boolean
---@source ../src/luascript.cpp:5230
---@field sendMagicEffect fun(self: Position, effectType: MagicEffectConsts, creature?: Creature): boolean
---@source ../src/luascript.cpp:5258
---@field sendDistanceEffect fun(self: Position, targetPos: Position, effectType: MagicEffectConsts, player?: Player): boolean
Position = {}

---@class Tile
---@field create fun(): Tile
---@field __eq fun(self: Tile, other: Tile): boolean
---@field remove fun(self: Tile): boolean
---@field getPosition fun(self: Tile): Position
---@field getGround fun(self: Tile): Item
---@field getThing fun(self: Tile, index: number): Creature|Item|nil
---@field getThingCount fun(self: Tile): number
---@field getTopVisibleThing fun(self: Tile): Creature|Item|nil
---@field getTopTopItem fun(self: Tile): Item
---@field getTopDownItem fun(self: Tile): Item
---@field getFieldItem fun(self: Tile): Item
---@field getItemById fun(self: Tile, itemId: number): Item
---@field getItemByType fun(self: Tile, itemType: number): Item
---@field getItemByTopOrder fun(self: Tile, order: number): Item
---@field getItemCountById fun(self: Tile, itemId: number): number
---@field getBottomCreature fun(self: Tile): Creature
---@field getTopCreature fun(self: Tile): Creature
---@field getBottomVisibleCreature fun(self: Tile): Creature
---@field getTopVisibleCreature fun(self: Tile): Creature
---@field getItems fun(self: Tile): table
---@field getItemCount fun(self: Tile): number
---@field getDownItemCount fun(self: Tile): number
---@field getTopItemCount fun(self: Tile): number
---@field getCreatures fun(self: Tile): table
---@field getCreatureCount fun(self: Tile): number
---@field getThingIndex fun(self: Tile, thing: Creature|Item): number
---@field hasProperty fun(self: Tile, property: number): boolean
---@field hasFlag fun(self: Tile, flag: number): boolean
---@field queryAdd fun(self: Tile, thing: Creature|Item, index: number): boolean
---@field addItem fun(self: Tile, item: Item, countOrSubtype?: number, flags?: number): boolean
---@field addItemEx fun(self: Tile, item: Item, index: number): boolean
---@field getHouse fun(self: Tile): House
Tile = {}

---@class NetworkMessage
---@field create fun(): NetworkMessage
---@field __eq fun(self: NetworkMessage, other: NetworkMessage): boolean
---@field __gc fun(self: NetworkMessage)
---@field delete fun(self: NetworkMessage)
---@field getByte fun(self: NetworkMessage): number
---@field getU16 fun(self: NetworkMessage): number
---@field getU32 fun(self: NetworkMessage): number
---@field getU64 fun(self: NetworkMessage): number
---@field getString fun(self: NetworkMessage): string
---@field getPosition fun(self: NetworkMessage): Position
---@field addByte fun(self: NetworkMessage, byte: number)
---@field addU16 fun(self: NetworkMessage, u16: number)
---@field addU32 fun(self: NetworkMessage, u32: number)
---@field addU64 fun(self: NetworkMessage, u64: number)
---@field addString fun(self: NetworkMessage, string: string)
---@field addPosition fun(self: NetworkMessage, position: Position)
---@field addDouble fun(self: NetworkMessage, double: number)
---@field addItem fun(self: NetworkMessage, item: Item)
---@field addItemId fun(self: NetworkMessage, itemId: number)
---@field reset fun(self: NetworkMessage)
---@field seek fun(self: NetworkMessage, position: number)
---@field tell fun(self: NetworkMessage): number
---@field len fun(self: NetworkMessage): number
---@field skipBytes fun(self: NetworkMessage, count: number)
---@field sendToPlayer fun(self: NetworkMessage, player: Player)
NetworkMessage = {}

---@class ModalWindow
---@field create fun(): ModalWindow
---@field __eq fun(self: ModalWindow, other: ModalWindow): boolean
---@field __gc fun(self: ModalWindow)
---@field delete fun(self: ModalWindow)
---@field getId fun(self: ModalWindow): number
---@field getTitle fun(self: ModalWindow): string
---@field getMessage fun(self: ModalWindow): string
---@field setTitle fun(self: ModalWindow, title: string)
---@field setMessage fun(self: ModalWindow, message: string)
---@field getButtonCount fun(self: ModalWindow): number
---@field getChoiceCount fun(self: ModalWindow): number
---@field addButton fun(self: ModalWindow, buttonId: number, buttonText: string)
---@field addChoice fun(self: ModalWindow, choiceId: number, choiceText: string)
---@field getDefaultEnterButton fun(self: ModalWindow): number
---@field setDefaultEnterButton fun(self: ModalWindow, buttonId: number)
---@field getDefaultEscapeButton fun(self: ModalWindow): number
---@field setDefaultEscapeButton fun(self: ModalWindow, buttonId: number)
---@field hasPriority fun(self: ModalWindow): boolean
---@field setPriority fun(self: ModalWindow, priority: boolean)
---@field sendToPlayer fun(self: ModalWindow, player: Player)
ModalWindow = {}

---@class Item : Thing
---@field create fun(): Item
---@field __eq fun(self: Item, other: Item): boolean
---@field isItem fun(self: Item): boolean
---@field getParent fun(self: Item): Item
---@field getTopParent fun(self: Item): Item
---@field getId fun(self: Item): number
---@field clone fun(self: Item): Item
---@field split fun(self: Item): Item
---@field remove fun(self: Item, count?: number): boolean
---@field getUniqueId fun(self: Item): number
---@field getActionId fun(self: Item): number
---@field setActionId fun(self: Item, actionId: number)
---@field getCount fun(self: Item): number
---@field getCharges fun(self: Item): number
---@field getFluidType fun(self: Item): number
---@field getWeight fun(self: Item): number
---@field getWorth fun(self: Item): number
---@field getSubType fun(self: Item): number
---@field getName fun(self: Item): string
---@field getPluralName fun(self: Item): string
---@field getArticle fun(self: Item): string
---@field getPosition fun(self: Item): Position
---@field getTile fun(self: Item): Tile
---@field hasAttribute fun(self: Item, attribute: number|string): boolean
---@field getAttribute fun(self: Item, attribute: number|string): any
---@field setAttribute fun(self: Item, attribute: number|string, value: any)
---@field removeAttribute fun(self: Item, attribute: number)
---@field getCustomAttribute fun(self: Item, key: string): any
---@field setCustomAttribute fun(self: Item, key: string, value: any)
---@field removeCustomAttribute fun(self: Item, key: string)
---@field moveTo fun(self: Item, destination: Position|Thing)
---@field transform fun(self: Item, newItem: number|string, count?: number)
---@field decay fun(self: Item)
---@field getSpecialDescription fun(self: Item): string
---@field hasProperty fun(self: Item, property: number): boolean
---@field isLoadedFromMap fun(self: Item): boolean
---@field setStoreItem fun(self: Item, store: boolean)
---@field isStoreItem fun(self: Item): boolean
---@field setReflect fun(self: Item, reflect: number)
---@field getReflect fun(self: Item): number
---@field setBoostPercent fun(self: Item, percent: number)
---@field getBoostPercent fun(self: Item): number
---@field addItem fun(self: Item, id: number, subtype?: number): boolean
Item = {}

---@class Container : Item
---@field getSize fun(self: Container): number
---@field getCapacity fun(self: Container): number
---@field getEmptySlots fun(self: Container): number
---@field getItems fun(self: Container): table
---@field getItemCountById fun(self: Container, itemId: number, subType: number): number
---@field getItem fun(self: Container, index: number): Item
---@field hasItem fun(self: Container, itemId: number, subType: number): boolean
---@field addItem fun(self: Container, itemId: number, count: number): Item
---@field addItemEx fun(self: Container, item: Item): boolean
---@field getCorpseOwner fun(self: Container): number
Container = {}

---@class Teleport : Item
---@field getDestination fun(self: Teleport): Position
---@field setDestination fun(self: Teleport, position: Position)
Teleport = {}

---@class Podium : Item
---@field getOutfit fun(self: Podium): Outfit
---@field setOutfit fun(self: Podium, outfit: Outfit)
---@field hasFlag fun(self: Podium, flag: number): boolean
---@field setFlag fun(self: Podium, flag: number)
---@field getDirection fun(self: Podium): number
---@field setDirection fun(self: Podium, direction: number)
Podium = {}

---@class Creature : Thing
---@field create fun(): Creature
---@field __eq fun(self: Creature, other: Creature): boolean
---@field getEvents fun(self: Creature): table
---@field registerEvent fun(self: Creature, eventName: string, callback: function)
---@field unregisterEvent fun(self: Creature, eventName: string)
---@field isRemoved fun(self: Creature): boolean
---@field isCreature fun(self: Creature): boolean
---@field isInGhostMode fun(self: Creature): boolean
---@field isHealthHidden fun(self: Creature): boolean
---@field isMovementBlocked fun(self: Creature): boolean
---@field isImmune fun(self: Creature): boolean
---@field canSee fun(self: Creature, position: Position): boolean
---@field canSeeCreature fun(self: Creature, creature: Creature): boolean
---@field canSeeGhostMode fun(self: Creature): boolean
---@field canSeeInvisibility fun(self: Creature): boolean
---@field getParent fun(self: Creature): Creature
---@field getId fun(self: Creature): number
---@field getName fun(self: Creature): string
---@field getTarget fun(self: Creature): Creature
---@field setTarget fun(self: Creature, target: Creature)
---@field getFollowCreature fun(self: Creature): Creature
---@field setFollowCreature fun(self: Creature, creature: Creature)
---@field getMaster fun(self: Creature): Creature
---@field setMaster fun(self: Creature, master: Creature)
---@field getLight fun(self: Creature): table
---@field setLight fun(self: Creature, lightInfo: table)
---@field getSpeed fun(self: Creature): number
---@field getBaseSpeed fun(self: Creature): number
---@field changeSpeed fun(self: Creature, speedDelta: number)
---@field setDropLoot fun(self: Creature, dropLoot: boolean)
---@field setSkillLoss fun(self: Creature, skillLoss: boolean)
---@field getPosition fun(self: Creature): Position
---@field getTile fun(self: Creature): Tile
---@field getDirection fun(self: Creature): number
---@field setDirection fun(self: Creature, direction: number)
---@field getHealth fun(self: Creature): number
---@field setHealth fun(self: Creature, health: number)
---@field addHealth fun(self: Creature, healthDelta: number)
---@field getMaxHealth fun(self: Creature): number
---@field setMaxHealth fun(self: Creature, maxHealth: number)
---@field setHiddenHealth fun(self: Creature, hidden: boolean)
---@field setMovementBlocked fun(self: Creature, blocked: boolean)
---@field getSkull fun(self: Creature): number
---@field setSkull fun(self: Creature, skullType: number)
---@field getOutfit fun(self: Creature): Outfit
---@field setOutfit fun(self: Creature, outfit: Outfit)
---@field getCondition fun(self: Creature, conditionType: number): Condition
---@field addCondition fun(self: Creature, condition: Condition)
---@field removeCondition fun(self: Creature, conditionType: number)
---@field hasCondition fun(self: Creature, conditionType: number): boolean
---@field remove fun(self: Creature)
---@field teleportTo fun(self: Creature, position: Position, createEffect?: boolean)
---@field say fun(self: Creature, text: string, type?: number, ghost?: boolean, receiver?: Creature, channelId?: number)
---@field getDamageMap fun(self: Creature): table
---@field getSummons fun(self: Creature): table
---@field getDescription fun(self: Creature): string
---@field getPathTo fun(self: Creature, position: Position): table
---@field move fun(self: Creature, direction: number)
---@field getZone fun(self: Creature): number
---@field hasIcon fun(self: Creature): boolean
---@field setIcon fun(self: Creature, iconId: number)
---@field getIcon fun(self: Creature): number
---@field removeIcon fun(self: Creature)
---@field getStorageValue fun(self: Creature, key: number): any
---@field setStorageValue fun(self: Creature, key: number, value: any)
Creature = {}

---@class Player : Creature
---@field create fun(): Player
---@field __eq fun(self: Player, other: Player): boolean
---@source ../src/luascript.cpp:8948
---@field isPlayer fun(self: Player): boolean
---@source ../src/luascript.cpp:8955
---@field getGuid fun(self: Player): uint32 -- Unique identifier for the player, used in the database
---@source ../src/luascript.cpp:8967
---@field getIp fun(self: Player): string -- IP address of the player as a string
---@source ../src/luascript.cpp:8979
---@field getAccountId fun(self: Player): uint32 -- Account ID of the player, used in the database
---@source ../src/luascript.cpp:8991
---@field getLastLoginSaved fun(self: Player): number -- Returns timestamp of the last login saved in the database
---@source ../src/luascript.cpp:9003
---@field getLastLogout fun(self: Player): number -- Returns timestamp of the last logout saved in the database
---@source ../src/luascript.cpp:9015
---@field getAccountType fun(self: Player): AccountTypes
---@source ../src/luascript.cpp:9027
---@field setAccountType fun(self: Player, accountType: AccountTypes)
---@source ../src/luascript.cpp:9041
---@field getCapacity fun(self: Player): uint32
---@source ../src/luascript.cpp:9053
---@field setCapacity fun(self: Player, capacity: uint32)
---@source ../src/luascript.cpp:9067
---@field getFreeCapacity fun(self: Player): uint32
---@source ../src/luascript.cpp:9079
---@field getDepotChest fun(self: Player, depotId: uint32, autoCreate?: boolean): Item
---@source ../src/luascript.cpp:9100
---@field getInbox fun(self: Player): Item
---@source ../src/luascript.cpp:9119
---@field getSkullTime fun(self: Player): int64
---@source ../src/luascript.cpp:9131
---@field setSkullTime fun(self: Player, time: int64)
---@source ../src/luascript.cpp:9144
---@field getDeathPenalty fun(self: Player): double
---@source ../src/luascript.cpp:9156
---@field getExperience fun(self: Player): uint64
---@source ../src/luascript.cpp:9168
---@field addExperience fun(self: Player, amount: uint64): boolean? -- Will return true if player, nil if there is no player 
---@source ../src/luascript.cpp:9183
---@field removeExperience fun(self: Player, amount: uint64): boolean? --- Will return true if player, nil if there is no player
---@source ../src/luascript.cpp:9198
---@field getLevel fun(self: Player): uint32
---@source ../src/luascript.cpp:9210
---@field getLevelPercent fun(self: Player): uint8
---@source ../src/luascript.cpp:9222
---@field getMagicLevel fun(self: Player): uint32
---@source ../src/luascript.cpp:9234
---@field getMagicLevelPercent fun(self: Player): uint16
---@source ../src/luascript.cpp:9246
---@field getBaseMagicLevel fun(self: Player): uint32
---@source ../src/luascript.cpp:9258
---@field getMana fun(self: Player): uint32
---@source ../src/luascript.cpp:9270
---@field addMana fun(self: Player, amount: int32, animationOnLoss?: boolean)
---@source ../src/luascript.cpp:9293
---@field getMaxMana fun(self: Player): uint32
---@source ../src/luascript.cpp:9305
---@field setMaxMana fun(self: Player, maxMana: int32)
---@source ../src/luascript.cpp:9320
---@field setManaShieldBar fun(self: Player, capacity: uint16, value: uint16) -- Capacity is the "max mana", value is the current mana
---@source ../src/luascript.cpp:9335
---@field getManaSpent fun(self: Player): uint64
---@source ../src/luascript.cpp:9347
---@field addManaSpent fun(self: Player, amount: uint64)
---@source ../src/luascript.cpp:9360
---@field removeManaSpent fun(self: Player, amount: uint64, notify?: boolean) -- Notify is set to true by default
---@source ../src/luascript.cpp:9373
---@field getBaseMaxHealth fun(self: Player): int32
---@source ../src/luascript.cpp:9385
---@field getBaseMaxMana fun(self: Player): uint32
---@source ../src/luascript.cpp:9397
---@field getSkillLevel fun(self: Player, skill: SkillTypes): uint8
---@source ../src/luascript.cpp:9410
---@field getEffectiveSkillLevel fun(self: Player, skill: SkillTypes): uint16
---@source ../src/luascript.cpp:9423
---@field getSkillPercent fun(self: Player, skill: SkillTypes): uint16
---@source ../src/luascript.cpp:9436
---@field getSkillTries fun(self: Player, skill: SkillTypes): uint64
---@source ../src/luascript.cpp:9449
---@field addSkillTries fun(self: Player, skill: SkillTypes, amount: uint64)
---@source ../src/luascript.cpp:9464
---@field removeSkillTries fun(self: Player, skill: SkillTypes, amount: uint64, notify?: boolean) -- Notify is set to true by default
---@source ../src/luascript.cpp:9479
---@field getSpecialSkill fun(self: Player, skill: SpecialSkills): uint16 --! Why is this uint16. getSpecialSkill returns uint16, however varSpecialSkills is a int32
---@source ../src/luascript.cpp:9492
---@field addSpecialSkill fun(self: Player, skill: SpecialSkills, amount: int32)
---@source ../src/luascript.cpp:9513
---@field addOfflineTrainingTime fun(self: Player, time: int32)
---@source ../src/luascript.cpp:9528
---@field getOfflineTrainingTime fun(self: Player): int32
---@source ../src/luascript.cpp:9540
---@field removeOfflineTrainingTime fun(self: Player, time: int32)
---@source ../src/luascript.cpp:9555
---@field addOfflineTrainingTries fun(self: Player, skill: SkillTypes, amount: uint64): boolean? -- Returns nil if no player, true if player had offline training tries added, false if not
---@source ../src/luascript.cpp:9569
---@field getOfflineTrainingSkill fun(self: Player): int32
---@source ../src/luascript.cpp:9581
---@field setOfflineTrainingSkill fun(self: Player, skill: SkillTypes)
---@source ../src/luascript.cpp:9595
---@field getItemCount fun(self: Player, itemId: uint16, subType?: int32): uint32
---@source ../src/luascript.cpp:9620
---@field getItemById fun(self: Player, itemId: uint16, deepSearch?:boolean, subType?: int32): Item
---@source ../src/luascript.cpp:9652
---@field getVocation fun(self: Player): Vocation
---@source ../src/luascript.cpp:9665
---@field setVocation fun(self: Player, vocationId: uint16|string|Vocation): boolean
---@source ../src/luascript.cpp:9695
---@field getSex fun(self: Player): PlayerSexes
---@source ../src/luascript.cpp:9707
---@field setSex fun(self: Player, sexId: PlayerSexes)
---@source ../src/luascript.cpp:9721
---@field getTown fun(self: Player): Town
---@source ../src/luascript.cpp:9733
---@field setTown fun(self: Player, townId: uint32)
---@source ../src/luascript.cpp:9757
---@field getGuild fun(self: Player): Guild
---@source ../src/luascript.cpp:9775
---@field setGuild fun(self: Player, guild: Guild)
---@source ../src/luascript.cpp:9789
---@field getGuildLevel fun(self: Player): uint8 -- GuildLevel is the rank of the player in the guild
---@source ../src/luascript.cpp:9801
---@field setGuildLevel fun(self: Player, level: uint8) -- GuildLevel is the rank of the player in the guild
---@source ../src/luascript.cpp:9826
---@field getGuildNick fun(self: Player): string
---@source ../src/luascript.cpp:9838
---@field setGuildNick fun(self: Player, nick: string)
---@source ../src/luascript.cpp:9852
---@field getGroup fun(self: Player): Group
---@source ../src/luascript.cpp:9865
---@field setGroup fun(self: Player, groupId: uint16)
---@source ../src/luascript.cpp:9884
---@field getStamina fun(self: Player): uint16
---@source ../src/luascript.cpp:9896
---@field setStamina fun(self: Player, stamina: uint16)
---@source ../src/luascript.cpp:9911
---@field getSoul fun(self: Player): uint8
---@source ../src/luascript.cpp:9923
---@field addSoul fun(self: Player, amount: int32)
---@source ../src/luascript.cpp:9937
---@field getMaxSoul fun(self: Player): uint8
---@source ../src/luascript.cpp:9949
---@field getBankBalance fun(self: Player): uint64
---@source ../src/luascript.cpp:9961
---@field setBankBalance fun(self: Player, balance: uint64)
---@source ../src/luascript.cpp:9982
---@field addItem fun(self: Player, itemId: uint16|string, count?: int32, canDropOnMap?: boolean, subType?: int32, slot?: PlayerEquipmentSlots): Item
---@source ../src/luascript.cpp:10068
---@field addItemEx fun(self: Player, item: Item, dropOnMap?: boolean, indexOrSlot?: PlayerEquipmentSlots|int32): boolean -- If dropOnMap is FALSE, index = INDEXWHEREVER unless defined and returns internalPlayerAddItem, if dropOnMap is TRUE, slot = CONST_SLOT_WHEREEVER, and returns internalAddItem
---@source ../src/luascript.cpp:10109 
---@field removeItem fun(self: Player, itemId: uint16|string, count?: uint32, subtype?: int32, ignoreEquipped?: boolean): boolean 
---@source ../src/luascript.cpp:10136
---@field sendSupplyUsed fun(self: Player, item: Item)
---@source ../src/luascript.cpp:10158
---@field getMoney fun(self: Player): uint64
---@source ../src/luascript.cpp:10170
---@field addMoney fun(self: Player, amount: uint64)
---@source ../src/luascript.cpp:10184
---@field removeMoney fun(self: Player, amount: uint64)
---@source ../src/luascript.cpp:10197
---@field showTextDialog fun(self: Player, itemToShow: uint16|string|Item, text: string, canWrite: boolean, length: int32): boolean?|uint32 -- Returns nil if no player. Returns false if cannot show the window. Push 
---@source ../src/luascript.cpp:10255
---@field sendTextMessage fun(self: Player, messageType: MessageTypes, message: string, position?: Position|number, primaryValue?: int32, primaryColor?: TextColors, secondaryValue?: int32, secondaryColor?: TextColors): boolean? -- Returns nil if no player, true if message sent, false if cannot send message (e.g. player is not part of that channel)
---@source ../src/luascript.cpp:10296
---@field sendChannelMessage fun(self: Player, author: string, message: string, type: SpeakClasses, channelId: uint16): boolean? -- Returns nil if no player, true if message sent
---@source ../src/luascript.cpp:10314
---@field sendPrivateMessage fun(self: Player, receiver: Player, message: string, type?: SpeakClasses)
---@source ../src/luascript.cpp:10331
---@field channelSay fun(self: Player, speaker: Creature, type: SpeakClasses, message: string, channelId: uint16)
---@source ../src/luascript.cpp:10349
---@field openChannel fun(self: Player, channelId: uint16)
---@source ../src/luascript.cpp:10377
---@field getSlotItem fun(self: Player, slot: PlayerEquipmentSlots): Item
---@source ../src/luascript.cpp:10403
---@field getParty fun(self: Player): Party
---@source ../src/luascript.cpp:10422
---@field addOutfit fun(self: Player, outfitId: uint16)
---@source ../src/luascript.cpp:10435
---@field addOutfitAddon fun(self: Player, outfitId: uint16, addonId: uint8)
---@source ../src/luascript.cpp:10450
---@field removeOutfit fun(self: Player, outfitId: uint16)
---@source ../src/luascript.cpp:10463
---@field removeOutfitAddon fun(self: Player, outfitId: uint16, addonId: uint8)
---@source ../src/luascript.cpp:10477
---@field hasOutfit fun(self: Player, outfitId: uint16, addon?: uint8): boolean
---@source ../src/luascript.cpp:10491
---@field canWearOutfit fun(self: Player, outfitId: uint16, addonId?: uint8): boolean
---@source ../src/luascript.cpp:10505
---@field sendOutfitWindow fun(self: Player)
---@source ../src/luascript.cpp:10518
---@field sendEditPodium fun(self: Player, item: Item)
---@source ../src/luascript.cpp:10533
---@field addMount fun(self: Player, mountId: uint16): boolean? -- Returns nil if no player or no mount, false if player already has mount, true if tamed
---@source ../src/luascript.cpp:10556
---@field removeMount fun(self: Player, mountId: uint16): boolean? -- Returns nil if no player or no mount, false if player does not have mount, true if untamed
---@source ../src/luascript.cpp:10580
---@field hasMount fun(self: Player, mountId: uint16): boolean
---@source ../src/luascript.cpp:10604
---@field toggleMount fun(self: Player, active: boolean)
---@source ../src/luascript.cpp:10618
---@field getPremiumEndsAt fun(self: Player): number
---@source ../src/luascript.cpp:10630
---@field setPremiumEndsAt fun(self: Player, timestamp: number)
---@source ../src/luascript.cpp:10647
---@field hasBlessing fun(self: Player, blessingId: uint8): boolean? -- Returns nil if no player, true if player has blessing, false if player does not have blessing
---@source ../src/luascript.cpp:10660
---@field addBlessing fun(self: Player, blessingId: uint8): boolean? -- Returns nil if no player, true if player did not have the blessing and it was added, false if player already had the blessing
---@source ../src/luascript.cpp:10680
---@field removeBlessing fun(self: Player, blessingId: uint8): boolean? -- Returns nil if no player, true if player had the blessing and it was removed, false if player did not have the blessing
---@source ../src/luascript.cpp:10700
---@field canLearnSpell fun(self: Player, spellName: string): boolean? -- Returns nil if no player, true if player can learn the spell, false if player cannot learn the spell
---@source ../src/luascript.cpp:10734
---@field learnSpell fun(self: Player, spellName: string): boolean? -- Returns nil if no player, true if player. Does not check if player already had the spell
---@source ../src/luascript.cpp:10748
---@field forgetSpell fun(self: Player, spellName: string): boolean? -- Returns nil if no player, true if player. Does not check if player forgot the spell
---@source ../src/luascript.cpp:10762
---@field hasLearnedSpell fun(self: Player, spellName: string): boolean -- Retrusn nil if no player, true if player has learned the spell, false if player has not learned the spell
---@source ../src/luascript.cpp:10775
---@field sendTutorial fun(self: Player, tutorialId: uint8)
---@source ../src/luascript.cpp:10789
---@field addMapMark fun(self: Player, position: Position, type: MapMarks, description?: string)
---@source ../src/luascript.cpp:10805
---@field save fun(self: Player)
---@source ../src/luascript.cpp:10818
---@field popupFYI fun(self: Player, message: string)
---@source ../src/luascript.cpp:10832
---@field isPzLocked fun(self: Player): boolean
---@source ../src/luascript.cpp:10844
---@field getClient fun(self: Player): UserClientVersion -- ClientVersion is a table {version = uint16, os = OperatingSystem}
---@source ../src/luascript.cpp:10858
---@field getHouse fun(self: Player): House
---@source ../src/luascript.cpp:10877
---@field sendHouseWindow fun(self: Player, houseId: uint32, listId: uint32)
---@source ../src/luascript.cpp:10898
---@field setEditHouse fun(self: Player, houseId: uint32, listId: uint32)
---@source ../src/luascript.cpp:10919
---@field setGhostMode fun(self: Player, enabled: boolean, magicEffect: MagicEffectConsts)
---@source ../src/luascript.cpp:10983
---@field getContainerId fun(self: Player, container: Container): int8
---@source ../src/luascript.cpp:11001
---@field getContainerById fun(self: Player, containerId: int8): Container
---@source ../src/luascript.cpp:11020
---@field getContainerIndex fun(self: Player, containerIndex: uint8): uint16
---@source ../src/luascript.cpp:11032
---@field getInstantSpells fun(self: Player): table<number, SpellsTable> -- Spells table has the following information in the table: {name = string, words = string, level = uint32, mlevel = uint32, mana = uint32, manapercent = uint32, params = boolean}
---@source ../src/luascript.cpp:11068
---@field canCast fun(self: Player, spellId: number): boolean
---@source ../src/luascript.cpp:11081
---@field hasChaseMode fun(self: Player): boolean
---@source ../src/luascript.cpp:11093
---@field hasSecureMode fun(self: Player): boolean
---@source ../src/luascript.cpp:11105
---@field getFightMode fun(self: Player): FightModes
---@source ../src/luascript.cpp:11117
---@field getStoreInbox fun(self: Player): Container
---@source ../src/luascript.cpp:11137
---@field isNearDepotBox fun(self: Player): boolean
---@source ../src/luascript.cpp:11150
---@field getIdleTime fun(self: Player): int32
---@source ../src/luascript.cpp:11163
---@field resetIdleTime fun(self: Player)
---@source ../src/luascript.cpp:11177
---@field sendCreatureSquare fun(self: Player, creature: Creature, color: SquareColours)
---@source ../src/luascript.cpp:11198
---@field getClientExpDisplay fun(self: Player): uint16
---@source ../src/luascript.cpp:11210
---@field setClientExpDisplay fun(self: Player, display: uint16)
---@source ../src/luascript.cpp:11224
---@field getClientStaminaBonusDisplay fun(self: Player): uint16
---@source ../src/luascript.cpp:11236
---@field setClientStaminaBonusDisplay fun(self: Player, display: uint16)
---@source ../src/luascript.cpp:11250
---@field getClientLowLevelBonusDisplay fun(self: Player): uint16
---@source ../src/luascript.cpp:11262
---@field setClientLowLevelBonusDisplay fun(self: Player, display: uint16)
---@source ../src/luascript.cpp:11276
---@field sendResourceBalance fun(self: Player, resource: ResourceTypes, amount: uint64): boolean?
---@source ../src/luascript.cpp:11291
---@field sendEnterMarket fun(self: Player): boolean?
Player = {}

---@class Monster : Creature
---@source ../src/luascript.cpp:11305
---@field create fun(monsterInfo: uint32|Monster): Monster -- Monster info can either be the ID of the monster, or user data of the monster
---@field __eq fun(self: Monster, other: Monster): boolean
---@source ../src/luascript.cpp:11330
---@field isMonster fun(self: Monster): boolean
---@source ../src/luascript.cpp:11337
---@field getId fun(self: Monster): uint32
---@source ../src/luascript.cpp:11354
---@field getType fun(self: Monster): MonsterType
---@source ../src/luascript.cpp:11367
---@field rename fun(self: Monster, newName: string, nameDescription?: string)
---@source ../src/luascript.cpp:11385
---@field getSpawnPosition fun(self: Monster): Position
---@source ../src/luascript.cpp:11397
---@field isInSpawnRange fun(self: Monster, position?: Position): boolean
---@source ../src/luascript.cpp:11410
---@field isIdle fun(self: Monster): boolean
---@source ../src/luascript.cpp:11422
---@field setIdle fun(self: Monster, idle: boolean)
---@source ../src/luascript.cpp:11436
---@field isTarget fun(self: Monster, creature: Creature): boolean
---@source ../src/luascript.cpp:114555
---@field isOpponent fun(self: Monster, creature: Creature): boolean
---@source ../src/luascript.cpp:11474
---@field isFriend fun(self: Monster, creature: Creature): boolean
---@source ../src/luascript.cpp:11493
---@field addFriend fun(self: Monster, creature: Creature)
---@source ../src/luascript.cpp:11513
---@field removeFriend fun(self: Monster, creature: Creature)
---@source ../src/luascript.cpp:11533
---@field getFriendList fun(self: Monster): table<number, Creature>
---@source ../src/luascript.cpp:11554
---@field getFriendCount fun(self: Monster): number
---@source ../src/luascript.cpp:11566
---@field addTarget fun(self: Monster, creature: Creature, pushFront?: boolean): boolean? -- Returns nil if no monster, false if no creature to target, true if success
---@source ../src/luascript.cpp:11588
---@field removeTarget fun(self: Monster, creature: Creature)
---@source ../src/luascript.cpp:11609
---@field getTargetList fun(self: Monster): table<number, Creature>
---@source ../src/luascript.cpp:11630
---@field getTargetCount fun(self: Monster): number
---@source ../src/luascript.cpp:11642
---@field selectTarget fun(self: Monster, creature: Creature): boolean? -- Returns nil if no monster, false if no creature to target, true/false if success
---@source ../src/luascript.cpp:11661
---@field searchTarget fun(self: Monster, searchType: TargetSearchType): boolean
---@source ../src/luascript.cpp:11674
---@field isWalkingToSpawn fun(self: Monster): boolean
---@source ../src/luascript.cpp:11686
---@field walkToSpawn fun(self: Monster): boolean
---@source ../src/luascript.cpp:11698
---@field hasSpecialIcon fun(self: Monster, iconId: MonsterIcons): boolean
---@source ../src/luascript.cpp:11710
---@field setSpecialIcon fun(self: Monster, iconId: MonsterIcons, value: uint16)
---@source ../src/luascript.cpp:11733
---@field getSpecialIcon fun(self: Monster): number -- Returns the value stored with the special icon
---@source ../src/luascript.cpp:11753
---@field removeSpecialIcon fun(self: Monster, iconId: MonsterIcons)
Monster = {}

---@class Npc : Creature
---@source ../src/luascript.cpp:11766
---@field create fun(npcInfo: uint32|string|Npc): Npc
---@field __eq fun(self: Npc, other: Npc): boolean
---@source ../src/luascript.cpp:11807
---@field isNpc fun(self: Npc): boolean
---@source ../src/luascript.cpp:11814
---@field setMasterPos fun(self: Npc, position: Position, radius?: int32)
---@source ../src/luascript.cpp:11830
---@field getSpeechBubble fun(self: Npc): uint8
---@source ../src/luascript.cpp:11842
---@field setSpeechBubble fun(self: Npc, bubbleType: uint8)
---@source ../src/luascript.cpp:11869
---@field getSpectators fun(self: Npc): table<number, Player> -- Returns a table of all players the npc can see
Npc = {}

---@class Guild
---@field create fun(): Guild
---@field __eq fun(self: Guild, other: Guild): boolean
---@field getId fun(self: Guild): number
---@field getName fun(self: Guild): string
---@field getMembersOnline fun(self: Guild): number
---@field addRank fun(self: Guild, rankName: string, rankLevel: number)
---@field getRankById fun(self: Guild, rankId: number): string
---@field getRankByLevel fun(self: Guild, level: number): string
---@field getMotd fun(self: Guild): string
---@field setMotd fun(self: Guild, motd: string)
Guild = {}

---@class Group
---@field create fun(): Group
---@field __eq fun(self: Group, other: Group): boolean
---@field getId fun(self: Group): number
---@field getName fun(self: Group): string
---@field getFlags fun(self: Group): number
---@field getAccess fun(self: Group): number
---@field getMaxDepotItems fun(self: Group): number
---@field getMaxVipEntries fun(self: Group): number
---@field hasFlag fun(self: Group, flag: number): boolean
Group = {}

---@class Vocation
---@field create fun(): Vocation
---@field __eq fun(self: Vocation, other: Vocation): boolean
---@field getId fun(self: Vocation): number
---@field getClientId fun(self: Vocation): number
---@field getName fun(self: Vocation): string
---@field getDescription fun(self: Vocation): string
---@field getRequiredSkillTries fun(self: Vocation, skillType: number, skillLevel: number): number
---@field getRequiredManaSpent fun(self: Vocation, magicLevel: number): number
---@field getCapacityGain fun(self: Vocation): number
---@field getHealthGain fun(self: Vocation): number
---@field getHealthGainTicks fun(self: Vocation): number
---@field getHealthGainAmount fun(self: Vocation): number
---@field getManaGain fun(self: Vocation): number
---@field getManaGainTicks fun(self: Vocation): number
---@field getManaGainAmount fun(self: Vocation): number
---@field getMaxSoul fun(self: Vocation): number
---@field getSoulGainTicks fun(self: Vocation): number
---@field getAttackSpeed fun(self: Vocation): number
---@field getBaseSpeed fun(self: Vocation): number
---@field getDemotion fun(self: Vocation): Vocation
---@field getPromotion fun(self: Vocation): Vocation
---@field allowsPvp fun(self: Vocation): boolean
Vocation = {}

---@class Town
---@field create fun(): Town
---@field __eq fun(self: Town, other: Town): boolean
---@field getId fun(self: Town): number
---@field getName fun(self: Town): string
---@field getTemplePosition fun(self: Town): Position
Town = {}

---@class House
---@field create fun(): House
---@field __eq fun(self: House, other: House): boolean
---@field getId fun(self: House): number
---@field getName fun(self: House): string
---@field getTown fun(self: House): Town
---@field getExitPosition fun(self: House): Position
---@field getRent fun(self: House): number
---@field setRent fun(self: House, rent: number)
---@field getPaidUntil fun(self: House): number
---@field setPaidUntil fun(self: House, timestamp: number)
---@field getPayRentWarnings fun(self: House): number
---@field setPayRentWarnings fun(self: House, warnings: number)
---@field getOwnerName fun(self: House): string
---@field getOwnerGuid fun(self: House): number
---@field setOwnerGuid fun(self: House, guid: number)
---@field startTrade fun(self: House)
---@field getBeds fun(self: House): table
---@field getBedCount fun(self: House): number
---@field getDoors fun(self: House): table
---@field getDoorCount fun(self: House): number
---@field getDoorIdByPosition fun(self: House, position: Position): number
---@field getTiles fun(self: House): table
---@field getItems fun(self: House): table
---@field getTileCount fun(self: House): number
---@field canEditAccessList fun(self: House): boolean
---@field getAccessList fun(self: House, listId: number): string
---@field setAccessList fun(self: House, listId: number, list: string)
---@field kickPlayer fun(self: House, player: Player)
---@field save fun(self: House)
House = {}

---@class ItemType
---@field create fun(): ItemType
---@field __eq fun(self: ItemType, other: ItemType): boolean
---@field isCorpse fun(self: ItemType): boolean
---@field isDoor fun(self: ItemType): boolean
---@field isContainer fun(self: ItemType): boolean
---@field isFluidContainer fun(self: ItemType): boolean
---@field isMovable fun(self: ItemType): boolean
---@field isRune fun(self: ItemType): boolean
---@field isStackable fun(self: ItemType): boolean
---@field isReadable fun(self: ItemType): boolean
---@field isWritable fun(self: ItemType): boolean
---@field isBlocking fun(self: ItemType): boolean
---@field isGroundTile fun(self: ItemType): boolean
---@field isMagicField fun(self: ItemType): boolean
---@field isUseable fun(self: ItemType): boolean
---@field isPickupable fun(self: ItemType): boolean
---@field isRotatable fun(self: ItemType): boolean
---@field getType fun(self: ItemType): string
---@field getGroup fun(self: ItemType): string
---@field getId fun(self: ItemType): number
---@field getClientId fun(self: ItemType): number
---@field getName fun(self: ItemType): string
---@field getPluralName fun(self: ItemType): string
---@field getRotateTo fun(self: ItemType): number
---@field getArticle fun(self: ItemType): string
---@field getDescription fun(self: ItemType): string
---@field getSlotPosition fun(self: ItemType): number
---@field getCharges fun(self: ItemType): number
---@field getFluidSource fun(self: ItemType): number
---@field getCapacity fun(self: ItemType): number
---@field getWeight fun(self: ItemType): number
---@field getWorth fun(self: ItemType): number
---@field getHitChance fun(self: ItemType): number
---@field getShootRange fun(self: ItemType): number
---@field getAttack fun(self: ItemType): number
---@field getAttackSpeed fun(self: ItemType): number
---@field getDefense fun(self: ItemType): number
---@field getExtraDefense fun(self: ItemType): number
---@field getArmor fun(self: ItemType): number
---@field getWeaponType fun(self: ItemType): string
---@field getElementType fun(self: ItemType): string
---@field getElementDamage fun(self: ItemType): number
---@field getTransformEquipId fun(self: ItemType): number
---@field getTransformDeEquipId fun(self: ItemType): number
---@field getDestroyId fun(self: ItemType): number
---@field getDecayId fun(self: ItemType): number
---@field getRequiredLevel fun(self: ItemType): number
---@field getAmmoType fun(self: ItemType): string
---@field getCorpseType fun(self: ItemType): string
---@field getClassification fun(self: ItemType): string
---@field getAbilities fun(self: ItemType): table
---@field hasShowAttributes fun(self: ItemType): boolean
---@field hasShowCount fun(self: ItemType): boolean
---@field hasShowCharges fun(self: ItemType): boolean
---@field hasShowDuration fun(self: ItemType): boolean
---@field hasAllowDistRead fun(self: ItemType): boolean
---@field getWieldInfo fun(self: ItemType): string
---@field getDurationMin fun(self: ItemType): number
---@field getDurationMax fun(self: ItemType): number
---@field getLevelDoor fun(self: ItemType): number
---@field getRuneSpellName fun(self: ItemType): string
---@field getVocationString fun(self: ItemType): string
---@field getMinReqLevel fun(self: ItemType): number
---@field getMinReqMagicLevel fun(self: ItemType): number
---@field getMarketBuyStatistics fun(self: ItemType): table
---@field getMarketSellStatistics fun(self: ItemType): table
---@field hasSubType fun(self: ItemType): boolean
---@field isStoreItem fun(self: ItemType): boolean
ItemType = {}

---@class Combat
---@field create fun(): Combat
---@field __eq fun(self: Combat, other: Combat): boolean
---@field __gc fun(self: Combat)
---@field delete fun(self: Combat)
---@field setParameter fun(self: Combat, key: number, value: any)
---@field getParameter fun(self: Combat, key: number): any
---@field setFormula fun(self: Combat, formulaType: number, base: number, variation: number, multiplier: number)
---@field setArea fun(self: Combat, area: table)
---@field addCondition fun(self: Combat, condition: Condition)
---@field clearConditions fun(self: Combat)
---@field setCallback fun(self: Combat, callback: function)
---@field setOrigin fun(self: Combat, originType: number)
---@field execute fun(self: Combat, caster: Creature, target: Variant)
Combat = {}

---@class Condition
---@field create fun(): Condition
---@field __eq fun(self: Condition, other: Condition): boolean
---@field __gc fun(self: Condition)
---@field getId fun(self: Condition): number
---@field getSubId fun(self: Condition): number
---@field getType fun(self: Condition): number
---@field getIcons fun(self: Condition): number
---@field getEndTime fun(self: Condition): number
---@field clone fun(self: Condition): Condition
---@field getTicks fun(self: Condition): number
---@field setTicks fun(self: Condition, ticks: number)
---@field setParameter fun(self: Condition, key: number, value: any)
---@field getParameter fun(self: Condition, key: number): any
---@field setFormula fun(self: Condition, formulaType: number, base: number, variation: number, multiplier: number)
---@field setOutfit fun(self: Condition, outfit: Outfit)
---@field addDamage fun(self: Condition, amount: number, interval: number)
Condition = {}

---@class Outfit
---@field create fun(): Outfit
---@field lookType number
---@field lookHead number
---@field lookBody number
---@field lookLegs number
---@field lookFeet number
---@field lookAddons number
---@field lookMount number
---@field lookMountHead number
---@field lookMountBody number
---@field lookMountLegs number
---@field lookMountFeet number
---@field lookTypeEx number
Outfit = {}

---@class MonsterType
---@field create fun(): MonsterType
---@field __eq fun(self: MonsterType, other: MonsterType): boolean
---@field isAttackable fun(self: MonsterType): boolean
---@field isChallengeable fun(self: MonsterType): boolean
---@field isConvinceable fun(self: MonsterType): boolean
---@field isSummonable fun(self: MonsterType): boolean
---@field isIgnoringSpawnBlock fun(self: MonsterType): boolean
---@field isIllusionable fun(self: MonsterType): boolean
---@field isHostile fun(self: MonsterType): boolean
---@field isPushable fun(self: MonsterType): boolean
---@field isHealthHidden fun(self: MonsterType): boolean
---@field isBoss fun(self: MonsterType): boolean
---@field canPushItems fun(self: MonsterType): boolean
---@field canPushCreatures fun(self: MonsterType): boolean
---@field canWalkOnEnergy fun(self: MonsterType): boolean
---@field canWalkOnFire fun(self: MonsterType): boolean
---@field canWalkOnPoison fun(self: MonsterType): boolean
---@field name fun(self: MonsterType): string
---@field nameDescription fun(self: MonsterType): string
---@field health fun(self: MonsterType): number
---@field maxHealth fun(self: MonsterType): number
---@field runHealth fun(self: MonsterType): number
---@field experience fun(self: MonsterType): number
---@field skull fun(self: MonsterType): number
---@field combatImmunities fun(self: MonsterType): number
---@field conditionImmunities fun(self: MonsterType): number
---@field getAttackList fun(self: MonsterType): table
---@field addAttack fun(self: MonsterType, attack: table)
---@field getDefenseList fun(self: MonsterType): table
---@field addDefense fun(self: MonsterType, defense: table)
---@field getElementList fun(self: MonsterType): table
---@field addElement fun(self: MonsterType, element: table)
---@field getVoices fun(self: MonsterType): table
---@field addVoice fun(self: MonsterType, voice: table)
---@field getLoot fun(self: MonsterType): table
---@field addLoot fun(self: MonsterType, loot: table)
---@field getCreatureEvents fun(self: MonsterType): table
---@field registerEvent fun(self: MonsterType, event: string)
---@field eventType fun(self: MonsterType): string
---@field onThink fun(self: MonsterType, callback: function)
---@field onAppear fun(self: MonsterType, callback: function)
---@field onDisappear fun(self: MonsterType, callback: function)
---@field onMove fun(self: MonsterType, callback: function)
---@field onSay fun(self: MonsterType, callback: function)
---@field getSummonList fun(self: MonsterType): table
---@field addSummon fun(self: MonsterType, summon: table)
---@field maxSummons fun(self: MonsterType): number
---@field armor fun(self: MonsterType): number
---@field defense fun(self: MonsterType): number
---@field outfit fun(self: MonsterType): Outfit
---@field race fun(self: MonsterType): string
---@field corpseId fun(self: MonsterType): number
---@field manaCost fun(self: MonsterType): number
---@field baseSpeed fun(self: MonsterType): number
---@field light fun(self: MonsterType): table
---@field staticAttackChance fun(self: MonsterType): number
---@field targetDistance fun(self: MonsterType): number
---@field yellChance fun(self: MonsterType): number
---@field yellSpeedTicks fun(self: MonsterType): number
---@field changeTargetChance fun(self: MonsterType): number
---@field changeTargetSpeed fun(self: MonsterType): number
---@field bestiaryInfo fun(self: MonsterType): string
MonsterType = {}

---@class Loot
---@field create fun(): Loot
---@field __gc fun(self: Loot)
---@field delete fun(self: Loot)
---@field setId fun(self: Loot, id: number)
---@field setMaxCount fun(self: Loot, count: number)
---@field setSubType fun(self: Loot, subtype: number)
---@field setChance fun(self: Loot, chance: number)
---@field setActionId fun(self: Loot, actionId: number)
---@field setDescription fun(self: Loot, description: string)
---@field addChildLoot fun(self: Loot, childLoot: Loot)
Loot = {}

---@class MonsterSpell
---@field create fun(): MonsterSpell
---@field __gc fun(self: MonsterSpell)
---@field delete fun(self: MonsterSpell)
---@field setType fun(self: MonsterSpell, type: string)
---@field setScriptName fun(self: MonsterSpell, scriptName: string)
---@field setChance fun(self: MonsterSpell, chance: number)
---@field setInterval fun(self: MonsterSpell, interval: number)
---@field setRange fun(self: MonsterSpell, range: number)
---@field setCombatValue fun(self: MonsterSpell, value: number)
---@field setCombatType fun(self: MonsterSpell, type: string)
---@field setAttackValue fun(self: MonsterSpell, value: number)
---@field setNeedTarget fun(self: MonsterSpell, needTarget: boolean)
---@field setNeedDirection fun(self: MonsterSpell, needDirection: boolean)
---@field setCombatLength fun(self: MonsterSpell, length: number)
---@field setCombatSpread fun(self: MonsterSpell, spread: number)
---@field setCombatRadius fun(self: MonsterSpell, radius: number)
---@field setCombatRing fun(self: MonsterSpell, ring: number)
---@field setConditionType fun(self: MonsterSpell, type: string)
---@field setConditionDamage fun(self: MonsterSpell, damage: number)
---@field setConditionSpeedChange fun(self: MonsterSpell, speedChange: number)
---@field setConditionDuration fun(self: MonsterSpell, duration: number)
---@field setConditionDrunkenness fun(self: MonsterSpell, drunkenness: number)
---@field setConditionTickInterval fun(self: MonsterSpell, tickInterval: number)
---@field setCombatShootEffect fun(self: MonsterSpell, effect: string)
---@field setCombatEffect fun(self: MonsterSpell, effect: string)
---@field setOutfit fun(self: MonsterSpell, outfit: Outfit)
MonsterSpell = {}

---@class Party
---@field create fun(): Party
---@field __eq fun(self: Party, other: Party): boolean
---@field disband fun(self: Party)
---@field getLeader fun(self: Party): Player
---@field setLeader fun(self: Party, leader: Player)
---@field getMembers fun(self: Party): table
---@field getMemberCount fun(self: Party): number
---@field getInvitees fun(self: Party): table
---@field getInviteeCount fun(self: Party): number
---@field addInvite fun(self: Party, player: Player)
---@field removeInvite fun(self: Party, player: Player)
---@field addMember fun(self: Party, player: Player)
---@field removeMember fun(self: Party, player: Player)
---@field isSharedExperienceActive fun(self: Party): boolean
---@field isSharedExperienceEnabled fun(self: Party): boolean
---@field isMemberSharingExp fun(self: Party, player: Player): boolean
---@field shareExperience fun(self: Party, experience: number)
---@field setSharedExperience fun(self: Party, active: boolean)
Party = {}

-- MARK: Spell
---@class Spell
---@field onCastSpell fun(player: Player, variant: Variant, isHotkey: boolean|nil): boolean
---@field register fun(self: Spell): boolean
---@field name fun(self: Spell, name: string|nil): string|boolean
---@field id fun(self: Spell, id: integer|nil): integer|boolean
---@field cooldown fun(self: Spell, cooldown: integer|nil): integer|boolean
---@field group fun(self: Spell, primaryGroup?: IntOrStr, secondaryGroup?: IntOrStr): integer, integer|boolean
---@field groupCooldown fun(self:Spell, primaryGroupCd?:integer, secondaryGroupCd?:integer): integer, integer|boolean
---@field level fun(self:Spell, lvl?:integer): integer|boolean
---@field magicLevel fun(self:Spell, lvl?:integer):integer | boolean
---@field mana fun(self:Spell, mana?:integer):integer | boolean
---@field manaPercent fun(self:Spell, percent?:integer):integer | boolean
---@field soul fun(self:Spell, soul?:integer):integer | boolean
---@field range fun(self:Spell, range?:integer):integer | boolean
---@field isPremium fun(self:Spell, bool?:boolean):boolean
---@field isEnabled fun(self:Spell, bool?:boolean):boolean
---@field needTarget fun(self:Spell, bool?:boolean):boolean
---@field needWeapon fun(self:Spell, bool?:boolean):boolean
---@field needLearn fun(self:Spell, bool?:boolean):boolean
---@field isSelfTarget fun(self:Spell, bool?:boolean):boolean
---@field isBlocking fun(self:Spell, blockingSolid?:boolean, blockingCreature?:boolean):boolean, boolean | boolean
---@field isAggressive fun(self:Spell, bool?:boolean):boolean
---@field isPzLock fun(self:Spell, bool?:boolean):boolean
---@field vocation fun(self:Spell, vocation?:string, ...?):string[] | boolean
-- Only for InstantSpells
---@field words fun(self:Spell, words?:string, separator?:string):string, string | boolean -- Only for InstantSpells
---@field needDirection fun(self:Spell, bool?:boolean):boolean -- Only for InstantSpells
---@field hasParams fun(self:Spell, bool?:boolean):boolean -- Only for InstantSpells
---@field hasPlayerNameParam fun(self:Spell, bool?:boolean):boolean -- Only for InstantSpells
---@field needCasterTargetOrDirection fun(self:Spell, bool?:boolean):boolean -- Only for InstantSpells
---@field isBlockingWalls fun(self:Spell, bool1?:boolean):boolean -- Only for InstantSpells
-- Only for RuneSpells
---@field runeLevel fun(self:Spell, level?:integer):integer | boolean -- Only for RuneSpells
---@field runeMagicLevel fun(self:Spell, magLevel?:integer):integer | boolean -- Only for RuneSpells
---@field runeId fun(self:Spell, id?:integer):integer | boolean -- Only for RuneSpells
---@field charges fun(self:Spell, charges?:integer):integer | boolean -- Only for RuneSpells
---@field allowFarUse fun(self:Spell, bool?:boolean):boolean -- Only for RuneSpells
---@field blockWalls fun(self:Spell, bool1?:boolean):boolean -- Only for RuneSpells
---@field checkFloor fun(self:Spell, bool?:boolean):boolean -- Only for RuneSpells
-- From Player.getInstantSpells
---@field mlevel integer -- Only from Player.getInstantSpells
---@field manapercent integer -- Only from Player.getInstantSpells
---@field params boolean -- Only from Player.getInstantSpells
---@operator call(string | integer):Spell
Spell = {}

-- MARK: Action
---@class Action
---@field onUse fun(player:Player, item:Item, fromPos:Position, target?:(Creature|Item), toPos:Position, isHotkey:boolean):boolean
---@field register fun(self:Action):boolean
---@field id fun(self:Action, itemId:integer, ...?):boolean
---@field aid fun(self:Action, actionId:integer, ...?):boolean
---@field uid fun(self:Action, uniqueId:integer, ...?):boolean
---@field allowFarUse fun(self:Action, allowFarUse:boolean):boolean
---@field blockWalls fun(self:Action, blockWalls:boolean):boolean
---@field checkFloor fun(self:Action, checkFloor:boolean):boolean
---@operator call():Action
Action = {}

-- MARK: TalkAction
---@class TalkAction
---@field onSay fun(player:Player, words:string, param:string, type:integer):boolean
---@field register fun(self:TalkAction):boolean
---@field words fun(self:TalkAction):string
---@field separator fun(self:TalkAction, separator?:string):string | boolean
---@field access fun(self:TalkAction, access?:boolean):boolean
---@field accountType fun(self:TalkAction, accountType?:integer):integer | boolean
---@overload fun(...:string):TalkAction
TalkAction = {}

-- MARK: CreatureEvemt
---@class CreatureEvent
---@field type fun(self:CreatureEvent, callback:string):boolean
---@field register fun(self:CreatureEvent):boolean
---@field onLogin fun(player:Player):boolean
---@field onLogout fun(player:Player):boolean
---@field onReconnect fun(player:Player)
---@field onThink fun(creature:Creature, interval:integer):boolean
---@field onPrepareDeath fun(creature:Creature, killer:Creature):boolean
---@field onDeath fun(creature:Creature, corpse:Item, killer:Creature, mostDamageKiller:Creature, lastHitUnjustified:boolean, mostDamageUnjustified:boolean):boolean
---@field onAdvance fun(player:Player, skill:integer, oldLevel:integer, newLevel:integer):boolean
---@field onKill fun(player:Player, target:Creature):boolean
---@field onTextEdit fun(player:Player, item:Item, text:string, windowTextId:integer):boolean
---@field onHealthChange fun(creature:Creature, attacker:Creature, primaryDamage:integer, primaryType:integer, secondaryDamage:integer, secondaryType:integer, origin:integer):integer, integer, integer, integer
---@field onManaChange fun(creature:Creature, attacker:Creature, primaryDamage:integer, primaryType:integer, secondaryDamage:integer, secondaryType:integer, origin:integer):integer, integer, integer, integer
---@field onModalWindow fun(player:Player, modalWindowId:integer, buttonId:integer, choiceId:integer):nil
---@field onExtendedOpcode fun(player:Player, opcode:integer, buffer:string):boolean
---@operator call(string):CreatureEvent
CreatureEvent = {}

-- MARK: MoveEvent
---@class MoveEvent
---@field type fun(self:MoveEvent, callback:string):boolean
---@field register fun(self:MoveEvent):boolean
---@field slot fun(self:MoveEvent, slot:string):boolean
---@field level fun(self:MoveEvent, level:integer):boolean
---@field magicLevel fun(self:MoveEvent, magicLevel:integer):boolean
---@field premium fun(self:MoveEvent, premium:boolean):boolean
---@field vocation fun(self:MoveEvent, vocation:string, showInDescription?:boolean, lastVocation?:boolean):boolean
---@field tileItem fun(self:MoveEvent, tileItem:boolean):boolean
---@field id fun(self:MoveEvent, itemId:integer, ...?):boolean
---@field aid fun(self:MoveEvent, actionId:integer, ...?):boolean
---@field uid fun(self:MoveEvent, uniqueId:integer, ...?):boolean
---@field position fun(self:MoveEvent, position:Position, ...?):boolean
---@field onStepIn fun(creature:Creature, item:Item, position:Position, fromPosition:Position):boolean
---@field onStepOut fun(creature:Creature, item:Item, position:Position, fromPosition:Position):boolean
---@field onEquip fun(player:Player, item:Item, slot:integer, isCheck:boolean):boolean
---@field onDeEquip fun(player:Player, item:Item, slot:integer, isCheck:boolean):boolean
---@field onAddItem fun(moveItem:Item, tileItem:Item, pos:Position):boolean
---@operator call():MoveEvent
MoveEvent = {}

-- MARK: GlobalEvent
---@class GlobalEvent
---@field type fun(self:GlobalEvent, callback:string):boolean
---@field register fun(self:GlobalEvent):boolean
---```diff
---+ Establece la hora con HH:MM:SS, por ejemplo:00:00:00
---
---@field time fun(self:GlobalEvent, time:string):boolean -- "hh:mm:ss"
---@field interval fun(self:GlobalEvent, interval:integer):boolean
---@field onStartup fun():boolean
---@field onShutdown fun():boolean
---@field onRecord fun(current:integer, old:integer):boolean
---@field onThink fun(interval:integer):boolean
---@field onTime fun(interval:integer):boolean
---@operator call(string):GlobalEvent
GlobalEvent = {}

-- MARK: Weapon
---@class Weapon
---@field action fun(self:Weapon, callback:string):boolean
---@field register fun(self:Weapon):boolean
---@field onUseWeapon fun(player:Player, variant:Variant):boolean
---@field wieldedUnproperly fun(self:Weapon, bool:boolean):boolean
---@field level fun(self:Weapon, lvl:integer):boolean
---@field magicLevel fun(self:Weapon, lvl:integer):boolean
---@field mana fun(self:Weapon, mana:integer):boolean
---@field manaPercent fun(self:Weapon, percent:integer):boolean
---@field health fun(self:Weapon, health:integer):boolean
---@field healthPercent fun(self:Weapon, percent:integer):boolean
---@field soul fun(self:Weapon, soul:integer):boolean
---@field breakChance fun(self:Weapon, percent:integer):boolean
---@field damage fun(self:Weapon, min?:integer, max?:integer):boolean
---@field element fun(self:Weapon, combatType:IntOrStr):boolean
---@field isPremium fun(self:Weapon, bool?:boolean):boolean
---@field vocation fun(self:Weapon, vocName:string, showInDescription?:boolean, lastVoc?:boolean):boolean
---@field id fun(self:Weapon, id:integer):boolean
---@field attack fun(self:Weapon, atk:integer):boolean
---@field defense fun(self:Weapon, defense:integer, extraDefense?:integer):boolean
---@field range fun(self:Weapon, range:integer):boolean
---@field charges fun(self:Weapon, charges:integer, showCharges?:boolean):boolean
---@field duration fun(self:Weapon, duration:integer, showDuration?:boolean):boolean
---@field decayTo fun(self:Weapon, itemid?:integer):boolean
---@field transformEquipTo fun(self:Weapon, itemid:integer):boolean
---@field transformDeEquipTo fun(self:Weapon, itemid:integer):boolean
---@field shootType fun(self:Weapon, type:integer):boolean
---@field slotType fun(self:Weapon, slot:string):boolean
---@field ammoType fun(self:Weapon, type:string):boolean
---@field hitChance fun(self:Weapon, chance:integer):boolean
---@field maxHitChance fun(self:Weapon, max:integer):boolean
---@field extraElement fun(self:Weapon, atk:integer, combatType:IntOrStr):boolean
---@operator call(integer):Weapon
Weapon = {}

---@class Event
---@field register fun(self:Event, triggerIndex?:integer):boolean
---@field onChangeOutfit fun(creature:Creature, outfit: Outfit_t):boolean
---@field onAreaCombat fun(creature:Creature, tile:Tile, aggresive:boolean): integer
---@field onTargetCombat fun(creature:Creature, target:Creature): integer
---@field onHear fun(creature:Creature, speaker:Creature, words:string, type:integer):nil
---@field onChangeZone fun(creature:Creature, fromZone:integer, toZone:integer):nil
---@field onUpdateStorage fun(creature:Creature, key:integer, value?:integer, oldValue?:integer, isSpawn?:boolean):nil
---@field onJoin fun(party:Party, player:Player):boolean
---@field onLeave fun(party:Party, player:Player):boolean
---@field onDisband fun(party:Party):boolean
---@field onShareExperience fun(party:Party, exp:integer, rawExp:integer):integer
---@field onInvite fun(party:Party, player:Player):boolean
---@field onRevokeInvitation fun(party:Party, player:Player):boolean
---@field onPassLeadership fun(party:Party, player:Player):boolean
---@field onLook fun(player:Player, thing:Thing, position:Position, distance:integer, description?:string):string
---@field onLookInBattleList fun(player:Player, creature:Creature, distance:integer, description?:string):string
---@field onLookInTrade fun(player:Player, partner:Player, item:Item, distance:integer, description?:string):string
---@field onLookInShop fun(player:Player, itemType:ItemType, count:integer, description?:string):string
---@field onMoveItem fun(player:Player, item:Item, count:integer, fromPosition:Position, toPosition:Position, fromCylinder?:Thing, toCylinder?:Thing):integer
---@field onItemMoved fun(player:Player, item:Item, count:integer, fromPosition:Position, toPosition:Position, fromCylinder?:Thing, toCylinder?:Thing):nil
---@field onMoveCreature fun(player:Player, creature:Creature, fromPosition:Position, toPosition:Position):boolean
---@field onReportRuleViolation fun(player:Player, targetName:string, reportType:integer, reportReason:integer, comment:string, translation:string):nil
---@field onReportBug fun(player:Player, message:string, position:Position, category:any):boolean
---@field onTurn fun(player:Player, direction:integer):boolean
---@field onTradeRequest fun(player:Player, target:Player, item:Item):boolean
---@field onTradeAccept fun(player:Player, target:Player, item:Item, targetItem:Item):boolean
---@field onTradeCompleted fun(player:Player, target:Player, item:Item, targetItem:Item, isSuccess:boolean):nil
---@field onGainExperience fun(player:Player, source?:Creature, exp:integer, rawExp:integer, sendText:boolean):integer
---@field onLoseExperience fun(player:Player, exp:integer):integer
---@field onGainSkillTries fun(player:Player, skill:integer, tries:integer, artificial?:boolean):integer
---@field onNetworkMessage fun(player:Player, recvByte:integer, msg:NetworkMessage):nil
---@field onUpdateInventory fun(player:Player, item:Item, slot:integer, equip?:boolean):nil
---@field onRotateItem fun(player:Player, item:Item)
---@field onSpellCheck fun(player:Player, spell:Spell):boolean
---@field onDropLoot fun(monster:Monster, corpse?:Container):nil
---@field onSpawn fun(monster:Monster, position:Position, startup:boolean, artificial:boolean):nil
---@field onReload fun(player:Player, reloadType:integer):nil
---@operator call():Event
Event = {}
EventCallback = Event()

---@class hasEvent
---@field onChangeOutfit boolean
---@field onAreaCombat boolean
---@field onTargetCombat boolean
---@field onHear boolean
---@field onChangeZone boolean
---@field onUpdateStorage boolean
---@field onJoin boolean
---@field onLeave boolean
---@field onDisband boolean
---@field onShareExperience boolean
---@field onInvite boolean
---@field onRevokeInvitation boolean
---@field onPassLeadership boolean
---@field onLook boolean
---@field onLookInBattleList boolean
---@field onLookInTrade boolean
---@field onLookInShop boolean
---@field onMoveItem boolean
---@field onItemMoved boolean
---@field onMoveCreature boolean
---@field onReportRuleViolation boolean
---@field onReportBug boolean
---@field onTurn boolean
---@field onTradeRequest boolean
---@field onTradeAccept boolean
---@field onTradeCompleted boolean
---@field onGainExperience boolean
---@field onLoseExperience boolean
---@field onGainSkillTries boolean
---@field onNetworkMessage boolean
---@field onUpdateInventory boolean
---@field onRotateItem boolean
---@field onSpellCheck boolean
---@field onDropLoot boolean
---@field onSpawn boolean
---@field onReload boolean
hasEvent = {}

-- MARK: Thing
---@class Thing
---@field uid integer
---@field itemid integer
---@field actionid integer
---@field type integer
---@field getParent fun(self:Thing):Thing?
---@field getPosition fun(self:Thing):Position
---@field getDescription fun(self:Thing, distance:integer):string?
---@field isTile fun(self:Thing):boolean?
---@field isPosition fun(self:Thing):boolean?
---@field isContainer fun(self:Thing):boolean?
---@field isItem fun(self:Thing):boolean?
---@field isCreature fun(self:Thing):boolean?
---@field isTeleport fun(self:Thing):boolean?
---@field isMonster fun(self:Thing):boolean?
---@field isNpc fun(self:Thing):boolean?
---@field asTile fun(self:Thing):Tile?
---@field asPosition fun(self:Thing):Position?
---@field asContainer fun(self:Thing):Container?
---@field asItem fun(self:Thing):Item?
---@field asCreature fun(self:Thing):Creature?
---@field asPlayer fun(self:Thing):Player?
---@field asTeleport fun(self:Thing):Teleport?
---@field asMonster fun(self:Thing):Monster?
---@field asNpc fun(self:Thing):Npc?
Thing = {}

---@class XMLDocument
---@field create fun(): XMLDocument
---@field __gc fun(self: XMLDocument)
---@field delete fun(self: XMLDocument)
---@field child fun(self: XMLDocument, childName: string): XMLNode
XMLDocument = {}

---@class XMLNode
---@field create fun(): XMLNode
---@field __gc fun(self: XMLNode)
---@field delete fun(self: XMLNode)
---@field attribute fun(self: XMLNode, attributeName: string): string
---@field name fun(self: XMLNode): string
---@field firstChild fun(self: XMLNode): XMLNode
---@field nextSibling fun(self: XMLNode): XMLNode
XMLNode = {}

---@alias IntOrStr integer|string

-- Legacy Functions

--- Adds an item to a player's inventory.
function doPlayerAddItem(uid, itemid, count) end
---@alias doPlayerAddItem fun(uid: number, itemid: number, count?: number): number

--- Checks if a given UID is valid.
function isValidUID(uid) end
---@alias isValidUID fun(uid: number): boolean

--- Checks if a given UID is a depot.
function isDepot(uid) end
---@alias isDepot fun(uid: number): boolean

--- Checks if an item is movable.
function isMovable(uid) end
---@alias isMovable fun(uid: number): boolean

--- Adds an item to a container.
function doAddContainerItem(uid, itemid, count) end
---@alias doAddContainerItem fun(uid: number, itemid: number, count?: number)

--- Retrieves the depot ID associated with a given UID.
function getDepotId(uid) end
---@alias getDepotId fun(uid: number): number

--- Gets the uptime of the world/server.
function getWorldUpTime() end
---@alias getWorldUpTime fun(): number

--- Gets the name of a subtype.
function getSubTypeName(subType) end
---@alias getSubTypeName fun(subType: number): string

--- Creates a combat area.
function createCombatArea(area, extArea) end
---@alias createCombatArea fun(area: table, extArea?: table)

--- Executes area combat.
function doAreaCombat(cid, type, pos, area, min, max, effect, origin, blockArmor, blockShield, ignoreResistances) end
---@alias doAreaCombat fun(cid: number, type: number, pos: table, area: table, min: number, max: number, effect: number, origin?: number, blockArmor?: boolean, blockShield?: boolean, ignoreResistances?: boolean)

--- Executes target combat.
function doTargetCombat(cid, target, type, min, max, effect, origin, blockArmor, blockShield, ignoreResistances) end
---@alias doTargetCombat fun(cid: number, target: number, type: number, min: number, max: number, effect: number, origin?: number, blockArmor?: boolean, blockShield?: boolean, ignoreResistances?: boolean)

--- Challenges a creature.
function doChallengeCreature(cid, target, force) end
---@alias doChallengeCreature fun(cid: number, target: number, force?: boolean)

--- Schedules an event to be called after a delay.
function addEvent(callback, delay, ...) end
---@alias addEvent fun(callback: function, delay: number, ...: any): number

--- Stops a scheduled event.
function stopEvent(eventid) end
---@alias stopEvent fun(eventid: number)

--- Saves the server state.
function saveServer() end
---@alias saveServer fun()

--- Cleans the map.
function cleanMap() end
---@alias cleanMap fun()

--- Outputs a debug message.
function debugPrint(text) end
---@alias debugPrint fun(text: string)

--- Checks if two creatures are in war.
function isInWar(cid, target) end
---@alias isInWar fun(cid: number, target: number): boolean

--- Gets the position of a waypoint by name.
function getWaypointPositionByName(name) return {} end
---@alias getWaypointPositionByName fun(name: string): table

--- Sends a message to a channel.
function sendChannelMessage(channelId, type, message) end
---@alias sendChannelMessage fun(channelId: number, type: number, message: string)

--- Sends a message to a guild channel.
function sendGuildChannelMessage(guildId, type, message) end
---@alias sendGuildChannelMessage fun(guildId: number, type: number, message: string)

--- Checks if the current interface is a scripts interface.
function isScriptsInterface() end
---@alias isScriptsInterface fun(): boolean

function getNpcCid() end
function selfSay(message, player) end
function selfMove(direction) end
function selfMoveTo(...) end
function selfTurn(direction) end
function selfFollow(creature) end
function getDistanceTo(creature) end
function doNpcSetCreatureFocus(creature) end
function getNpcParameter(key) end
function openShopWindow(shopWindow) end
function closeShopWindow() end
function doSellItem(item) end

storages = {}

CREATURE_ID_MIN = 0x10000000
ITEM_STACK_SIZE = 100

-- Constants: CONST_ANI
CONST_ANI_NONE = 0
CONST_ANI_SPEAR = 1
CONST_ANI_BOLT = 2
CONST_ANI_ARROW = 3
CONST_ANI_FIRE = 4
CONST_ANI_ENERGY = 5
CONST_ANI_POISONARROW = 6
CONST_ANI_BURSTARROW = 7
CONST_ANI_THROWINGSTAR = 8
CONST_ANI_THROWINGKNIFE = 9
CONST_ANI_SMALLSTONE = 10
CONST_ANI_DEATH = 11
CONST_ANI_LARGEROCK = 12
CONST_ANI_SNOWBALL = 13
CONST_ANI_POWERBOLT = 14
CONST_ANI_POISON = 15
CONST_ANI_INFERNALBOLT = 16
CONST_ANI_HUNTINGSPEAR = 17
CONST_ANI_ENCHANTEDSPEAR = 18
CONST_ANI_REDSTAR = 19
CONST_ANI_GREENSTAR = 20
CONST_ANI_ROYALSPEAR = 21
CONST_ANI_SNIPERARROW = 22
CONST_ANI_ONYXARROW = 23
CONST_ANI_PIERCINGBOLT = 24
CONST_ANI_WHIRLWINDSWORD = 25
CONST_ANI_WHIRLWINDAXE = 26
CONST_ANI_WHIRLWINDCLUB = 27
CONST_ANI_ETHEREALSPEAR = 28
CONST_ANI_ICE = 29
CONST_ANI_EARTH = 30
CONST_ANI_HOLY = 31
CONST_ANI_SUDDENDEATH = 32
CONST_ANI_FLASHARROW = 33
CONST_ANI_FLAMMINGARROW = 34
CONST_ANI_SHIVERARROW = 35
CONST_ANI_ENERGYBALL = 36
CONST_ANI_SMALLICE = 37
CONST_ANI_SMALLHOLY = 38
CONST_ANI_SMALLEARTH = 39
CONST_ANI_EARTHARROW = 40
CONST_ANI_EXPLOSION = 41
CONST_ANI_CAKE = 42
CONST_ANI_TARSALARROW = 44
CONST_ANI_VORTEXBOLT = 45
CONST_ANI_PRISMATICBOLT = 48
CONST_ANI_CRYSTALLINEARROW = 49
CONST_ANI_DRILLBOLT = 50
CONST_ANI_ENVENOMEDARROW = 51
CONST_ANI_GLOOTHSPEAR = 53
CONST_ANI_SIMPLEARROW = 54
CONST_ANI_LEAFSTAR = 56
CONST_ANI_DIAMONDARROW = 57
CONST_ANI_SPECTRALBOLT = 58
CONST_ANI_ROYALSTAR = 59
CONST_ANI_WEAPONTYPE = 254

---@alias MagicEffectConsts uint8
---| 0 # CONST_ME_NONE
---| 1 # CONST_ME_DRAWBLOOD
---| 2 # CONST_ME_LOSEENERGY
---| 3 # CONST_ME_POFF
---| 4 # CONST_ME_BLOCKHIT
---| 5 # CONST_ME_EXPLOSIONAREA
---| 6 # CONST_ME_EXPLOSIONHIT
---| 7 # CONST_ME_FIREAREA
---| 8 # CONST_ME_YELLOW_RINGS
---| 9 # CONST_ME_GREEN_RINGS
---| 10 # CONST_ME_HITAREA
---| 11 # CONST_ME_TELEPORT
---| 12 # CONST_ME_ENERGYHIT
---| 13 # CONST_ME_MAGIC_BLUE
---| 14 # CONST_ME_MAGIC_RED
---| 15 # CONST_ME_MAGIC_GREEN
---| 16 # CONST_ME_HITBYFIRE
---| 17 # CONST_ME_HITBYPOISON
---| 18 # CONST_ME_MORTAREA
---| 19 # CONST_ME_SOUND_GREEN
---| 20 # CONST_ME_SOUND_RED
---| 21 # CONST_ME_POISONAREA
---| 22 # CONST_ME_SOUND_YELLOW
---| 23 # CONST_ME_SOUND_PURPLE
---| 24 # CONST_ME_SOUND_BLUE
---| 25 # CONST_ME_SOUND_WHITE
---| 26 # CONST_ME_BUBBLES
---| 27 # CONST_ME_CRAPS
---| 28 # CONST_ME_GIFT_WRAPS
---| 29 # CONST_ME_FIREWORK_YELLOW
---| 30 # CONST_ME_FIREWORK_RED
---| 31 # CONST_ME_FIREWORK_BLUE
---| 32 # CONST_ME_STUN
---| 33 # CONST_ME_SLEEP
---| 34 # CONST_ME_WATERCREATURE
---| 35 # CONST_ME_GROUNDSHAKER
---| 36 # CONST_ME_HEARTS
---| 37 # CONST_ME_FIREATTACK
---| 38 # CONST_ME_ENERGYAREA
---| 39 # CONST_ME_SMALLCLOUDS
---| 40 # CONST_ME_HOLYDAMAGE
---| 41 # CONST_ME_BIGCLOUDS
---| 42 # CONST_ME_ICEAREA
---| 43 # CONST_ME_ICETORNADO
---| 44 # CONST_ME_ICEATTACK
---| 45 # CONST_ME_STONES
---| 46 # CONST_ME_SMALLPLANTS
---| 47 # CONST_ME_CARNIPHILA
---| 48 # CONST_ME_PURPLEENERGY
---| 49 # CONST_ME_YELLOWENERGY
---| 50 # CONST_ME_HOLYAREA
---| 51 # CONST_ME_BIGPLANTS
---| 52 # CONST_ME_CAKE
---| 53 # CONST_ME_GIANTICE
---| 54 # CONST_ME_WATERSPLASH
---| 55 # CONST_ME_PLANTATTACK
---| 56 # CONST_ME_TUTORIALARROW
---| 57 # CONST_ME_TUTORIALSQUARE
---| 58 # CONST_ME_MIRRORHORIZONTAL
---| 59 # CONST_ME_MIRRORVERTICAL
---| 60 # CONST_ME_SKULLHORIZONTAL
---| 61 # CONST_ME_SKULLVERTICAL
---| 62 # CONST_ME_ASSASSIN
---| 63 # CONST_ME_STEPSHORIZONTAL
---| 64 # CONST_ME_BLOODYSTEPS
---| 65 # CONST_ME_STEPSVERTICAL
---| 66 # CONST_ME_YALAHARIGHOST
---| 67 # CONST_ME_BATS
---| 68 # CONST_ME_SMOKE
---| 69 # CONST_ME_INSECTS
---| 70 # CONST_ME_DRAGONHEAD
---| 71 # CONST_ME_ORCSHAMAN
---| 72 # CONST_ME_ORCSHAMAN_FIRE
---| 73 # CONST_ME_THUNDER
---| 74 # CONST_ME_FERUMBRAS
---| 75 # CONST_ME_CONFETTI_HORIZONTAL
---| 76 # CONST_ME_CONFETTI_VERTICAL
---| 158 # CONST_ME_BLACKSMOKE
---| 167 # CONST_ME_REDSMOKE
---| 168 # CONST_ME_YELLOWSMOKE
---| 169 # CONST_ME_GREENSMOKE
---| 170 # CONST_ME_PURPLESMOKE
---| 171 # CONST_ME_EARLY_THUNDER
---| 172 # CONST_ME_RAGIAZ_BONECAPSULE
---| 173 # CONST_ME_CRITICAL_DAMAGE
---| 175 # CONST_ME_PLUNGING_FISH
---| 176 # CONST_ME_BLUECHAIN
---| 177 # CONST_ME_ORANGECHAIN
---| 178 # CONST_ME_GREENCHAIN
---| 179 # CONST_ME_PURPLECHAIN
---| 180 # CONST_ME_GREYCHAIN
---| 181 # CONST_ME_YELLOWCHAIN
---| 182 # CONST_ME_YELLOWSPARKLES
---| 184 # CONST_ME_FAEEXPLOSION
---| 185 # CONST_ME_FAECOMING
---| 186 # CONST_ME_FAEGOING
---| 188 # CONST_ME_BIGCLOUDSSINGLESPACE
---| 189 # CONST_ME_STONESSINGLESPACE
---| 191 # CONST_ME_BLUEGHOST
---| 193 # CONST_ME_POINTOFINTEREST
---| 194 # CONST_ME_MAPEFFECT
---| 195 # CONST_ME_PINKSPARK
---| 196 # CONST_ME_FIREWORK_GREEN
---| 197 # CONST_ME_FIREWORK_ORANGE
---| 198 # CONST_ME_FIREWORK_PURPLE
---| 199 # CONST_ME_FIREWORK_TURQUOISE
---| 201 # CONST_ME_THECUBE
---| 202 # CONST_ME_DRAWINK
---| 203 # CONST_ME_PRISMATICSPARKLES
---| 204 # CONST_ME_THAIAN
---| 205 # CONST_ME_THAIANGHOST
---| 206 # CONST_ME_GHOSTSMOKE
---| 208 # CONST_ME_FLOATINGBLOCK
---| 209 # CONST_ME_BLOCK
---| 210 # CONST_ME_ROOTING
---| 213 # CONST_ME_GHOSTLYSCRATCH
---| 214 # CONST_ME_GHOSTLYBITE
---| 215 # CONST_ME_BIGSCRATCHING
---| 216 # CONST_ME_SLASH
---| 217 # CONST_ME_BITE
---| 219 # CONST_ME_CHIVALRIOUSCHALLENGE
---| 220 # CONST_ME_DIVINEDAZZLE
---| 221 # CONST_ME_ELECTRICALSPARK
---| 222 # CONST_ME_PURPLETELEPORT
---| 223 # CONST_ME_REDTELEPORT
---| 224 # CONST_ME_ORANGETELEPORT
---| 225 # CONST_ME_GREYTELEPORT
---| 226 # CONST_ME_LIGHTBLUETELEPORT
---| 230 # CONST_ME_FATAL
---| 231 # CONST_ME_DODGE
---| 232 # CONST_ME_HOURGLASS
---| 233 # CONST_ME_FIREWORKSSTAR
---| 234 # CONST_ME_FIREWORKSCIRCLE
---| 235 # CONST_ME_FERUMBRAS_1
---| 236 # CONST_ME_GAZHARAGOTH
---| 237 # CONST_ME_MAD_MAGE
---| 238 # CONST_ME_HORESTIS
---| 239 # CONST_ME_DEVOVORGA
---| 240 # CONST_ME_FERUMBRAS_2
---| 241 # CONST_ME_FOAM

---@source ../src/const.h:23
CONST_ME_NONE = 0
---@source ../src/const.h:25
CONST_ME_DRAWBLOOD = 1
---@source ../src/const.h:26
CONST_ME_LOSEENERGY = 2
---@source ../src/const.h:27
CONST_ME_POFF = 3
---@source ../src/const.h:28
CONST_ME_BLOCKHIT = 4
---@source ../src/const.h:29
CONST_ME_EXPLOSIONAREA = 5
---@source ../src/const.h:30
CONST_ME_EXPLOSIONHIT = 6
---@source ../src/const.h:31
CONST_ME_FIREAREA = 7
---@source ../src/const.h:32
CONST_ME_YELLOW_RINGS = 8
---@source ../src/const.h:33
CONST_ME_GREEN_RINGS = 9
---@source ../src/const.h:34
CONST_ME_HITAREA = 10
---@source ../src/const.h:35
CONST_ME_TELEPORT = 11
---@source ../src/const.h:36
CONST_ME_ENERGYHIT = 12
---@source ../src/const.h:37
CONST_ME_MAGIC_BLUE = 13
---@source ../src/const.h:38
CONST_ME_MAGIC_RED = 14
---@source ../src/const.h:39
CONST_ME_MAGIC_GREEN = 15
---@source ../src/const.h:40
CONST_ME_HITBYFIRE = 16
---@source ../src/const.h:41
CONST_ME_HITBYPOISON = 17
---@source ../src/const.h:42
CONST_ME_MORTAREA = 18
---@source ../src/const.h:43
CONST_ME_SOUND_GREEN = 19
---@source ../src/const.h:44
CONST_ME_SOUND_RED = 20
---@source ../src/const.h:45
CONST_ME_POISONAREA = 21
---@source ../src/const.h:46
CONST_ME_SOUND_YELLOW = 22
---@source ../src/const.h:47
CONST_ME_SOUND_PURPLE = 23
---@source ../src/const.h:48
CONST_ME_SOUND_BLUE = 24
---@source ../src/const.h:49
CONST_ME_SOUND_WHITE = 25
---@source ../src/const.h:50
CONST_ME_BUBBLES = 26
---@source ../src/const.h:51
CONST_ME_CRAPS = 27
---@source ../src/const.h:52
CONST_ME_GIFT_WRAPS = 28
---@source ../src/const.h:53
CONST_ME_FIREWORK_YELLOW = 29
---@source ../src/const.h:54
CONST_ME_FIREWORK_RED = 30
---@source ../src/const.h:55
CONST_ME_FIREWORK_BLUE = 31
---@source ../src/const.h:56
CONST_ME_STUN = 32
---@source ../src/const.h:57
CONST_ME_SLEEP = 33
---@source ../src/const.h:58
CONST_ME_WATERCREATURE = 34
---@source ../src/const.h:59
CONST_ME_GROUNDSHAKER = 35
---@source ../src/const.h:60
CONST_ME_HEARTS = 36
---@source ../src/const.h:61
CONST_ME_FIREATTACK = 37
---@source ../src/const.h:62
CONST_ME_ENERGYAREA = 38
---@source ../src/const.h:63
CONST_ME_SMALLCLOUDS = 39
---@source ../src/const.h:64
CONST_ME_HOLYDAMAGE = 40
---@source ../src/const.h:65
CONST_ME_BIGCLOUDS = 41
---@source ../src/const.h:66
CONST_ME_ICEAREA = 42
---@source ../src/const.h:67
CONST_ME_ICETORNADO = 43
---@source ../src/const.h:68
CONST_ME_ICEATTACK = 44
---@source ../src/const.h:69
CONST_ME_STONES = 45
---@source ../src/const.h:70
CONST_ME_SMALLPLANTS = 46
---@source ../src/const.h:71
CONST_ME_CARNIPHILA = 47
---@source ../src/const.h:72
CONST_ME_PURPLEENERGY = 48
---@source ../src/const.h:73
CONST_ME_YELLOWENERGY = 49
---@source ../src/const.h:74
CONST_ME_HOLYAREA = 50
---@source ../src/const.h:75
CONST_ME_BIGPLANTS = 51
---@source ../src/const.h:76
CONST_ME_CAKE = 52
---@source ../src/const.h:77
CONST_ME_GIANTICE = 53
---@source ../src/const.h:78
CONST_ME_WATERSPLASH = 54
---@source ../src/const.h:79
CONST_ME_PLANTATTACK = 55
---@source ../src/const.h:80
CONST_ME_TUTORIALARROW = 56
---@source ../src/const.h:81
CONST_ME_TUTORIALSQUARE = 57
---@source ../src/const.h:82
CONST_ME_MIRRORHORIZONTAL = 58
---@source ../src/const.h:83
CONST_ME_MIRRORVERTICAL = 59
---@source ../src/const.h:84
CONST_ME_SKULLHORIZONTAL = 60
---@source ../src/const.h:85
CONST_ME_SKULLVERTICAL = 61
---@source ../src/const.h:86
CONST_ME_ASSASSIN = 62
---@source ../src/const.h:87
CONST_ME_STEPSHORIZONTAL = 63
---@source ../src/const.h:88
CONST_ME_BLOODYSTEPS = 64
---@source ../src/const.h:89
CONST_ME_STEPSVERTICAL = 65
---@source ../src/const.h:90
CONST_ME_YALAHARIGHOST = 66
---@source ../src/const.h:91
CONST_ME_BATS = 67
---@source ../src/const.h:92
CONST_ME_SMOKE = 68
---@source ../src/const.h:93
CONST_ME_INSECTS = 69
---@source ../src/const.h:94
CONST_ME_DRAGONHEAD = 70
---@source ../src/const.h:95
CONST_ME_ORCSHAMAN = 71
---@source ../src/const.h:96
CONST_ME_ORCSHAMAN_FIRE = 72
---@source ../src/const.h:97
CONST_ME_THUNDER = 73
---@source ../src/const.h:98
CONST_ME_FERUMBRAS = 74
---@source ../src/const.h:99
CONST_ME_CONFETTI_HORIZONTAL = 75
---@source ../src/const.h:100
CONST_ME_CONFETTI_VERTICAL = 76
---@source ../src/const.h:101
-- 77-157 are empty
---@source ../src/const.h:102
CONST_ME_BLACKSMOKE = 158
---@source ../src/const.h:103
-- 159-166 are empty
---@source ../src/const.h:104
CONST_ME_REDSMOKE = 167
---@source ../src/const.h:105
CONST_ME_YELLOWSMOKE = 168
---@source ../src/const.h:106
CONST_ME_GREENSMOKE = 169
---@source ../src/const.h:107
CONST_ME_PURPLESMOKE = 170
---@source ../src/const.h:108
CONST_ME_EARLY_THUNDER = 171
---@source ../src/const.h:109
CONST_ME_RAGIAZ_BONECAPSULE = 172
---@source ../src/const.h:110
CONST_ME_CRITICAL_DAMAGE = 173
---@source ../src/const.h:111
-- 174 is empty
---@source ../src/const.h:112
CONST_ME_PLUNGING_FISH = 175
---@source ../src/const.h:113
CONST_ME_BLUECHAIN = 176
---@source ../src/const.h:114
CONST_ME_ORANGECHAIN = 177
---@source ../src/const.h:115
CONST_ME_GREENCHAIN = 178
---@source ../src/const.h:116
CONST_ME_PURPLECHAIN = 179
---@source ../src/const.h:117
CONST_ME_GREYCHAIN = 180
---@source ../src/const.h:118
CONST_ME_YELLOWCHAIN = 181
---@source ../src/const.h:119
CONST_ME_YELLOWSPARKLES = 182
---@source ../src/const.h:120
-- 183 is empty
---@source ../src/const.h:121
CONST_ME_FAEEXPLOSION = 184
---@source ../src/const.h:122
CONST_ME_FAECOMING = 185
---@source ../src/const.h:123
CONST_ME_FAEGOING = 186
---@source ../src/const.h:124
-- 187 is empty
---@source ../src/const.h:125
CONST_ME_BIGCLOUDSSINGLESPACE = 188
---@source ../src/const.h:126
CONST_ME_STONESSINGLESPACE = 189
---@source ../src/const.h:127
-- 190 is empty
---@source ../src/const.h:128
CONST_ME_BLUEGHOST = 191
---@source ../src/const.h:129
-- 192 is empty
---@source ../src/const.h:130
CONST_ME_POINTOFINTEREST = 193
---@source ../src/const.h:131
CONST_ME_MAPEFFECT = 194
---@source ../src/const.h:132
CONST_ME_PINKSPARK = 195
---@source ../src/const.h:133
CONST_ME_FIREWORK_GREEN = 196
---@source ../src/const.h:134
CONST_ME_FIREWORK_ORANGE = 197
---@source ../src/const.h:135
CONST_ME_FIREWORK_PURPLE = 198
---@source ../src/const.h:136
CONST_ME_FIREWORK_TURQUOISE = 199
---@source ../src/const.h:137
-- 200 is empty
---@source ../src/const.h:138
CONST_ME_THECUBE = 201
---@source ../src/const.h:139
CONST_ME_DRAWINK = 202
---@source ../src/const.h:140
CONST_ME_PRISMATICSPARKLES = 203
---@source ../src/const.h:141
CONST_ME_THAIAN = 204
---@source ../src/const.h:142
CONST_ME_THAIANGHOST = 205
---@source ../src/const.h:143
CONST_ME_GHOSTSMOKE = 206
---@source ../src/const.h:144
-- 207 is empty
---@source ../src/const.h:145
CONST_ME_FLOATINGBLOCK = 208
---@source ../src/const.h:146
CONST_ME_BLOCK = 209
---@source ../src/const.h:147
CONST_ME_ROOTING = 210
---@source ../src/const.h:148
-- 211-212 were removed from the client
---@source ../src/const.h:149
CONST_ME_GHOSTLYSCRATCH = 213
---@source ../src/const.h:150
CONST_ME_GHOSTLYBITE = 214
---@source ../src/const.h:151
CONST_ME_BIGSCRATCHING = 215
---@source ../src/const.h:152
CONST_ME_SLASH = 216
---@source ../src/const.h:153
CONST_ME_BITE = 217
---@source ../src/const.h:154
-- 218 is empty
---@source ../src/const.h:155
CONST_ME_CHIVALRIOUSCHALLENGE = 219
---@source ../src/const.h:156
CONST_ME_DIVINEDAZZLE = 220
---@source ../src/const.h:157
CONST_ME_ELECTRICALSPARK = 221
---@source ../src/const.h:158
CONST_ME_PURPLETELEPORT = 222
---@source ../src/const.h:159
CONST_ME_REDTELEPORT = 223
---@source ../src/const.h:160
CONST_ME_ORANGETELEPORT = 224
---@source ../src/const.h:161
CONST_ME_GREYTELEPORT = 225
---@source ../src/const.h:162
CONST_ME_LIGHTBLUETELEPORT = 226
---@source ../src/const.h:163
-- 227-229 are empty
---@source ../src/const.h:164
CONST_ME_FATAL = 230
---@source ../src/const.h:165
CONST_ME_DODGE = 231
---@source ../src/const.h:166
CONST_ME_HOURGLASS = 232
---@source ../src/const.h:167
CONST_ME_FIREWORKSSTAR = 233
---@source ../src/const.h:168
CONST_ME_FIREWORKSCIRCLE = 234
---@source ../src/const.h:169
CONST_ME_FERUMBRAS_1 = 235
---@source ../src/const.h:170
CONST_ME_GAZHARAGOTH = 236
---@source ../src/const.h:171
CONST_ME_MAD_MAGE = 237
---@source ../src/const.h:172
CONST_ME_HORESTIS = 238
---@source ../src/const.h:173
CONST_ME_DEVOVORGA = 239
---@source ../src/const.h:174
CONST_ME_FERUMBRAS_2 = 240
---@source ../src/const.h:175
CONST_ME_FOAM = 241

-- Constants: CONST_PROP
CONST_PROP_BLOCKSOLID = 0
CONST_PROP_HASHEIGHT = 1
CONST_PROP_BLOCKPROJECTILE = 2
CONST_PROP_BLOCKPATH = 3
CONST_PROP_ISVERTICAL = 4
CONST_PROP_ISHORIZONTAL = 5
CONST_PROP_MOVEABLE = 6
CONST_PROP_IMMOVABLEBLOCKSOLID = 7
CONST_PROP_IMMOVABLEBLOCKPATH = 8
CONST_PROP_IMMOVABLENOFIELDBLOCKPATH = 9
CONST_PROP_NOFIELDBLOCKPATH = 10
CONST_PROP_SUPPORTHANGABLE = 11


---@alias PlayerEquipmentSlots uint8
---| 0 # CONST_SLOT_WHEREEVER
---| 1 # CONST_SLOT_HEAD
---| 2 # CONST_SLOT_NECKLACE
---| 3 # CONST_SLOT_BACKPACK
---| 4 # CONST_SLOT_ARMOR
---| 5 # CONST_SLOT_RIGHT
---| 6 # CONST_SLOT_LEFT
---| 7 # CONST_SLOT_LEGS
---| 8 # CONST_SLOT_FEET
---| 9 # CONST_SLOT_RING
---| 10 # CONST_SLOT_AMMO
---| 11 # CONST_SLOT_STORE_INBOX
---| 1 # CONST_SLOT_FIRST
---| 10 # CONST_SLOT_LAST

-- Constants: CONST_SLOT

---@source ../src/creature.h:27
CONST_SLOT_WHEREEVER = 0
---@source ../src/creature.h:28
CONST_SLOT_HEAD = 1
---@source ../src/creature.h:29
CONST_SLOT_NECKLACE = 2
---@source ../src/creature.h:30
CONST_SLOT_BACKPACK = 3
---@source ../src/creature.h:31
CONST_SLOT_ARMOR = 4
---@source ../src/creature.h:32
CONST_SLOT_RIGHT = 5
---@source ../src/creature.h:33
CONST_SLOT_LEFT = 6
---@source ../src/creature.h:34
CONST_SLOT_LEGS = 7
---@source ../src/creature.h:35
CONST_SLOT_FEET = 8
---@source ../src/creature.h:36
CONST_SLOT_RING = 9
---@source ../src/creature.h:37
CONST_SLOT_AMMO = 10
---@source ../src/creature.h:38
CONST_SLOT_STORE_INBOX = 11
---@source ../src/creature.h:40
CONST_SLOT_FIRST = CONST_SLOT_HEAD
---@source ../src/creature.h:41
CONST_SLOT_LAST = CONST_SLOT_AMMO

TILESTATE_NONE = 0
TILESTATE_FLOORCHANGE_DOWN = 1 * 2 ^ 0
TILESTATE_FLOORCHANGE_NORTH = 1 * 2 ^ 1
TILESTATE_FLOORCHANGE_SOUTH = 1 * 2 ^ 2
TILESTATE_FLOORCHANGE_EAST = 1 * 2 ^ 3
TILESTATE_FLOORCHANGE_WEST = 1 * 2 ^ 4
TILESTATE_FLOORCHANGE_SOUTH_ALT = 1 * 2 ^ 5
TILESTATE_FLOORCHANGE_EAST_ALT = 1 * 2 ^ 6
TILESTATE_PROTECTIONZONE = 1 * 2 ^ 7
TILESTATE_NOPVPZONE = 1 * 2 ^ 8
TILESTATE_NOLOGOUT = 1 * 2 ^ 9
TILESTATE_PVPZONE = 1 * 2 ^ 10
TILESTATE_TELEPORT = 1 * 2 ^ 11
TILESTATE_MAGICFIELD = 1 * 2 ^ 12
TILESTATE_MAILBOX = 1 * 2 ^ 13
TILESTATE_TRASHHOLDER = 1 * 2 ^ 14
TILESTATE_BED = 1 * 2 ^ 15
TILESTATE_DEPOT = 1 * 2 ^ 16
TILESTATE_BLOCKSOLID = 1 * 2 ^ 17
TILESTATE_BLOCKPATH = 1 * 2 ^ 18
TILESTATE_IMMOVABLEBLOCKSOLID = 1 * 2 ^ 19
TILESTATE_IMMOVABLEBLOCKPATH = 1 * 2 ^ 20
TILESTATE_IMMOVABLENOFIELDBLOCKPATH = 1 * 2 ^ 21
TILESTATE_NOFIELDBLOCKPATH = 1 * 2 ^ 22
TILESTATE_SUPPORTS_HANGABLE = 1 * 2 ^ 23

TILESTATE_FLOORCHANGE =
	TILESTATE_FLOORCHANGE_DOWN or TILESTATE_FLOORCHANGE_NORTH or
		TILESTATE_FLOORCHANGE_SOUTH or TILESTATE_FLOORCHANGE_EAST or
		TILESTATE_FLOORCHANGE_WEST or TILESTATE_FLOORCHANGE_SOUTH_ALT or
		TILESTATE_FLOORCHANGE_EAST_ALT

SPELL_UNDEFINED = 0
SPELL_INSTANT = 1
SPELL_RUNE = 2

COMBAT_PARAM_TYPE = 0
COMBAT_PARAM_EFFECT = 1
COMBAT_PARAM_DISTANCEEFFECT = 2
COMBAT_PARAM_BLOCKSHIELD = 3
COMBAT_PARAM_BLOCKARMOR = 4
COMBAT_PARAM_TARGETCASTERORTOPMOST = 5
COMBAT_PARAM_CREATEITEM = 6
COMBAT_PARAM_AGGRESSIVE = 7
COMBAT_PARAM_DISPEL = 8
COMBAT_PARAM_USECHARGES = 9

COMBAT_NONE = 0

COMBAT_PHYSICALDAMAGE = 1 * 2 ^ 0
COMBAT_ENERGYDAMAGE = 1 * 2 ^ 1
COMBAT_EARTHDAMAGE = 1 * 2 ^ 2
COMBAT_FIREDAMAGE = 1 * 2 ^ 3
COMBAT_UNDEFINEDDAMAGE = 1 * 2 ^ 4
COMBAT_LIFEDRAIN = 1 * 2 ^ 5
COMBAT_MANADRAIN = 1 * 2 ^ 6
COMBAT_HEALING = 1 * 2 ^ 7
COMBAT_DROWNDAMAGE = 1 * 2 ^ 8
COMBAT_ICEDAMAGE = 1 * 2 ^ 9
COMBAT_HOLYDAMAGE = 1 * 2 ^ 10
COMBAT_DEATHDAMAGE = 1 * 2 ^ 11

COMBAT_COUNT = 12

COMBAT_FORMULA_UNDEFINED = 0
COMBAT_FORMULA_LEVELMAGIC = 1
COMBAT_FORMULA_SKILL = 2
COMBAT_FORMULA_DAMAGE = 3

---@alias AccountTypes uint8
---| 1 # ACCOUNT_TYPE_NORMAL
---| 2 # ACCOUNT_TYPE_TUTOR
---| 3 # ACCOUNT_TYPE_SENIORTUTOR
---| 4 # ACCOUNT_TYPE_GAMEMASTER
---| 5 # ACCOUNT_TYPE_COMMUNITYMANAGER
---| 6 # ACCOUNT_TYPE_GOD
---@source ../src/const.h:172
ACCOUNT_TYPE_NORMAL = 1
---@source ../src/const.h:173
ACCOUNT_TYPE_TUTOR = 2
---@source ../src/const.h:174
ACCOUNT_TYPE_SENIORTUTOR = 3
---@source ../src/const.h:175
ACCOUNT_TYPE_GAMEMASTER = 4
---@source ../src/const.h:176
ACCOUNT_TYPE_COMMUNITYMANAGER = 5
---@source ../src/const.h:177
ACCOUNT_TYPE_GOD = 6

---@enum CallBackParam
CallBackParam = {
	LEVELMAGICVALUE = 0,
	SKILLVALUE = 1,
	TARGETTILE = 2,
	TARGETCREATURE = 3
}

CALLBACK_PARAM_LEVELMAGICVALUE = 0
CALLBACK_PARAM_SKILLVALUE = 1
CALLBACK_PARAM_TARGETTILE = 2
CALLBACK_PARAM_TARGETCREATURE = 3

---@enum ExperienceRateType
ExperienceRateType = {BASE = 1, LOW_LEVEL = 2, BONUS = 3, STAMINA = 4}

GUEST_LIST = 0x100
SUBOWNER_LIST = 0x101

PODIUM_SHOW_PLATFORM = 0 -- show the platform below the outfit
PODIUM_SHOW_OUTFIT = 1   -- show outfit
PODIUM_SHOW_MOUNT = 2     -- show mount

DIRECTION_NORTH = 0
DIRECTION_EAST = 1
DIRECTION_SOUTH = 2
DIRECTION_WEST = 3

DIRECTION_DIAGONAL_MASK = 4
DIRECTION_SOUTHWEST = DIRECTION_DIAGONAL_MASK or 0
DIRECTION_SOUTHEAST = DIRECTION_DIAGONAL_MASK or 1
DIRECTION_NORTHWEST = DIRECTION_DIAGONAL_MASK or 2
DIRECTION_NORTHEAST = DIRECTION_DIAGONAL_MASK or 3

DIRECTION_LAST = DIRECTION_NORTHEAST
DIRECTION_NONE = 8

---@alias Skulls number
---| 0 # SKULL_NONE
---| 1 # SKULL_YELLOW
---| 2 # SKULL_GREEN
---| 3 # SKULL_WHITE
---| 4 # SKULL_RED
---| 5 # SKULL_BLACK
---| 6 # SKULL_ORANGE
---@source ../src/const.h:485
SKULL_NONE = 0
---@source ../src/const.h:486
SKULL_YELLOW = 1
---@source ../src/const.h:487
SKULL_GREEN = 2
---@source ../src/const.h:488
SKULL_WHITE = 3
---@source ../src/const.h:489
SKULL_RED = 4
---@source ../src/const.h:490
SKULL_BLACK = 5
---@source ../src/const.h:491
SKULL_ORANGE = 6

---@alias MessageTypes uint8
---| 17 # MESSAGE_STATUS_DEFAULT -- White, bottom + console
---| 18 # MESSAGE_STATUS_WARNING -- Red, over player + console
---| 19 # MESSAGE_EVENT_ADVANCE -- White, over player + console
---| 20 # MESSAGE_STATUS_WARNING2 -- Red, over player + console
---| 21 # MESSAGE_STATUS_SMALL -- White, bottom of the screen
---| 22 # MESSAGE_INFO_DESCR -- Green, over player + console
---| 23 # MESSAGE_DAMAGE_DEALT
---| 24 # MESSAGE_DAMAGE_RECEIVED
---| 25 # MESSAGE_HEALED
---| 26 # MESSAGE_EXPERIENCE
---| 27 # MESSAGE_DAMAGE_OTHERS
---| 28 # MESSAGE_HEALED_OTHERS
---| 29 # MESSAGE_EXPERIENCE_OTHERS
---| 30 # MESSAGE_EVENT_DEFAULT -- White, bottom + console
---| 31 # MESSAGE_LOOT -- White, over player + console, supports colors as {text|itemClientId}
---| 32 # MESSAGE_TRADE -- Green, over player + console
---| 33 # MESSAGE_GUILD
---| 34 # MESSAGE_PARTY_MANAGEMENT
---| 35 # MESSAGE_PARTY
---| 38 # MESSAGE_REPORT -- White, over player + console
---| 39 # MESSAGE_HOTKEY_PRESSED -- Green, over player + console
---| 42 # MESSAGE_MARKET -- Window "Market Message" + "Ok" button
---| 44 # MESSAGE_BEYOND_LAST -- White, console only
---| 45 # MESSAGE_TOURNAMENT_INFO -- Window "Tournament" + "Ok" button
---| 48 # MESSAGE_ATTENTION -- White, console only
---| 49 # MESSAGE_BOOSTED_CREATURE -- White, console only
---| 50 # MESSAGE_OFFLINE_TRAINING -- White, over player + console
---| 51 # MESSAGE_TRANSACTION -- White, console only
---@source ../src/const.h:270
MESSAGE_STATUS_DEFAULT = 17  -- White, bottom + console
---@source ../src/const.h:271
MESSAGE_STATUS_WARNING = 18  -- Red, over player + console
---@source ../src/const.h:272
MESSAGE_EVENT_ADVANCE = 19   -- White, over player + console
---@source ../src/const.h:273
MESSAGE_STATUS_WARNING2 = 20 -- Red, over player + console
---@source ../src/const.h:274
MESSAGE_STATUS_SMALL = 21    -- White, bottom of the screen
---@source ../src/const.h:275
MESSAGE_INFO_DESCR = 22      -- Green, over player + console
---@source ../src/const.h:278
MESSAGE_DAMAGE_DEALT = 23
---@source ../src/const.h:279
MESSAGE_DAMAGE_RECEIVED = 24
---@source ../src/const.h:280
MESSAGE_HEALED = 25
---@source ../src/const.h:281
MESSAGE_EXPERIENCE = 26
---@source ../src/const.h:282
MESSAGE_DAMAGE_OTHERS = 27
---@source ../src/const.h:283
MESSAGE_HEALED_OTHERS = 28
---@source ../src/const.h:284
MESSAGE_EXPERIENCE_OTHERS = 29
---@source ../src/const.h:286
MESSAGE_EVENT_DEFAULT = 30 -- White, bottom + console
---@source ../src/const.h:287
MESSAGE_LOOT = 31          -- White, over player + console, supports colors as {text|itemClientId}
---@source ../src/const.h:288
MESSAGE_TRADE = 32         -- Green, over player + console
---@source ../src/const.h:291
MESSAGE_GUILD = 33
---@source ../src/const.h:292
MESSAGE_PARTY_MANAGEMENT = 34
---@source ../src/const.h:293
MESSAGE_PARTY = 35
---@source ../src/const.h:295
MESSAGE_REPORT = 38         -- White, over player + conosle
---@source ../src/const.h:296
MESSAGE_HOTKEY_PRESSED = 39 -- Green, over player + console
---@source ../src/const.h:297
-- MESSAGE_TUTORIAL_HINT = 40 -- not working (?)
---@source ../src/const.h:298
-- MESSAGE_THANK_YOU = 41 -- not working (?)
---@source ../src/const.h:299
MESSAGE_MARKET = 42 -- Window "Market Message" + "Ok" button
---@source ../src/const.h:300
-- MESSAGE_MANA = 43 -- not working (?)
---@source ../src/const.h:301
MESSAGE_BEYOND_LAST = 44     -- White, console only
---@source ../src/const.h:302
MESSAGE_TOURNAMENT_INFO = 45 -- Window "Tournament" + "Ok" button
---@source ../src/const.h:303
-- unused 46?
---@source ../src/const.h:304
-- unused 47?
---@source ../src/const.h:305
MESSAGE_ATTENTION = 48        -- White, console only
---@source ../src/const.h:306
MESSAGE_BOOSTED_CREATURE = 49 -- White, console only
---@source ../src/const.h:307
MESSAGE_OFFLINE_TRAINING = 50 -- White, over player + console
---@source ../src/const.h:308
MESSAGE_TRANSACTION = 51      -- White, console only

---@source ../src/cylinder.h:13
INDEX_WHEREEVER = -1
---@source ../src/luascript.cpp:2253
VIRTUAL_PARENT = true

---@alias CylinderFlag number
---@source ../src/cylinder.h:17
FLAG_NOLIMIT = 1 * 2 ^ 0             -- Bypass limits like capacity/container limits, blocking items/creatures etc.
---@source ../src/cylinder.h:18
FLAG_IGNOREBLOCKITEM = 1 * 2 ^ 1     -- Bypass movable blocking item checks
---@source ../src/cylinder.h:19
FLAG_IGNOREBLOCKCREATURE = 1 * 2 ^ 2 -- Bypass creature checks
---@source ../src/cylinder.h:20
FLAG_CHILDISOWNER = 1 * 2 ^ 3        -- Used by containers to query capacity of the carrier (player)
---@source ../src/cylinder.h:21
FLAG_PATHFINDING = 1 * 2 ^ 4         -- An additional check is done for floor changing/teleport items
---@source ../src/cylinder.h:21
FLAG_IGNOREFIELDDAMAGE = 1 * 2 ^ 5   -- Bypass field damage checks
---@source ../src/cylinder.h:22
FLAG_IGNORENOTMOVEABLE = 1 * 2 ^ 6   -- Bypass check for mobility
---@source ../src/cylinder.h:23
FLAG_IGNOREAUTOSTACK = 1 * 2 ^ 7     -- queryDestination will not try to stack items together

---@source src/enum.h:414
---@alias ReturnValues number
---| 0 # RETURNVALUE_NOERROR
---| 1 # RETURNVALUE_NOTPOSSIBLE
---| 2 # RETURNVALUE_NOTENOUGHROOM
---| 3 # RETURNVALUE_PLAYERISPZLOCKED
---| 4 # RETURNVALUE_PLAYERISNOTINVITED
---| 5 # RETURNVALUE_CANNOTTHROW
---| 6 # RETURNVALUE_THEREISNOWAY
---| 7 # RETURNVALUE_DESTINATIONOUTOFREACH
---| 8 # RETURNVALUE_CREATUREBLOCK
---| 9 # RETURNVALUE_NOTMOVEABLE
---| 10 # RETURNVALUE_DROPTWOHANDEDITEM
---| 11 # RETURNVALUE_BOTHHANDSNEEDTOBEFREE
---| 12 # RETURNVALUE_CANONLYUSEONEWEAPON
---| 13 # RETURNVALUE_NEEDEXCHANGE
---| 14 # RETURNVALUE_CANNOTBEDRESSED
---| 15 # RETURNVALUE_PUTTHISOBJECTINYOURHAND
---| 16 # RETURNVALUE_PUTTHISOBJECTINBOTHHANDS
---| 17 # RETURNVALUE_TOOFARAWAY
---| 18 # RETURNVALUE_FIRSTGODOWNSTAIRS
---| 19 # RETURNVALUE_FIRSTGOUPSTAIRS
---| 20 # RETURNVALUE_CONTAINERNOTENOUGHROOM
---| 21 # RETURNVALUE_NOTENOUGHCAPACITY
---| 22 # RETURNVALUE_CANNOTPICKUP
---| 23 # RETURNVALUE_THISISIMPOSSIBLE
---| 24 # RETURNVALUE_DEPOTISFULL
---| 25 # RETURNVALUE_CREATUREDOESNOTEXIST
---| 26 # RETURNVALUE_CANNOTUSETHISOBJECT
---| 27 # RETURNVALUE_PLAYERWITHTHISNAMEISNOTONLINE
---| 28 # RETURNVALUE_NOTREQUIREDLEVELTOUSERUNE
---| 29 # RETURNVALUE_YOUAREALREADYTRADING
---| 30 # RETURNVALUE_THISPLAYERISALREADYTRADING
---| 31 # RETURNVALUE_YOUMAYNOTLOGOUTDURINGAFIGHT
---| 32 # RETURNVALUE_DIRECTPLAYERSHOOT
---| 33 # RETURNVALUE_NOTENOUGHLEVEL
---| 34 # RETURNVALUE_NOTENOUGHMAGICLEVEL
---| 35 # RETURNVALUE_NOTENOUGHMANA
---| 36 # RETURNVALUE_NOTENOUGHSOUL
---| 37 # RETURNVALUE_YOUAREEXHAUSTED
---| 38 # RETURNVALUE_YOUCANNOTUSEOBJECTSTHATFAST
---| 39 # RETURNVALUE_PLAYERISNOTREACHABLE
---| 40 # RETURNVALUE_CANONLYUSETHISRUNEONCREATURES
---| 41 # RETURNVALUE_ACTIONNOTPERMITTEDINPROTECTIONZONE
---| 42 # RETURNVALUE_YOUMAYNOTATTACKTHISPLAYER
---| 43 # RETURNVALUE_YOUMAYNOTATTACKAPERSONINPROTECTIONZONE
---| 44 # RETURNVALUE_YOUMAYNOTATTACKAPERSONWHILEINPROTECTIONZONE
---| 45 # RETURNVALUE_YOUMAYNOTATTACKTHISCREATURE
---| 46 # RETURNVALUE_YOUCANONLYUSEITONCREATURES
---| 47 # RETURNVALUE_CREATUREISNOTREACHABLE
---| 48 # RETURNVALUE_TURNSECUREMODETOATTACKUNMARKEDPLAYERS
---| 49 # RETURNVALUE_YOUNEEDPREMIUMACCOUNT
---| 50 # RETURNVALUE_YOUNEEDTOLEARNTHISSPELL
---| 51 # RETURNVALUE_YOURVOCATIONCANNOTUSETHISSPELL
---| 52 # RETURNVALUE_YOUNEEDAWEAPONTOUSETHISSPELL
---| 53 # RETURNVALUE_PLAYERISPZLOCKEDLEAVEPVPZONE
---| 54 # RETURNVALUE_PLAYERISPZLOCKEDENTERPVPZONE
---| 55 # RETURNVALUE_ACTIONNOTPERMITTEDINANOPVPZONE
---| 56 # RETURNVALUE_YOUCANNOTLOGOUTHERE
---| 57 # RETURNVALUE_YOUNEEDAMAGICITEMTOCASTSPELL
---| 58 # RETURNVALUE_NAMEISTOOAMBIGUOUS
---| 59 # RETURNVALUE_CANONLYUSEONESHIELD
---| 60 # RETURNVALUE_NOPARTYMEMBERSINRANGE
---| 61 # RETURNVALUE_YOUARENOTTHEOWNER
---| 62 # RETURNVALUE_TRADEPLAYERFARAWAY
---| 63 # RETURNVALUE_YOUDONTOWNTHISHOUSE
---| 64 # RETURNVALUE_TRADEPLAYERALREADYOWNSAHOUSE
---| 65 # RETURNVALUE_TRADEPLAYERHIGHESTBIDDER
---| 66 # RETURNVALUE_YOUCANNOTTRADETHISHOUSE
---| 67 # RETURNVALUE_YOUDONTHAVEREQUIREDPROFESSION
---| 68 # RETURNVALUE_CANNOTMOVEITEMISNOTSTOREITEM
---| 69 # RETURNVALUE_ITEMCANNOTBEMOVEDTHERE
---| 70 # RETURNVALUE_YOUCANNOTUSETHISBED
---| 71 # RETURNVALUE_QUIVERAMMOONLY

---@source ../src/enums.h:414
RETURNVALUE_NOERROR = 0
---@source ../src/enums.h:415
RETURNVALUE_NOTPOSSIBLE = 1
---@source ../src/enums.h:416
RETURNVALUE_NOTENOUGHROOM = 2
---@source ../src/enums.h:417
RETURNVALUE_PLAYERISPZLOCKED = 3
---@source ../src/enums.h:418
RETURNVALUE_PLAYERISNOTINVITED = 4
---@source ../src/enums.h:419
RETURNVALUE_CANNOTTHROW = 5
---@source ../src/enums.h:420
RETURNVALUE_THEREISNOWAY = 6
---@source ../src/enums.h:421
RETURNVALUE_DESTINATIONOUTOFREACH = 7
---@source ../src/enums.h:422
RETURNVALUE_CREATUREBLOCK = 8
---@source ../src/enums.h:423
RETURNVALUE_NOTMOVEABLE = 9
---@source ../src/enums.h:424
RETURNVALUE_DROPTWOHANDEDITEM = 10
---@source ../src/enums.h:425
RETURNVALUE_BOTHHANDSNEEDTOBEFREE = 11
---@source ../src/enums.h:426
RETURNVALUE_CANONLYUSEONEWEAPON = 12
---@source ../src/enums.h:427
RETURNVALUE_NEEDEXCHANGE = 13
---@source ../src/enums.h:428
RETURNVALUE_CANNOTBEDRESSED = 14
---@source ../src/enums.h:429
RETURNVALUE_PUTTHISOBJECTINYOURHAND = 15
---@source ../src/enums.h:430
RETURNVALUE_PUTTHISOBJECTINBOTHHANDS = 16
---@source ../src/enums.h:431
RETURNVALUE_TOOFARAWAY = 17
---@source ../src/enums.h:432
RETURNVALUE_FIRSTGODOWNSTAIRS = 18
---@source ../src/enums.h:433
RETURNVALUE_FIRSTGOUPSTAIRS = 19
---@source ../src/enums.h:434
RETURNVALUE_CONTAINERNOTENOUGHROOM = 20
---@source ../src/enums.h:435
RETURNVALUE_NOTENOUGHCAPACITY = 21
---@source ../src/enums.h:436
RETURNVALUE_CANNOTPICKUP = 22
---@source ../src/enums.h:437
RETURNVALUE_THISISIMPOSSIBLE = 23
---@source ../src/enums.h:438
RETURNVALUE_DEPOTISFULL = 24
---@source ../src/enums.h:439
RETURNVALUE_CREATUREDOESNOTEXIST = 25
---@source ../src/enums.h:440
RETURNVALUE_CANNOTUSETHISOBJECT = 26
---@source ../src/enums.h:441
RETURNVALUE_PLAYERWITHTHISNAMEISNOTONLINE = 27
---@source ../src/enums.h:442
RETURNVALUE_NOTREQUIREDLEVELTOUSERUNE = 28
---@source ../src/enums.h:443
RETURNVALUE_YOUAREALREADYTRADING = 29
---@source ../src/enums.h:444
RETURNVALUE_THISPLAYERISALREADYTRADING = 30
---@source ../src/enums.h:445
RETURNVALUE_YOUMAYNOTLOGOUTDURINGAFIGHT = 31
---@source ../src/enums.h:446
RETURNVALUE_DIRECTPLAYERSHOOT = 32
---@source ../src/enums.h:447
RETURNVALUE_NOTENOUGHLEVEL = 33
---@source ../src/enums.h:448
RETURNVALUE_NOTENOUGHMAGICLEVEL = 34
---@source ../src/enums.h:449
RETURNVALUE_NOTENOUGHMANA = 35
---@source ../src/enums.h:450
RETURNVALUE_NOTENOUGHSOUL = 36
---@source ../src/enums.h:451
RETURNVALUE_YOUAREEXHAUSTED = 37
---@source ../src/enums.h:452
RETURNVALUE_YOUCANNOTUSEOBJECTSTHATFAST = 38
---@source ../src/enums.h:453
RETURNVALUE_PLAYERISNOTREACHABLE = 39
---@source ../src/enums.h:454
RETURNVALUE_CANONLYUSETHISRUNEONCREATURES = 40
---@source ../src/enums.h:455
RETURNVALUE_ACTIONNOTPERMITTEDINPROTECTIONZONE = 41
---@source ../src/enums.h:456
RETURNVALUE_YOUMAYNOTATTACKTHISPLAYER = 42
---@source ../src/enums.h:457
RETURNVALUE_YOUMAYNOTATTACKAPERSONINPROTECTIONZONE = 43
---@source ../src/enums.h:458
RETURNVALUE_YOUMAYNOTATTACKAPERSONWHILEINPROTECTIONZONE = 44
---@source ../src/enums.h:459
RETURNVALUE_YOUMAYNOTATTACKTHISCREATURE = 45
---@source ../src/enums.h:460
RETURNVALUE_YOUCANONLYUSEITONCREATURES = 46
---@source ../src/enums.h:461
RETURNVALUE_CREATUREISNOTREACHABLE = 47
---@source ../src/enums.h:462
RETURNVALUE_TURNSECUREMODETOATTACKUNMARKEDPLAYERS = 48
---@source ../src/enums.h:463
RETURNVALUE_YOUNEEDPREMIUMACCOUNT = 49
---@source ../src/enums.h:464
RETURNVALUE_YOUNEEDTOLEARNTHISSPELL = 50
---@source ../src/enums.h:465
RETURNVALUE_YOURVOCATIONCANNOTUSETHISSPELL = 51
---@source ../src/enums.h:466
RETURNVALUE_YOUNEEDAWEAPONTOUSETHISSPELL = 52
---@source ../src/enums.h:467
RETURNVALUE_PLAYERISPZLOCKEDLEAVEPVPZONE = 53
---@source ../src/enums.h:468
RETURNVALUE_PLAYERISPZLOCKEDENTERPVPZONE = 54
---@source ../src/enums.h:469
RETURNVALUE_ACTIONNOTPERMITTEDINANOPVPZONE = 55
---@source ../src/enums.h:470
RETURNVALUE_YOUCANNOTLOGOUTHERE = 56
---@source ../src/enums.h:471
RETURNVALUE_YOUNEEDAMAGICITEMTOCASTSPELL = 57
---@source ../src/enums.h:472
RETURNVALUE_NAMEISTOOAMBIGUOUS = 58
---@source ../src/enums.h:473
RETURNVALUE_CANONLYUSEONESHIELD = 59
---@source ../src/enums.h:474
RETURNVALUE_NOPARTYMEMBERSINRANGE = 60
---@source ../src/enums.h:475
RETURNVALUE_YOUARENOTTHEOWNER = 61
---@source ../src/enums.h:476
RETURNVALUE_TRADEPLAYERFARAWAY = 62
---@source ../src/enums.h:477
RETURNVALUE_YOUDONTOWNTHISHOUSE = 63
---@source ../src/enums.h:478
RETURNVALUE_TRADEPLAYERALREADYOWNSAHOUSE = 64
---@source ../src/enums.h:479
RETURNVALUE_TRADEPLAYERHIGHESTBIDDER = 65
---@source ../src/enums.h:480
RETURNVALUE_YOUCANNOTTRADETHISHOUSE = 66
---@source ../src/enums.h:481
RETURNVALUE_YOUDONTHAVEREQUIREDPROFESSION = 67
---@source ../src/enums.h:482
RETURNVALUE_CANNOTMOVEITEMISNOTSTOREITEM = 68
---@source ../src/enums.h:483
RETURNVALUE_ITEMCANNOTBEMOVEDTHERE = 69
---@source ../src/enums.h:484
RETURNVALUE_YOUCANNOTUSETHISBED = 70
---@source ../src/enums.h:485
RETURNVALUE_QUIVERAMMOONLY = 71

---@alias ConditionType number
---| 0 # CONDITION_NONE
---| 1 # CONDITION_POISON
---| 2 # CONDITION_FIRE
---| 4 # CONDITION_ENERGY
---| 8 # CONDITION_BLEEDING
---| 16 # CONDITION_HASTE
---| 32 # CONDITION_PARALYZE
---| 64 # CONDITION_OUTFIT
---| 128 # CONDITION_INVISIBLE
---| 256 # CONDITION_LIGHT
---| 512 # CONDITION_MANASHIELD
---| 1024 # CONDITION_INFIGHT
---| 2048 # CONDITION_DRUNK
---| 4096 # CONDITION_EXHAUST_WEAPON -- UNUSED
---| 8192 # CONDITION_REGENERATION
---| 16384 # CONDITION_SOUL
---| 32768 # CONDITION_DROWN
---| 65536 # CONDITION_MUTED
---| 131072 # CONDITION_CHANNELMUTEDTICKS
---| 262144 # CONDITION_YELLTICKS
---| 524288 # CONDITION_ATTRIBUTES
---| 1048576 # CONDITION_FREEZING
---| 2097152 # CONDITION_DAZZLED
---| 4194304 # CONDITION_CURSED
---| 8388608 # CONDITION_EXHAUST_COMBAT  -- UNUSED
---| 16777216 # CONDITION_EXHAUST_HEAL  -- UNUSED
---| 33554432 # CONDITION_PACIFIED
---| 67108864 # CONDITION_SPELLCOOLDOWN
---| 134217728 # CONDITION_SPELLGROUPCOOLDOWN
---| 268435456 # CONDITION_ROOT
---| 536870912 # CONDITION_MANASHIELD_BREAKABLE
CONDITION_NONE = 0
CONDITION_POISON = 1 * 2 ^ 0
CONDITION_FIRE = 1 * 2 ^ 1
CONDITION_ENERGY = 1 * 2 ^ 2
CONDITION_BLEEDING = 1 * 2 ^ 3
CONDITION_HASTE = 1 * 2 ^ 4
CONDITION_PARALYZE = 1 * 2 ^ 5
CONDITION_OUTFIT = 1 * 2 ^ 6
CONDITION_INVISIBLE = 1 * 2 ^ 7
CONDITION_LIGHT = 1 * 2 ^ 8
CONDITION_MANASHIELD = 1 * 2 ^ 9
CONDITION_INFIGHT = 1 * 2 ^ 10
CONDITION_DRUNK = 1 * 2 ^ 11
CONDITION_EXHAUST_WEAPON = 1 * 2 ^ 12 -- unused
CONDITION_REGENERATION = 1 * 2 ^ 13
CONDITION_SOUL = 1 * 2 ^ 14
CONDITION_DROWN = 1 * 2 ^ 15
CONDITION_MUTED = 1 * 2 ^ 16
CONDITION_CHANNELMUTEDTICKS = 1 * 2 ^ 17
CONDITION_YELLTICKS = 1 * 2 ^ 18
CONDITION_ATTRIBUTES = 1 * 2 ^ 19
CONDITION_FREEZING = 1 * 2 ^ 20
CONDITION_DAZZLED = 1 * 2 ^ 21
CONDITION_CURSED = 1 * 2 ^ 22
CONDITION_EXHAUST_COMBAT = 1 * 2 ^ 23 -- unused
CONDITION_EXHAUST_HEAL = 1 * 2 ^ 24   -- unused
CONDITION_PACIFIED = 1 * 2 ^ 25
CONDITION_SPELLCOOLDOWN = 1 * 2 ^ 26
CONDITION_SPELLGROUPCOOLDOWN = 1 * 2 ^ 27
CONDITION_ROOT = 1 * 2 ^ 28
CONDITION_MANASHIELD_BREAKABLE = 1 * 2 ^ 29

---@alias ConditionParam number
---| 0 # CONDITION_PARAM_TICKS
---| 1 # CONDITION_PARAM_OWNER
---| 2 # CONDITION_PARAM_TICKS
---| 4 # CONDITION_PARAM_HEALTHGAIN
---| 5 # CONDITION_PARAM_HEALTHTICKS
---| 6 # CONDITION_PARAM_MANAGAIN
---| 7 # CONDITION_PARAM_MANATICKS
---| 8 # CONDITION_PARAM_DELAYED
---| 9 # CONDITION_PARAM_SPEED
---| 10 # CONDITION_PARAM_LIGHT_LEVEL
---| 11 # CONDITION_PARAM_LIGHT_COLOR
---| 12 # CONDITION_PARAM_SOULGAIN
---| 13 # CONDITION_PARAM_SOULTICKS
---| 14 # CONDITION_PARAM_MINVALUE
---| 15 # CONDITION_PARAM_MAXVALUE
---| 16 # CONDITION_PARAM_STARTVALUE
---| 17 # CONDITION_PARAM_TICKINTERVAL
---| 18 # CONDITION_PARAM_FORCEUPDATE
---| 19 # CONDITION_PARAM_SKILL_MELEE
---| 20 # CONDITION_PARAM_SKILL_FIST
---| 21 # CONDITION_PARAM_SKILL_CLUB
---| 22 # CONDITION_PARAM_SKILL_SWORD
---| 23 # CONDITION_PARAM_SKILL_AXE
---| 24 # CONDITION_PARAM_SKILL_DISTANCE
---| 25 # CONDITION_PARAM_SKILL_SHIELD
---| 26 # CONDITION_PARAM_SKILL_FISHING
---| 27 # CONDITION_PARAM_STAT_MAXHITPOINTS
---| 28 # CONDITION_PARAM_STAT_MAXMANAPOINTS
---| 30 # CONDITION_PARAM_STAT_MAGICPOINTS
---| 31 # CONDITION_PARAM_STAT_MAXHITPOINTSPERCENT
---| 32 # CONDITION_PARAM_STAT_MAXMANAPOINTSPERCENT
---| 34 # CONDITION_PARAM_STAT_MAGICPOINTSPERCENT
---| 35 # CONDITION_PARAM_PERIODICDAMAGE
---| 36 # CONDITION_PARAM_SKILL_MELEEPERCENT
---| 37 # CONDITION_PARAM_SKILL_FISTPERCENT
---| 38 # CONDITION_PARAM_SKILL_CLUBPERCENT
---| 39 # CONDITION_PARAM_SKILL_SWORDPERCENT
---| 40 # CONDITION_PARAM_SKILL_AXEPERCENT
---| 41 # CONDITION_PARAM_SKILL_DISTANCEPERCENT
---| 42 # CONDITION_PARAM_SKILL_SHIELDPERCENT
---| 43 # CONDITION_PARAM_SKILL_FISHINGPERCENT
---| 44 # CONDITION_PARAM_BUFF_SPELL
---| 45 # CONDITION_PARAM_SUBID
---| 46 # CONDITION_PARAM_FIELD
---| 47 # CONDITION_PARAM_DISABLE_DEFENSE
---| 48 # CONDITION_PARAM_SPECIALSKILL_CRITICALHITCHANCE
---| 49 # CONDITION_PARAM_SPECIALSKILL_CRITICALHITAMOUNT
---| 50 # CONDITION_PARAM_SPECIALSKILL_LIFELEECHCHANCE
---| 51 # CONDITION_PARAM_SPECIALSKILL_LIFELEECHAMOUNT
---| 52 # CONDITION_PARAM_SPECIALSKILL_MANALEECHCHANCE
---| 53 # CONDITION_PARAM_SPECIALSKILL_MANALEECHAMOUNT
---| 54 # CONDITION_PARAM_AGGRESSIVE
---| 55 # CONDITION_PARAM_DRUNKENNESS
---| 56 # CONDITION_PARAM_MANASHIELD_BREAKABLE
CONDITION_PARAM_OWNER = 1
CONDITION_PARAM_TICKS = 2
-- CONDITION_PARAM_OUTFIT = 3
CONDITION_PARAM_HEALTHGAIN = 4
CONDITION_PARAM_HEALTHTICKS = 5
CONDITION_PARAM_MANAGAIN = 6
CONDITION_PARAM_MANATICKS = 7
CONDITION_PARAM_DELAYED = 8
CONDITION_PARAM_SPEED = 9
CONDITION_PARAM_LIGHT_LEVEL = 10
CONDITION_PARAM_LIGHT_COLOR = 11
CONDITION_PARAM_SOULGAIN = 12
CONDITION_PARAM_SOULTICKS = 13
CONDITION_PARAM_MINVALUE = 14
CONDITION_PARAM_MAXVALUE = 15
CONDITION_PARAM_STARTVALUE = 16
CONDITION_PARAM_TICKINTERVAL = 17
CONDITION_PARAM_FORCEUPDATE = 18
CONDITION_PARAM_SKILL_MELEE = 19
CONDITION_PARAM_SKILL_FIST = 20
CONDITION_PARAM_SKILL_CLUB = 21
CONDITION_PARAM_SKILL_SWORD = 22
CONDITION_PARAM_SKILL_AXE = 23
CONDITION_PARAM_SKILL_DISTANCE = 24
CONDITION_PARAM_SKILL_SHIELD = 25
CONDITION_PARAM_SKILL_FISHING = 26
CONDITION_PARAM_STAT_MAXHITPOINTS = 27
CONDITION_PARAM_STAT_MAXMANAPOINTS = 28
-- CONDITION_PARAM_STAT_SOULPOINTS = 29
CONDITION_PARAM_STAT_MAGICPOINTS = 30
CONDITION_PARAM_STAT_MAXHITPOINTSPERCENT = 31
CONDITION_PARAM_STAT_MAXMANAPOINTSPERCENT = 32
-- CONDITION_PARAM_STAT_SOULPOINTSPERCENT = 33
CONDITION_PARAM_STAT_MAGICPOINTSPERCENT = 34
CONDITION_PARAM_PERIODICDAMAGE = 35
CONDITION_PARAM_SKILL_MELEEPERCENT = 36
CONDITION_PARAM_SKILL_FISTPERCENT = 37
CONDITION_PARAM_SKILL_CLUBPERCENT = 38
CONDITION_PARAM_SKILL_SWORDPERCENT = 39
CONDITION_PARAM_SKILL_AXEPERCENT = 40
CONDITION_PARAM_SKILL_DISTANCEPERCENT = 41
CONDITION_PARAM_SKILL_SHIELDPERCENT = 42
CONDITION_PARAM_SKILL_FISHINGPERCENT = 43
CONDITION_PARAM_BUFF_SPELL = 44
CONDITION_PARAM_SUBID = 45
CONDITION_PARAM_FIELD = 46
CONDITION_PARAM_DISABLE_DEFENSE = 47
CONDITION_PARAM_SPECIALSKILL_CRITICALHITCHANCE = 48
CONDITION_PARAM_SPECIALSKILL_CRITICALHITAMOUNT = 49
CONDITION_PARAM_SPECIALSKILL_LIFELEECHCHANCE = 50
CONDITION_PARAM_SPECIALSKILL_LIFELEECHAMOUNT = 51
CONDITION_PARAM_SPECIALSKILL_MANALEECHCHANCE = 52
CONDITION_PARAM_SPECIALSKILL_MANALEECHAMOUNT = 53
CONDITION_PARAM_AGGRESSIVE = 54
CONDITION_PARAM_DRUNKENNESS = 55
CONDITION_PARAM_MANASHIELD_BREAKABLE = 56

---@alias WeaponType number
---| 0 # WEAPON_NONE
---| 1 # WEAPON_SWORD
---| 2 # WEAPON_CLUB
---| 3 # WEAPON_AXE
---| 4 # WEAPON_SHIELD
---| 5 # WEAPON_DISTANCE
---| 6 # WEAPON_WAND
---| 7 # WEAPON_AMMO
---| 8 # WEAPON_QUIVER
WEAPON_NONE = 0
WEAPON_SWORD = 1
WEAPON_CLUB = 2
WEAPON_AXE = 3
WEAPON_SHIELD = 4
WEAPON_DISTANCE = 5
WEAPON_WAND = 6
WEAPON_AMMO = 7
WEAPON_QUIVER = 8

---@alias SlotPositions number
---| 4294967295 # SLOTP_WHEREEVER
---| 1 # SLOTP_HEAD
---| 2 # SLOTP_NECKLACE
---| 4 # SLOTP_BACKPACK
---| 8 # SLOTP_ARMOR
---| 16 # SLOTP_RIGHT
---| 32 # SLOTP_LEFT
---| 64 # SLOTP_LEGS
---| 128 # SLOTP_FEET
---| 256 # SLOTP_RING
---| 512 # SLOTP_AMMO
---| 1024 # SLOTP_DEPOT
---| 2048 # SLOTP_TWO_HAND
---| 48 # SLOTP_HAND
SLOTP_WHEREEVER = 0xFFFFFFFF
SLOTP_HEAD = 1 * 2 ^ 0
SLOTP_NECKLACE = 1 * 2 ^ 1
SLOTP_BACKPACK = 1 * 2 ^ 2
SLOTP_ARMOR = 1 * 2 ^ 3
SLOTP_RIGHT = 1 * 2 ^ 4
SLOTP_LEFT = 1 * 2 ^ 5
SLOTP_LEGS = 1 * 2 ^ 6
SLOTP_FEET = 1 * 2 ^ 7
SLOTP_RING = 1 * 2 ^ 8
SLOTP_AMMO = 1 * 2 ^ 9
SLOTP_DEPOT = 1 * 2 ^ 10
SLOTP_TWO_HAND = 1 * 2 ^ 11
SLOTP_HAND = SLOTP_LEFT or SLOTP_RIGHT

---@alias DamageOrigin number
---| 0 # ORIGIN_NONE
---| 1 # ORIGIN_CONDITION
---| 2 # ORIGIN_SPELL
---| 3 # ORIGIN_MELEE
---| 4 # ORIGIN_RANGED
---| 5 # ORIGIN_WAND
---| 6 # ORIGIN_REFLECT
ORIGIN_NONE = 0
ORIGIN_CONDITION = 1
ORIGIN_SPELL = 2
ORIGIN_MELEE = 3
ORIGIN_RANGED = 4
ORIGIN_WAND = 5
ORIGIN_REFLECT = 6

---@alias PlayerSexes number
---| 0 # PLAYERSEX_FEMALE
---| 1 # PLAYERSEX_MALE
PLAYERSEX_FEMALE = 0
PLAYERSEX_MALE = 1

---@alias ConditionIds number
---| -1 # CONDITIONID_DEFAULT
---| 0 # CONDITIONID_COMBAT
---| 1 # CONDITIONID_HEAD
---| 2 # CONDITIONID_NECKLACE
---| 3 # CONDITIONID_BACKPACK
---| 4 # CONDITIONID_ARMOR
---| 5 # CONDITIONID_RIGHT
---| 6 # CONDITIONID_LEFT
---| 7 # CONDITIONID_LEGS
---| 8 # CONDITIONID_FEET
---| 9 # CONDITIONID_RING
---| 10 # CONDITIONID_AMMO
CONDITIONID_DEFAULT = -1
CONDITIONID_COMBAT = 0
CONDITIONID_HEAD = 1
CONDITIONID_NECKLACE = 2
CONDITIONID_BACKPACK = 3
CONDITIONID_ARMOR = 4
CONDITIONID_RIGHT = 5
CONDITIONID_LEFT = 6
CONDITIONID_LEGS = 7
CONDITIONID_FEET = 8
CONDITIONID_RING = 9
CONDITIONID_AMMO = 10

---@alias MapMarks uint8
---| 0 # MAPMARK_TICK
---| 1 # MAPMARK_QUESTION
---| 2 # MAPMARK_EXCLAMATION
---| 3 # MAPMARK_STAR
---| 4 # MAPMARK_CROSS
---| 5 # MAPMARK_TEMPLE
---| 6 # MAPMARK_KISS
---| 7 # MAPMARK_SHOVEL
---| 8 # MAPMARK_SWORD
---| 9 # MAPMARK_FLAG
---| 10 # MAPMARK_LOCK
---| 11 # MAPMARK_BAG
---| 12 # MAPMARK_SKULL
---| 13 # MAPMARK_DOLLAR
---| 14 # MAPMARK_REDNORTH
---| 15 # MAPMARK_REDSOUTH
---| 16 # MAPMARK_REDEAST
---| 17 # MAPMARK_REDWEST
---| 18 # MAPMARK_GREENNORTH
---| 19 # MAPMARK_GREENSOUTH
MAPMARK_TICK = 0
MAPMARK_QUESTION = 1
MAPMARK_EXCLAMATION = 2
MAPMARK_STAR = 3
MAPMARK_CROSS = 4
MAPMARK_TEMPLE = 5
MAPMARK_KISS = 6
MAPMARK_SHOVEL = 7
MAPMARK_SWORD = 8
MAPMARK_FLAG = 9
MAPMARK_LOCK = 10
MAPMARK_BAG = 11
MAPMARK_SKULL = 12
MAPMARK_DOLLAR = 13
MAPMARK_REDNORTH = 14
MAPMARK_REDSOUTH = 15
MAPMARK_REDEAST = 16
MAPMARK_REDWEST = 17
MAPMARK_GREENNORTH = 18
MAPMARK_GREENSOUTH = 19

---@alias ReloadTypes uint8
---| 0 # RELOAD_TYPE_ALL
---| 1 # RELOAD_TYPE_ACTIONS
---| 2 # RELOAD_TYPE_CHAT
---| 3 # RELOAD_TYPE_CONFIG
---| 4 # RELOAD_TYPE_CREATURESCRIPTS
---| 5 # RELOAD_TYPE_EVENTS
---| 6 # RELOAD_TYPE_GLOBAL
---| 7 # RELOAD_TYPE_GLOBALEVENTS
---| 8 # RELOAD_TYPE_ITEMS
---| 9 # RELOAD_TYPE_MONSTERS
---| 10 # RELOAD_TYPE_MOUNTS
---| 11 # RELOAD_TYPE_MOVEMENTS
---| 12 # RELOAD_TYPE_NPCS
---| 13 # RELOAD_TYPE_QUESTS
---| 14 # RELOAD_TYPE_SCRIPTS
---| 15 # RELOAD_TYPE_SPELLS
---| 16 # RELOAD_TYPE_TALKACTIONS
---| 17 # RELOAD_TYPE_WEAPONS

---@source ../src/const.h:666
RELOAD_TYPE_ALL = 0
---@source ../src/const.h:667
RELOAD_TYPE_ACTIONS = 1
---@source ../src/const.h:668
RELOAD_TYPE_CHAT = 2
---@source ../src/const.h:669
RELOAD_TYPE_CONFIG = 3
---@source ../src/const.h:670
RELOAD_TYPE_CREATURESCRIPTS = 4
---@source ../src/const.h:671
RELOAD_TYPE_EVENTS = 5
---@source ../src/const.h:672
RELOAD_TYPE_GLOBAL = 6
---@source ../src/const.h:673
RELOAD_TYPE_GLOBALEVENTS = 7
---@source ../src/const.h:674
RELOAD_TYPE_ITEMS = 8
---@source ../src/const.h:675
RELOAD_TYPE_MONSTERS = 9
---@source ../src/const.h:676
RELOAD_TYPE_MOUNTS = 10
---@source ../src/const.h:677
RELOAD_TYPE_MOVEMENTS = 11
---@source ../src/const.h:678
RELOAD_TYPE_NPCS = 12
---@source ../src/const.h:679
RELOAD_TYPE_QUESTS = 13
---@source ../src/const.h:680
RELOAD_TYPE_SCRIPTS = 14
---@source ../src/const.h:681
RELOAD_TYPE_SPELLS = 15
---@source ../src/const.h:682
RELOAD_TYPE_TALKACTIONS = 16
---@source ../src/const.h:683
RELOAD_TYPE_WEAPONS = 17

---@alias PlayerFlags number
---| 1 # PlayerFlag_CannotUseCombat
---| 2 # PlayerFlag_CannotAttackPlayer
---| 4 # PlayerFlag_CannotAttackMonster
---| 8 # PlayerFlag_CannotBeAttacked
---| 16 # PlayerFlag_CanConvinceAll
---| 32 # PlayerFlag_CanSummonAll
---| 64 # PlayerFlag_CanIllusionAll
---| 128 # PlayerFlag_CanSenseInvisibility
---| 256 # PlayerFlag_IgnoredByMonsters
---| 512 # PlayerFlag_NotGainInFight
---| 1024 # PlayerFlag_HasInfiniteMana
---| 2048 # PlayerFlag_HasInfiniteSoul
---| 4096 # PlayerFlag_HasNoExhaustion
---| 8192 # PlayerFlag_CannotUseSpells
---| 16384 # PlayerFlag_CannotPickupItem
---| 32768 # PlayerFlag_CanAlwaysLogin
---| 65536 # PlayerFlag_CanBroadcast
---| 131072 # PlayerFlag_CanEditHouses
---| 262144 # PlayerFlag_CannotBeBanned
---| 524288 # PlayerFlag_CannotBePushed
---| 1048576 # PlayerFlag_HasInfiniteCapacity
---| 2097152 # PlayerFlag_CanPushAllCreatures
---| 4194304 # PlayerFlag_CanTalkRedPrivate
---| 8388608 # PlayerFlag_CanTalkRedChannel
---| 16777216 # PlayerFlag_TalkOrangeHelpChannel
---| 33554432 # PlayerFlag_NotGainExperience
---| 67108864 # PlayerFlag_NotGainMana
---| 134217728 # PlayerFlag_NotGainHealth
---| 268435456 # PlayerFlag_NotGainSkill
---| 536870912 # PlayerFlag_SetMaxSpeed
---| 1073741824 # PlayerFlag_SpecialVIP
---| 2147483648 # PlayerFlag_NotGenerateLoot
---| 8589934592 # PlayerFlag_IgnoreProtectionZone
---| 17179869184 # PlayerFlag_IgnoreSpellCheck
---| 34359738368 # PlayerFlag_IgnoreWeaponCheck
---| 68719476736 # PlayerFlag_CannotBeMuted
---| 137438953472 # PlayerFlag_IsAlwaysPremium
---| 274877906944 # PlayerFlag_IgnoreYellCheck
---| 549755813888 # PlayerFlag_IgnoreSendPrivateCheck
PlayerFlag_CannotUseCombat = 1 * 2 ^ 0
PlayerFlag_CannotAttackPlayer = 1 * 2 ^ 1
PlayerFlag_CannotAttackMonster = 1 * 2 ^ 2
PlayerFlag_CannotBeAttacked = 1 * 2 ^ 3
PlayerFlag_CanConvinceAll = 1 * 2 ^ 4
PlayerFlag_CanSummonAll = 1 * 2 ^ 5
PlayerFlag_CanIllusionAll = 1 * 2 ^ 6
PlayerFlag_CanSenseInvisibility = 1 * 2 ^ 7
PlayerFlag_IgnoredByMonsters = 1 * 2 ^ 8
PlayerFlag_NotGainInFight = 1 * 2 ^ 9
PlayerFlag_HasInfiniteMana = 1 * 2 ^ 10
PlayerFlag_HasInfiniteSoul = 1 * 2 ^ 11
PlayerFlag_HasNoExhaustion = 1 * 2 ^ 12
PlayerFlag_CannotUseSpells = 1 * 2 ^ 13
PlayerFlag_CannotPickupItem = 1 * 2 ^ 14
PlayerFlag_CanAlwaysLogin = 1 * 2 ^ 15
PlayerFlag_CanBroadcast = 1 * 2 ^ 16
PlayerFlag_CanEditHouses = 1 * 2 ^ 17
PlayerFlag_CannotBeBanned = 1 * 2 ^ 18
PlayerFlag_CannotBePushed = 1 * 2 ^ 19
PlayerFlag_HasInfiniteCapacity = 1 * 2 ^ 20
PlayerFlag_CanPushAllCreatures = 1 * 2 ^ 21
PlayerFlag_CanTalkRedPrivate = 1 * 2 ^ 22
PlayerFlag_CanTalkRedChannel = 1 * 2 ^ 23
PlayerFlag_TalkOrangeHelpChannel = 1 * 2 ^ 24
PlayerFlag_NotGainExperience = 1 * 2 ^ 25
PlayerFlag_NotGainMana = 1 * 2 ^ 26
PlayerFlag_NotGainHealth = 1 * 2 ^ 27
PlayerFlag_NotGainSkill = 1 * 2 ^ 28
PlayerFlag_SetMaxSpeed = 1 * 2 ^ 29
PlayerFlag_SpecialVIP = 1 * 2 ^ 30
PlayerFlag_NotGenerateLoot = 1 * 2 ^ 31
-- PlayerFlag with exponent 32 existed but was deprecated (feature dropped from client).
PlayerFlag_IgnoreProtectionZone = 1 * 2 ^ 33
PlayerFlag_IgnoreSpellCheck = 1 * 2 ^ 34
PlayerFlag_IgnoreWeaponCheck = 1 * 2 ^ 35
PlayerFlag_CannotBeMuted = 1 * 2 ^ 36
PlayerFlag_IsAlwaysPremium = 1 * 2 ^ 37
PlayerFlag_IgnoreYellCheck = 1 * 2 ^ 38
PlayerFlag_IgnoreSendPrivateCheck = 1 * 2 ^ 39

---@alias ItemAttributes number
---| 0 # ITEM_ATTRIBUTE_NONE
---| 1 # ITEM_ATTRIBUTE_ACTIONID
---| 2 # ITEM_ATTRIBUTE_UNIQUEID
---| 4 # ITEM_ATTRIBUTE_DESCRIPTION
---| 8 # ITEM_ATTRIBUTE_TEXT
---| 16 # ITEM_ATTRIBUTE_DATE
---| 32 # ITEM_ATTRIBUTE_WRITER
---| 64 # ITEM_ATTRIBUTE_NAME
---| 128 # ITEM_ATTRIBUTE_ARTICLE
---| 256 # ITEM_ATTRIBUTE_PLURALNAME
---| 512 # ITEM_ATTRIBUTE_WEIGHT
---| 1024 # ITEM_ATTRIBUTE_ATTACK
---| 2048 # ITEM_ATTRIBUTE_DEFENSE
---| 4096 # ITEM_ATTRIBUTE_EXTRADEFENSE
---| 8192 # ITEM_ATTRIBUTE_ARMOR
---| 16384 # ITEM_ATTRIBUTE_HITCHANCE
---| 32768 # ITEM_ATTRIBUTE_SHOOTRANGE
---| 65536 # ITEM_ATTRIBUTE_OWNER
---| 131072 # ITEM_ATTRIBUTE_DURATION
---| 262144 # ITEM_ATTRIBUTE_DECAYSTATE
---| 524288 # ITEM_ATTRIBUTE_CORPSEOWNER
---| 1048576 # ITEM_ATTRIBUTE_CHARGES
---| 2097152 # ITEM_ATTRIBUTE_FLUIDTYPE
---| 4194304 # ITEM_ATTRIBUTE_DOORID
---| 8388608 # ITEM_ATTRIBUTE_DECAYTO
---| 16777216 # ITEM_ATTRIBUTE_WRAPID
---| 33554432 # ITEM_ATTRIBUTE_STOREITEM
---| 67108864 # ITEM_ATTRIBUTE_ATTACK_SPEED
---| 134217728 # ITEM_ATTRIBUTE_OPENCONTAINER
---| 134217728 # ITEM_ATTRIBUTE_DURATION_MIN
---| 134217728 # ITEM_ATTRIBUTE_DURATION_MAX
---| 2147483648 # ITEM_ATTRIBUTE_CUSTOM

---@source ../src/enums.h:48
ITEM_ATTRIBUTE_NONE = 0
---@source ../src/enums.h:50
ITEM_ATTRIBUTE_ACTIONID = 1 * 2 ^ 0
---@source ../src/enums.h:51
ITEM_ATTRIBUTE_UNIQUEID = 1 * 2 ^ 1
---@source ../src/enums.h:52
ITEM_ATTRIBUTE_DESCRIPTION = 1 * 2 ^ 2
---@source ../src/enums.h:53
ITEM_ATTRIBUTE_TEXT = 1 * 2 ^ 3
---@source ../src/enums.h:54
ITEM_ATTRIBUTE_DATE = 1 * 2 ^ 4
---@source ../src/enums.h:55
ITEM_ATTRIBUTE_WRITER = 1 * 2 ^ 5
---@source ../src/enums.h:56
ITEM_ATTRIBUTE_NAME = 1 * 2 ^ 6
---@source ../src/enums.h:57
ITEM_ATTRIBUTE_ARTICLE = 1 * 2 ^ 7
---@source ../src/enums.h:58
ITEM_ATTRIBUTE_PLURALNAME = 1 * 2 ^ 8
---@source ../src/enums.h:59
ITEM_ATTRIBUTE_WEIGHT = 1 * 2 ^ 9
---@source ../src/enums.h:60
ITEM_ATTRIBUTE_ATTACK = 1 * 2 ^ 10
---@source ../src/enums.h:61
ITEM_ATTRIBUTE_DEFENSE = 1 * 2 ^ 11
---@source ../src/enums.h:62
ITEM_ATTRIBUTE_EXTRADEFENSE = 1 * 2 ^ 12
---@source ../src/enums.h:63
ITEM_ATTRIBUTE_ARMOR = 1 * 2 ^ 13
---@source ../src/enums.h:64
ITEM_ATTRIBUTE_HITCHANCE = 1 * 2 ^ 14
---@source ../src/enums.h:65
ITEM_ATTRIBUTE_SHOOTRANGE = 1 * 2 ^ 15
---@source ../src/enums.h:66
ITEM_ATTRIBUTE_OWNER = 1 * 2 ^ 16
---@source ../src/enums.h:67
ITEM_ATTRIBUTE_DURATION = 1 * 2 ^ 17
---@source ../src/enums.h:68
ITEM_ATTRIBUTE_DECAYSTATE = 1 * 2 ^ 18
---@source ../src/enums.h:69
ITEM_ATTRIBUTE_CORPSEOWNER = 1 * 2 ^ 19
---@source ../src/enums.h:70
ITEM_ATTRIBUTE_CHARGES = 1 * 2 ^ 20
---@source ../src/enums.h:71
ITEM_ATTRIBUTE_FLUIDTYPE = 1 * 2 ^ 21
---@source ../src/enums.h:72
ITEM_ATTRIBUTE_DOORID = 1 * 2 ^ 22
---@source ../src/enums.h:73
ITEM_ATTRIBUTE_DECAYTO = 1 * 2 ^ 23
---@source ../src/enums.h:74
ITEM_ATTRIBUTE_WRAPID = 1 * 2 ^ 24
---@source ../src/enums.h:75
ITEM_ATTRIBUTE_STOREITEM = 1 * 2 ^ 25
---@source ../src/enums.h:76
ITEM_ATTRIBUTE_ATTACK_SPEED = 1 * 2 ^ 26
---@source ../src/enums.h:77
ITEM_ATTRIBUTE_OPENCONTAINER = 1 * 2 ^ 27
---@source ../src/enums.h:78
ITEM_ATTRIBUTE_DURATION_MIN = ITEM_ATTRIBUTE_DURATION
---@source ../src/enums.h:79
ITEM_ATTRIBUTE_DURATION_MAX = 1 * 2 ^ 27
---@source ../src/enums.h:80
ITEM_ATTRIBUTE_CUSTOM = 1 * 2 ^ 31

ITEM_BROWSEFIELD = 460
ITEM_DECORATION_KIT = 26054
ITEM_FIREFIELD_PVP_FULL = 1487
ITEM_FIREFIELD_PVP_MEDIUM = 1488
ITEM_FIREFIELD_PVP_SMALL = 1489
ITEM_FIREFIELD_PERSISTENT_FULL = 1492
ITEM_FIREFIELD_PERSISTENT_MEDIUM = 1493
ITEM_FIREFIELD_PERSISTENT_SMALL = 1494
ITEM_FIREFIELD_NOPVP = 1500
ITEM_FIREFIELD_NOPVP_MEDIUM = 1501
ITEM_POISONFIELD_PVP = 1490
ITEM_POISONFIELD_PERSISTENT = 1496
ITEM_POISONFIELD_NOPVP = 1503
ITEM_ENERGYFIELD_PVP = 1491
ITEM_ENERGYFIELD_PERSISTENT = 1495
ITEM_ENERGYFIELD_NOPVP = 1504
ITEM_MAGICWALL = 1497
ITEM_MAGICWALL_PERSISTENT = 1498
ITEM_MAGICWALL_SAFE = 11098
ITEM_MAGICWALL_NOPVP = 20669
ITEM_WILDGROWTH = 1499
ITEM_WILDGROWTH_PERSISTENT = 2721
ITEM_WILDGROWTH_SAFE = 11099
ITEM_WILDGROWTH_NOPVP = 20670
ITEM_BAG = 1987
ITEM_SHOPPING_BAG = 23782
ITEM_GOLD_COIN = 2148
ITEM_PLATINUM_COIN = 2152
ITEM_CRYSTAL_COIN = 2160
ITEM_STORE_COIN = 24774
ITEM_DEPOT = 2594
ITEM_LOCKER = 2589
ITEM_INBOX = 14404
ITEM_MARKET = 14405
ITEM_STORE_INBOX = 26052
ITEM_DEPOT_BOX_I = 25453
ITEM_DEPOT_BOX_II = 25454
ITEM_DEPOT_BOX_III = 25455
ITEM_DEPOT_BOX_IV = 25456
ITEM_DEPOT_BOX_V = 25457
ITEM_DEPOT_BOX_VI = 25458
ITEM_DEPOT_BOX_VII = 25459
ITEM_DEPOT_BOX_VIII = 25460
ITEM_DEPOT_BOX_IX = 25461
ITEM_DEPOT_BOX_X = 25462
ITEM_DEPOT_BOX_XI = 25463
ITEM_DEPOT_BOX_XII = 25464
ITEM_DEPOT_BOX_XIII = 25465
ITEM_DEPOT_BOX_XIV = 25466
ITEM_DEPOT_BOX_XV = 25467
ITEM_DEPOT_BOX_XVI = 25468
ITEM_DEPOT_BOX_XVII = 25469
ITEM_DEPOT_BOX_XVIII = 34571
ITEM_DEPOT_BOX_XIX = 44714
ITEM_DEPOT_BOX_XX = 44715
ITEM_MALE_CORPSE = 3058
ITEM_FEMALE_CORPSE = 3065
ITEM_FULLSPLASH = 2016
ITEM_SMALLSPLASH = 2019
ITEM_PARCEL = 2595
ITEM_LETTER = 2597
ITEM_LETTER_STAMPED = 2598
ITEM_LABEL = 2599
ITEM_AMULETOFLOSS = 2173
ITEM_DOCUMENT_RO = 1968

RESOURCE_BANK_BALANCE = 0
RESOURCE_GOLD_EQUIPPED = 1
RESOURCE_PREY_WILDCARDS = 10
RESOURCE_DAILYREWARD_STREAK = 20
RESOURCE_DAILYREWARD_JOKERS = 21

---@alias OperatingSystem uint8
---| 0 # CLIENTOS_NONE
---| 1 # CLIENTOS_LINUX
---| 2 # CLIENTOS_WINDOWS
---| 3 # CLIENTOS_FLASH
---| 4 # CLIENTOS_QT_LINUX
---| 5 # CLIENTOS_QT_WINDOWS
---| 6 # CLIENTOS_QT_MAC
---| 7 # CLIENTOS_QT_LINUX2
---| 10 # CLIENTOS_OTCLIENT_LINUX
---| 11 # CLIENTOS_OTCLIENT_WINDOWS
---| 12 # CLIENTOS_OTCLIENT_MAC
CLIENTOS_NONE = 0
CLIENTOS_LINUX = 1
CLIENTOS_WINDOWS = 2
CLIENTOS_FLASH = 3
CLIENTOS_QT_LINUX = 4
CLIENTOS_QT_WINDOWS = 5
CLIENTOS_QT_MAC = 6
CLIENTOS_QT_LINUX2 = 7
CLIENTOS_OTCLIENT_LINUX = 10
CLIENTOS_OTCLIENT_WINDOWS = 11
CLIENTOS_OTCLIENT_MAC = 12

---@alias SkillTypes number
---| 0 # SKILL_FIST
---| 1 # SKILL_CLUB
---| 2 # SKILL_SWORD
---| 3 # SKILL_AXE
---| 4 # SKILL_DISTANCE
---| 5 # SKILL_SHIELD
---| 6 # SKILL_FISHING
---| 7 # SKILL_MAGLEVEL
---| 8 # SKILL_LEVEL
---| 0 # SKILL_FIRST
---| 8 # SKILL_LAST
SKILL_FIST = 0
SKILL_CLUB = 1
SKILL_SWORD = 2
SKILL_AXE = 3
SKILL_DISTANCE = 4
SKILL_SHIELD = 5
SKILL_FISHING = 6
SKILL_MAGLEVEL = 7
SKILL_LEVEL = 8
SKILL_FIRST = SKILL_FIST
SKILL_LAST = SKILL_FISHING

---@enum configKeys
---@type table<string, any>
configKeys = {
	-- ConfigKeysBoolean
	ALLOW_CHANGEOUTFIT = 0,
	ONE_PLAYER_ON_ACCOUNT = 1,
	AIMBOT_HOTKEY_ENABLED = 2,
	REMOVE_RUNE_CHARGES = 3,
	REMOVE_WEAPON_AMMO = 4,
	REMOVE_WEAPON_CHARGES = 5,
	REMOVE_POTION_CHARGES = 6,
	EXPERIENCE_FROM_PLAYERS = 7,
	FREE_PREMIUM = 8,
	REPLACE_KICK_ON_LOGIN = 9,
	ALLOW_CLONES = 10,
	ALLOW_WALKTHROUGH = 11,
	BIND_ONLY_GLOBAL_ADDRESS = 12,
	OPTIMIZE_DATABASE = 13,
	MARKET_PREMIUM = 14,
	EMOTE_SPELLS = 15,
	STAMINA_SYSTEM = 16,
	WARN_UNSAFE_SCRIPTS = 17,
	CONVERT_UNSAFE_SCRIPTS = 18,
	CLASSIC_EQUIPMENT_SLOTS = 19,
	CLASSIC_ATTACK_SPEED = 20,
	SCRIPTS_CONSOLE_LOGS = 21,
	SERVER_SAVE_NOTIFY_MESSAGE = 22,
	SERVER_SAVE_CLEAN_MAP = 23,
	SERVER_SAVE_CLOSE = 24,
	SERVER_SAVE_SHUTDOWN = 25,
	ONLINE_OFFLINE_CHARLIST = 26,
	YELL_ALLOW_PREMIUM = 27,
	PREMIUM_TO_SEND_PRIVATE = 28,
	FORCE_MONSTERTYPE_LOAD = 29,
	HOUSE_OWNED_BY_ACCOUNT = 30,
	CLEAN_PROTECTION_ZONES = 31,
	HOUSE_DOOR_SHOW_PRICE = 32,
	ONLY_INVITED_CAN_MOVE_HOUSE_ITEMS = 33,
	REMOVE_ON_DESPAWN = 34,
	TWO_FACTOR_AUTH = 35,
	MANASHIELD_BREAKABLE = 36,
	CHECK_DUPLICATE_STORAGE_KEYS = 37,
	MONSTER_OVERSPAWN = 38,

	-- ConfigKeysString
	MAP_NAME = 0,
	HOUSE_RENT_PERIOD = 1,
	SERVER_NAME = 2,
	OWNER_NAME = 3,
	OWNER_EMAIL = 4,
	URL = 5,
	LOCATION = 6,
	IP = 7,
	WORLD_TYPE = 8,
	MYSQL_HOST = 9,
	MYSQL_USER = 10,
	MYSQL_PASS = 11,
	MYSQL_DB = 12,
	MYSQL_SOCK = 13,
	DEFAULT_PRIORITY = 14,
	MAP_AUTHOR = 15,
	CONFIG_FILE = 16,

	-- ConfigKeysInteger
	SQL_PORT = 0,
	MAX_PLAYERS = 1,
	PZ_LOCKED = 2,
	DEFAULT_DESPAWNRANGE = 3,
	DEFAULT_DESPAWNRADIUS = 4,
	DEFAULT_WALKTOSPAWNRADIUS = 5,
	RATE_EXPERIENCE = 6,
	RATE_SKILL = 7,
	RATE_LOOT = 8,
	RATE_MAGIC = 9,
	RATE_SPAWN = 10,
	HOUSE_PRICE = 11,
	KILLS_TO_RED = 12,
	KILLS_TO_BLACK = 13,
	MAX_MESSAGEBUFFER = 14,
	ACTIONS_DELAY_INTERVAL = 15,
	EX_ACTIONS_DELAY_INTERVAL = 16,
	KICK_AFTER_MINUTES = 17,
	PROTECTION_LEVEL = 18,
	DEATH_LOSE_PERCENT = 19,
	STATUSQUERY_TIMEOUT = 20,
	FRAG_TIME = 21,
	WHITE_SKULL_TIME = 22,
	GAME_PORT = 23,
	LOGIN_PORT = 24,
	STATUS_PORT = 25,
	HTTP_PORT = 26,
	HTTP_WORKERS = 27,
	STAIRHOP_DELAY = 28,
	MARKET_OFFER_DURATION = 29,
	CHECK_EXPIRED_MARKET_OFFERS_EACH_MINUTES = 30,
	MAX_MARKET_OFFERS_AT_A_TIME_PER_PLAYER = 31,
	EXP_FROM_PLAYERS_LEVEL_RANGE = 32,
	MAX_PACKETS_PER_SECOND = 33,
	SERVER_SAVE_NOTIFY_DURATION = 34,
	YELL_MINIMUM_LEVEL = 35,
	MINIMUM_LEVEL_TO_SEND_PRIVATE = 36,
	VIP_FREE_LIMIT = 37,
	VIP_PREMIUM_LIMIT = 38,
	DEPOT_FREE_LIMIT = 39,
	DEPOT_PREMIUM_LIMIT = 40,
	QUEST_TRACKER_FREE_LIMIT = 41,
	QUEST_TRACKER_PREMIUM_LIMIT = 42,
	STAMINA_REGEN_MINUTE = 43,
	STAMINA_REGEN_PREMIUM = 44,
}

ITEM_TYPE_NONE = 0
ITEM_TYPE_DEPOT = 1
ITEM_TYPE_MAILBOX = 2
ITEM_TYPE_TRASHHOLDER = 3
ITEM_TYPE_CONTAINER = 4
ITEM_TYPE_DOOR = 5
ITEM_TYPE_MAGICFIELD = 6
ITEM_TYPE_TELEPORT = 7
ITEM_TYPE_BED = 8
ITEM_TYPE_KEY = 9
ITEM_TYPE_RUNE = 10
ITEM_TYPE_PODIUM = 11
ITEM_TYPE_LAST = 12

ITEM_GROUP_NONE = 0
ITEM_GROUP_GROUND = 1
ITEM_GROUP_CONTAINER = 2
ITEM_GROUP_WEAPON = 3     -- deprecated
ITEM_GROUP_AMMUNITION = 4 -- deprecated
ITEM_GROUP_ARMOR = 5      -- deprecated
ITEM_GROUP_CHARGES = 6
ITEM_GROUP_TELEPORT = 7   -- deprecated
ITEM_GROUP_MAGICFIELD = 8 -- deprecated
ITEM_GROUP_WRITEABLE = 9  -- deprecated
ITEM_GROUP_KEY = 10       -- deprecated
ITEM_GROUP_SPLASH = 11
ITEM_GROUP_FLUID = 12
ITEM_GROUP_DOOR = 13      -- deprecated
ITEM_GROUP_DEPRECATED = 14
ITEM_GROUP_PODIUM = 15
ITEM_GROUP_LAST = 16

AMMO_NONE = 0
AMMO_BOLT = 1
AMMO_ARROW = 2
AMMO_SPEAR = 3
AMMO_THROWINGSTAR = 4
AMMO_THROWINGKNIFE = 5
AMMO_STONE = 6
AMMO_SNOWBALL = 7

WIELDINFO_NONE = 0 * 2 ^ 0
WIELDINFO_LEVEL = 1 * 2 ^ 0
WIELDINFO_MAGLV = 1 * 2 ^ 1
WIELDINFO_VOCREQ = 1 * 2 ^ 2
WIELDINFO_PREMIUM = 1 * 2 ^ 3

---@alias SpeakClasses uint8
---| 1 # TALKTYPE_SAY
---| 2 # TALKTYPE_WHISPER
---| 3 # TALKTYPE_YELL
---| 4 # TALKTYPE_PRIVATE_FROM -- Received private message
---| 5 # TALKTYPE_PRIVATE_TO -- Sent private message
---| 7 # TALKTYPE_CHANNEL_Y
---| 8 # TALKTYPE_CHANNEL_O
---| 9 # TALKTYPE_SPELL -- Like SAY but with "casts" instead of "says"
---| 10 # TALKTYPE_PRIVATE_NP -- NPC speaking to player
---| 11 # TALKTYPE_PRIVATE_NP_CONSOLE -- NPC channel message, no text on game screen, for sendPrivateMessage use only
---| 12 # TALKTYPE_PRIVATE_PN -- Player speaking to NPC
---| 13 # TALKTYPE_BROADCAST
---| 14 # TALKTYPE_CHANNEL_R1 -- red - #c text
---| 15 # TALKTYPE_PRIVATE_RED_FROM -- @name@text
---| 16 # TALKTYPE_PRIVATE_RED_TO -- @name@text
---| 36 # TALKTYPE_MONSTER_SAY
---| 37 # TALKTYPE_MONSTER_YELL
---| 52 # TALKTYPE_POTION -- Like MONSTER_SAY but can be disabled in client settings
---@source ../src/const.h:247
TALKTYPE_SAY = 1
---@source ../src/const.h:248
TALKTYPE_WHISPER = 2
---@source ../src/const.h:249
TALKTYPE_YELL = 3
---@source ../src/const.h:250
TALKTYPE_PRIVATE_FROM = 4 -- Received private message
---@source ../src/const.h:251
TALKTYPE_PRIVATE_TO = 5   -- Sent private message
---@source ../src/const.h:252
-- TALKTYPE_CHANNEL_M = 6 -- not working (?)
---@source ../src/const.h:253
TALKTYPE_CHANNEL_Y = 7
---@source ../src/const.h:254
TALKTYPE_CHANNEL_O = 8
---@source ../src/const.h:255
TALKTYPE_SPELL = 9               -- Like SAY but with "casts" instead of "says"
---@source ../src/const.h:256
TALKTYPE_PRIVATE_NP = 10         -- NPC speaking to player
---@source ../src/const.h:257
TALKTYPE_PRIVATE_NP_CONSOLE = 11 -- NPC channel message, no text on game screen, for sendPrivateMessage use only
---@source ../src/const.h:258
TALKTYPE_PRIVATE_PN = 12         -- Player speaking to NPC
---@source ../src/const.h:259
TALKTYPE_BROADCAST = 13
---@source ../src/const.h:260
TALKTYPE_CHANNEL_R1 = 14       -- red - #c text
---@source ../src/const.h:261
TALKTYPE_PRIVATE_RED_FROM = 15 -- @name@text
---@source ../src/const.h:262
TALKTYPE_PRIVATE_RED_TO = 16   -- @name@text
---@source ../src/const.h:263
TALKTYPE_MONSTER_SAY = 36
---@source ../src/const.h:264
TALKTYPE_MONSTER_YELL = 37
---@source ../src/const.h:265
TALKTYPE_POTION = 52 -- Like MONSTER_SAY but can be disabled in client settings

VARIANT_NUMBER = 0
VARIANT_POSITION = 1
VARIANT_TARGETPOSITION = 2
VARIANT_STRING = 3

---@alias SpecialSkills number
---| 0 # SPECIALSKILL_CRITICALHITCHANCE
---| 1 # SPECIALSKILL_CRITICALHITAMOUNT
---| 2 # SPECIALSKILL_LIFELEECHCHANCE
---| 3 # SPECIALSKILL_LIFELEECHAMOUNT
---| 4 # SPECIALSKILL_MANALEECHCHANCE
---| 5 # SPECIALSKILL_MANALEECHAMOUNT
---| 0 # SPECIALSKILL_FIRST
---| 5 # SPECIALSKILL_LAST
SPECIALSKILL_CRITICALHITCHANCE = 0
SPECIALSKILL_CRITICALHITAMOUNT = 1
SPECIALSKILL_LIFELEECHCHANCE = 2
SPECIALSKILL_LIFELEECHAMOUNT = 3
SPECIALSKILL_MANALEECHCHANCE = 4
SPECIALSKILL_MANALEECHAMOUNT = 5
SPECIALSKILL_FIRST = SPECIALSKILL_CRITICALHITCHANCE
SPECIALSKILL_LAST = SPECIALSKILL_MANALEECHAMOUNT

SPEECHBUBBLE_NONE = 0
SPEECHBUBBLE_NORMAL = 1
SPEECHBUBBLE_TRADE = 2
SPEECHBUBBLE_QUEST = 3
SPEECHBUBBLE_COMPASS = 4
SPEECHBUBBLE_NORMAL2 = 5
SPEECHBUBBLE_NORMAL3 = 6
SPEECHBUBBLE_HIRELING = 7
SPEECHBUBBLE_LAST = SPEECHBUBBLE_HIRELING

STAT_MAXHITPOINTS = 0
STAT_MAXMANAPOINTS = 1
STAT_SOULPOINTS = 2 -- unused
STAT_MAGICPOINTS = 3
STAT_FIRST = STAT_MAXHITPOINTS
STAT_LAST = STAT_MAGICPOINTS

MONSTERS_EVENT_NONE = 0
MONSTERS_EVENT_THINK = 1
MONSTERS_EVENT_APPEAR = 2
MONSTERS_EVENT_DISAPPEAR = 3
MONSTERS_EVENT_MOVE = 4
MONSTERS_EVENT_SAY = 5

CREATURETYPE_PLAYER = 0
CREATURETYPE_MONSTER = 1
CREATURETYPE_NPC = 2
CREATURETYPE_SUMMON_OWN = 3
CREATURETYPE_SUMMON_OTHERS = 4
CREATURETYPE_HIDDEN = 5

MAX_LOOTCHANCE = 100000

FLUID_EMPTY = 0
FLUID_BLUE = 1
FLUID_RED = 2
FLUID_BROWN = 3
FLUID_GREEN = 4
FLUID_YELLOW = 5
FLUID_WHITE = 6
FLUID_PURPLE = 7
FLUID_BLACK = 8

FLUID_NONE = FLUID_EMPTY
FLUID_WATER = FLUID_BLUE
FLUID_BLOOD = FLUID_RED
FLUID_BEER = FLUID_BROWN
FLUID_SLIME = FLUID_GREEN
FLUID_LEMONADE = FLUID_YELLOW
FLUID_MILK = FLUID_WHITE
FLUID_MANA = FLUID_PURPLE
FLUID_INK = FLUID_BLACK
FLUID_LIFE = FLUID_RED + 8
FLUID_OIL = FLUID_BROWN + 8
FLUID_URINE = FLUID_YELLOW + 8
FLUID_COCONUTMILK = FLUID_WHITE + 8
FLUID_WINE = FLUID_PURPLE + 8
FLUID_MUD = FLUID_BROWN + 16
FLUID_FRUITJUICE = FLUID_YELLOW + 16
FLUID_LAVA = FLUID_RED + 24
FLUID_RUM = FLUID_BROWN + 24
FLUID_SWAMP = FLUID_GREEN + 24
FLUID_TEA = FLUID_BROWN + 32
FLUID_MEAD = FLUID_BROWN + 40

RACE_NONE = 0
RACE_VENOM = 1
RACE_BLOOD = 2
RACE_UNDEAD = 3
RACE_FIRE = 4
RACE_ENERGY = 5
RACE_INK = 6

---@alias GameStates number
---| 0 # GAME_STATE_STARTUP
---| 1 # GAME_STATE_INIT
---| 2 # GAME_STATE_NORMAL
---| 3 # GAME_STATE_CLOSED
---| 4 # GAME_STATE_SHUTDOWN
---| 5 # GAME_STATE_CLOSING
---| 6 # GAME_STATE_MAINTAIN

---@source ../src/game.h:36
GAME_STATE_STARTUP = 0
---@source ../src/game.h:37
GAME_STATE_INIT = 1
---@source ../src/game.h:38
GAME_STATE_NORMAL = 2
---@source ../src/game.h:39
GAME_STATE_CLOSED = 3
---@source ../src/game.h:40
GAME_STATE_SHUTDOWN = 4
---@source ../src/game.h:41
GAME_STATE_CLOSING = 5
---@source ../src/game.h:42
GAME_STATE_MAINTAIN = 6

REPORT_TYPE_NAME = 0
REPORT_TYPE_STATEMENT = 1
REPORT_TYPE_BOT = 2

---@alias TextColors uint8
---| 5 # TEXTCOLOR_BLUE
---| 30 # TEXTCOLOR_LIGHTGREEN
---| 35 # TEXTCOLOR_LIGHTBLUE
---| 86 # TEXTCOLOR_DARKGREY
---| 95 # TEXTCOLOR_MAYABLUE
---| 108 # TEXTCOLOR_DARKRED
---| 129 # TEXTCOLOR_LIGHTGREY
---| 143 # TEXTCOLOR_SKYBLUE
---| 154 # TEXTCOLOR_PURPLE
---| 155 # TEXTCOLOR_ELECTRICPURPLE
---| 180 # TEXTCOLOR_RED
---| 194 # TEXTCOLOR_PASTELRED
---| 198 # TEXTCOLOR_ORANGE
---| 210 # TEXTCOLOR_YELLOW
---| 215 # TEXTCOLOR_WHITE_EXP
---| 255 # TEXTCOLOR_NONE

---@source ../src/const.h:392
TEXTCOLOR_BLUE = 5
---@source ../src/const.h:393
TEXTCOLOR_LIGHTGREEN = 30
---@source ../src/const.h:394
TEXTCOLOR_LIGHTBLUE = 35
---@source ../src/const.h:395
TEXTCOLOR_DARKGREY = 86
---@source ../src/const.h:396
TEXTCOLOR_MAYABLUE = 95
---@source ../src/const.h:397
TEXTCOLOR_DARKRED = 108
---@source ../src/const.h:398
TEXTCOLOR_LIGHTGREY = 129
---@source ../src/const.h:399
TEXTCOLOR_SKYBLUE = 143
---@source ../src/const.h:400
TEXTCOLOR_PURPLE = 154
---@source ../src/const.h:401
TEXTCOLOR_ELECTRICPURPLE = 155
---@source ../src/const.h:402
TEXTCOLOR_RED = 180
---@source ../src/const.h:403
TEXTCOLOR_PASTELRED = 194
---@source ../src/const.h:404
TEXTCOLOR_ORANGE = 198
---@source ../src/const.h:405
TEXTCOLOR_YELLOW = 210
---@source ../src/const.h:406
TEXTCOLOR_WHITE_EXP = 215
---@source ../src/const.h:407
TEXTCOLOR_NONE = 255

SPELLGROUP_NONE = 0
SPELLGROUP_ATTACK = 1
SPELLGROUP_HEALING = 2
SPELLGROUP_SUPPORT = 3
SPELLGROUP_SPECIAL = 4
-- SPELLGROUP_CONJURE = 5
SPELLGROUP_CRIPPLING = 6
SPELLGROUP_FOCUS = 7
SPELLGROUP_ULTIMATESTRIKES = 8

---@alias WorldTypes number
---| 1 # WORLD_TYPE_NO_PVP
---| 2 # WORLD_TYPE_PVP
---| 3 # WORLD_TYPE_PVP_ENFORCED

---@source ../src/game.h:29
WORLD_TYPE_NO_PVP = 1
---@source ../src/game.h:30
WORLD_TYPE_PVP = 2
---@source ../src/game.h:31
WORLD_TYPE_PVP_ENFORCED = 3

---@alias FightModes uint8
---| 1 # FIGHTMODE_ATTACK
---| 2 # FIGHTMODE_BALANCE
---| 3 # FIGHTMODE_DEFENSE

---@source ../src/player.h:35
FIGHTMODE_ATTACK = 1
---@source ../src/player.h:36
FIGHTMODE_BALANCE = 2
---@source ../src/player.h:37
FIGHTMODE_DEFENSE = 3

---@alias SquareColours uint8
---| 0 # SQ_COLOR_BLACK

---@source ../src/const.h:387
SQ_COLOR_BLACK = 0

---@alias ResourceTypes uint8
---| 0 # RESOURCE_BANK_BALANCE
---| 1 # RESOURCE_GOLD_EQUIPPED
---| 10 # RESOURCE_PREY_WILDCARDS
---| 20 # RESOURCE_DAILYREWARD_STREAK
---| 21 # RESOURCE_DAILYREWARD_JOKERS

---@source ../src/const.h:606
RESOURCE_BANK_BALANCE = 0x00
---@source ../src/const.h:607
RESOURCE_GOLD_EQUIPPED = 0x01
---@source ../src/const.h:608
RESOURCE_PREY_WILDCARDS = 0x0A
---@source ../src/const.h:609
RESOURCE_DAILYREWARD_STREAK = 0x14
---@source ../src/const.h:610
RESOURCE_DAILYREWARD_JOKERS = 0x15

---@alias TargetSearchType number
---| 0 # TARGETSEARCH_TYPE_DEFAULT
---| 1 # TARGETSEARCH_TYPE_RANDOM
---| 2 # TARGETSEARCH_TYPE_ATTACKRANGE
---| 3 # TARGETSEARCH_TYPE_NEAREST

---@source ../src/monster.h:21
TARGETSEARCH_DEFAULT = 0
---@source ../src/monster.h:22
TARGETSEARCH_RANDOM = 1
---@source ../src/monster.h:23
TARGETSEARCH_ATTACKRANGE = 2
---@source ../src/monster.h:24
TARGETSEARCH_NEAREST = 3

---@alias MonsterIcons uint8
---| 1 # MONSTER_ICON_VULNERABLE
---| 2 # MONSTER_ICON_WEAKENED
---| 3 # MONSTER_ICON_MELEE
---| 4 # MONSTER_ICON_INFLUENCED
---| 5 # MONSTER_ICON_FIENDISH
---| 1 # MONSTER_ICON_FIRST
---| 5 # MONSTER_ICON_LAST

---@source ../src/const.h:688
MONSTER_ICON_VULNERABLE = 1
---@source ../src/const.h:689
MONSTER_ICON_WEAKENED = 2
---@source ../src/const.h:690
MONSTER_ICON_MELEE = 3
---@source ../src/const.h:691
MONSTER_ICON_INFLUENCED = 4
---@source ../src/const.h:692
MONSTER_ICON_FIENDISH = 5

MONSTER_ICON_FIRST = MONSTER_ICON_VULNERABLE
MONSTER_ICON_LAST =  MONSTER_ICON_FIENDISH

--These aliases are at the bottom so when you "ctrl+click" an alias that uses them, the alias definition will pop up first

---@alias uint8 number @A number between 0 and 255
---@alias uint16 number @A number between 0 and 65535
---@alias uint32 number @A number between 0 and 4294967295
---@alias uint64 number @A number between 0 and 18446744073709551615
---@alias int8 number @A number between -128 and 127
---@alias int16 number @A number between -32768 and 32767
---@alias int32 number @A number between -2147483648 and 2147483647
---@alias int64 number @A number between -9223372036854775808 and 9223372036854775807
---@alias double number @A number between -1.7976931348623157E+308 and 1.7976931348623157E+308