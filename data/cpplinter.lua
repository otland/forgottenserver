---@alias os.mtime fun(): number

---@alias table.create fun(arrayLength: number, keyLength: number): table
---@alias table.pack fun(...): table

---@class rawgetmetatable
---@field __index fun(self: table, key: any): any
rawgetmetatable = {}

---@class bit
---@field band fun(a: number, b: number): number
---@field bor fun(a: number, b: number): number
---@field bxor fun(a: number, b: number): number
---@field lshift fun(a: number, b: number): number
---@field rshift fun(a: number, b: number): number
bit = {}

---@alias Outfit_t table<string, integer>
---@alias MagicEffect_t table<string, integer>

---@class DBInsert
---@field __gc fun(self: DBInsert)
---@field addRow fun(self: DBInsert, row: string): boolean|nil
---@field execute fun(self: DBInsert): boolean|nil
DBInsert = {}

---@class DBTransaction
---@field __eq fun(self: DBTransaction, other: DBTransaction): boolean
---@field __gc fun(self: DBTransaction)
---@field begin fun(self: DBTransaction): boolean|nil
---@field commit fun(self: DBTransaction): boolean|nil
---@field rollback fun(self: DBTransaction)
DBTransaction = {}

---@class db
---@field query fun(query: string): any
---@field storeQuery fun(query: string): any
---@field escapeString fun(value: string): string
---@field asyncQuery fun(query: string): boolean
db = {}

---@class result
---@field free fun(resultId: number)
---@field next fun(resultId: number): number
---@field getNumber fun(resultId: number, column: string): number
---@field getString fun(resultId: number, column: number): string
---@field getBoolean fun(resultId: number, column: number): boolean
---@field getStream fun(resultId: number, column: number): string
result = {}

---@class configManager
---@field getBoolean fun(key: string): boolean
---@field getNumber fun(key: string): number
---@field getString fun(key: string): string
---@field setBoolean fun(key: string, value: boolean): boolean
---@field setNumber fun(key: string, value: number): boolean
---@field setString fun(key: string, value: string): boolean
---@field remove fun(key: string): boolean
---@field getKeys fun(): table
configManager = {}

---@class Game
---@field getSpectators fun(position: Position, multifloor?: boolean, onlyPlayers?: boolean, minRangeX?: number, minRangeY?: number, maxRangeX?: number, maxRangeY?: number): table
---@field getPlayers fun(): table
---@field getNpcs fun(): table
---@field getMonsters fun(): table
---@field loadMap fun(path: string): boolean
---@field getExperienceStage fun(level: number): number
---@field getExperienceForLevel fun(level: number): number
---@field getMonsterCount fun(): number
---@field getPlayerCount fun(): number
---@field getNpcCount fun(): number
---@field getMonsterTypes fun(): table
---@field getBestiary fun(): table
---@field getCurrencyItems fun(): table
---@field getItemTypeByClientId fun(clientId: number): ItemType
---@field getMountIdByLookType fun(lookType: number): number
---@field getTowns fun(): table
---@field getHouses fun(): table
---@field getOutfits fun(sex: number): table
---@field getMounts fun(): table
---@field getVocations fun(): table
---@field getGameState fun(): string
---@field setGameState fun(state: string): boolean
---@field getWorldType fun(): string
---@field setWorldType fun(type: string): boolean
---@field getItemAttributeByName fun(attribute: string): any
---@field getReturnMessage fun(value: number): string
---@field createItem fun(itemId: number, count: number, subtype: number|nil): Item
---@field createContainer fun(containerId: number, size: number): Container
---@field createMonster fun(name: string, position: Position, extended?: boolean, force?: boolean, magicEffect?: MagicEffect_t): Monster
---@field createNpc fun(name: string, position: Position, extended?: boolean, force?: boolean, magicEffect?: MagicEffect_t): Npc
---@field createTile fun(position: Position): Tile
---@field createMonsterType fun(name: string): MonsterType
---@field startEvent fun(eventName: string): boolean
---@field getClientVersion fun(): string
---@field reload fun(reloadType: number): boolean
Game = {}

---@class Variant
---@field create fun(): Variant
---@field getNumber fun(self: Variant): number
---@field getString fun(self: Variant): string
---@field getPosition fun(self: Variant): Position
Variant = {}

---@class Position
---@field create fun(): Position
---@field isSightClear fun(self: Position, other: Position): boolean
---@field sendMagicEffect fun(self: Position, effectType: number, creature?: Creature): boolean
---@field sendDistanceEffect fun(self: Position, target: Position, effectType: number): boolean
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
---@field addItem fun(self: Tile, item: Item): boolean
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
---@field isPlayer fun(self: Player): boolean
---@field getGuid fun(self: Player): number
---@field getIp fun(self: Player): number
---@field getAccountId fun(self: Player): number
---@field getLastLoginSaved fun(self: Player): number
---@field getLastLogout fun(self: Player): number
---@field getAccountType fun(self: Player): number
---@field setAccountType fun(self: Player, accountType: number)
---@field getCapacity fun(self: Player): number
---@field setCapacity fun(self: Player, capacity: number)
---@field getFreeCapacity fun(self: Player): number
---@field getDepotChest fun(self: Player, depotId: number, autoCreate?: boolean): Item
---@field getInbox fun(self: Player): Item
---@field getSkullTime fun(self: Player): number
---@field setSkullTime fun(self: Player, time: number)
---@field getDeathPenalty fun(self: Player): number
---@field getExperience fun(self: Player): number
---@field addExperience fun(self: Player, amount: number)
---@field removeExperience fun(self: Player, amount: number)
---@field getLevel fun(self: Player): number
---@field getLevelPercent fun(self: Player): number
---@field getMagicLevel fun(self: Player): number
---@field getMagicLevelPercent fun(self: Player): number
---@field getBaseMagicLevel fun(self: Player): number
---@field getMana fun(self: Player): number
---@field addMana fun(self: Player, amount: number)
---@field getMaxMana fun(self: Player): number
---@field setMaxMana fun(self: Player, maxMana: number)
---@field setManaShieldBar fun(self: Player, visible: boolean)
---@field getManaSpent fun(self: Player): number
---@field addManaSpent fun(self: Player, amount: number)
---@field removeManaSpent fun(self: Player, amount: number)
---@field getBaseMaxHealth fun(self: Player): number
---@field getBaseMaxMana fun(self: Player): number
---@field getSkillLevel fun(self: Player, skill: number): number
---@field getEffectiveSkillLevel fun(self: Player, skill: number): number
---@field getSkillPercent fun(self: Player, skill: number): number
---@field getSkillTries fun(self: Player, skill: number): number
---@field addSkillTries fun(self: Player, skill: number, amount: number)
---@field removeSkillTries fun(self: Player, skill: number, amount: number, notify?: boolean)
---@field getSpecialSkill fun(self: Player, skill: number): number
---@field addSpecialSkill fun(self: Player, skill: number, amount: number)
---@field addOfflineTrainingTime fun(self: Player, time: number)
---@field getOfflineTrainingTime fun(self: Player): number
---@field removeOfflineTrainingTime fun(self: Player, time: number)
---@field addOfflineTrainingTries fun(self: Player, skill: number, amount: number)
---@field getOfflineTrainingSkill fun(self: Player): number
---@field setOfflineTrainingSkill fun(self: Player, skill: number)
---@field getItemCount fun(self: Player, itemId: number): number
---@field getItemById fun(self: Player, itemId: number, subType?: number): Item
---@field getVocation fun(self: Player): Vocation
---@field setVocation fun(self: Player, vocationId: number)
---@field getSex fun(self: Player): number
---@field setSex fun(self: Player, sexId: number)
---@field getTown fun(self: Player): number
---@field setTown fun(self: Player, townId: number)
---@field getGuild fun(self: Player): Guild
---@field setGuild fun(self: Player, guild: Guild)
---@field getGuildLevel fun(self: Player): number
---@field setGuildLevel fun(self: Player, level: number)
---@field getGuildNick fun(self: Player): string
---@field setGuildNick fun(self: Player, nick: string)
---@field getGroup fun(self: Player): Group
---@field setGroup fun(self: Player, groupId: number)
---@field getStamina fun(self: Player): number
---@field setStamina fun(self: Player, stamina: number)
---@field getSoul fun(self: Player): number
---@field addSoul fun(self: Player, amount: number)
---@field getMaxSoul fun(self: Player): number
---@field getBankBalance fun(self: Player): number
---@field setBankBalance fun(self: Player, balance: number)
---@field addItem fun(self: Player, itemId: number, count?: number): Item
---@field addItemEx fun(self: Player, item: Item, copyItem?: boolean): boolean
---@field removeItem fun(self: Player, itemId: number, count?: number, subtype?: number): boolean
---@field sendSupplyUsed fun(self: Player, item: Item)
---@field getMoney fun(self: Player): number
---@field addMoney fun(self: Player, amount: number)
---@field removeMoney fun(self: Player, amount: number)
---@field showTextDialog fun(self: Player, itemId: number, text: string)
---@field sendTextMessage fun(self: Player, messageType: number, message: string, position?: Position|number, primaryValue?: number, primaryColor?: number, secondaryValue?: number, secondaryColor?: number)
---@field sendChannelMessage fun(self: Player, author: string, message: string, type: number, channelId: number)
---@field sendPrivateMessage fun(self: Player, receiver: Player, message: string)
---@field channelSay fun(self: Player, channelId: number, message: string)
---@field openChannel fun(self: Player, channelId: number)
---@field getSlotItem fun(self: Player, slot: number): Item
---@field getParty fun(self: Player): Party
---@field addOutfit fun(self: Player, outfitId: number)
---@field addOutfitAddon fun(self: Player, outfitId: number, addonId: number)
---@field removeOutfit fun(self: Player, outfitId: number)
---@field removeOutfitAddon fun(self: Player, outfitId: number, addonId: number)
---@field hasOutfit fun(self: Player, outfitId: number, addon?: number): boolean
---@field canWearOutfit fun(self: Player, outfitId: number, addonId?: number): boolean
---@field sendOutfitWindow fun(self: Player)
---@field sendEditPodium fun(self: Player, item: Item)
---@field addMount fun(self: Player, mountId: number)
---@field removeMount fun(self: Player, mountId: number)
---@field hasMount fun(self: Player, mountId: number): boolean
---@field toggleMount fun(self: Player, active: boolean)
---@field getPremiumEndsAt fun(self: Player): number
---@field setPremiumEndsAt fun(self: Player, timestamp: number)
---@field hasBlessing fun(self: Player, blessingId: number): boolean
---@field addBlessing fun(self: Player, blessingId: number)
---@field removeBlessing fun(self: Player, blessingId: number)
---@field canLearnSpell fun(self: Player, spellId: number): boolean
---@field learnSpell fun(self: Player, spellId: number)
---@field forgetSpell fun(self: Player, spellId: number)
---@field hasLearnedSpell fun(self: Player, spellId: number): boolean
---@field sendTutorial fun(self: Player, tutorialId: number)
---@field addMapMark fun(self: Player, position: Position, type: number, description?: string)
---@field save fun(self: Player)
---@field popupFYI fun(self: Player, message: string)
---@field isPzLocked fun(self: Player): boolean
---@field getClient fun(self: Player): table
---@field getHouse fun(self: Player): House
---@field sendHouseWindow fun(self: Player, houseId: number)
---@field setEditHouse fun(self: Player, houseId: number, editMode: boolean)
---@field setGhostMode fun(self: Player, active: boolean)
---@field getContainerId fun(self: Player, index: number): number
---@field getContainerById fun(self: Player, containerId: number): Container
---@field getContainerIndex fun(self: Player, containerId: number): number
---@field getInstantSpells fun(self: Player): table
---@field canCast fun(self: Player, spellId: number): boolean
---@field hasChaseMode fun(self: Player): boolean
---@field hasSecureMode fun(self: Player): boolean
---@field getFightMode fun(self: Player): number
---@field getStoreInbox fun(self: Player): Container
---@field isNearDepotBox fun(self: Player): boolean
---@field getIdleTime fun(self: Player): number
---@field resetIdleTime fun(self: Player)
---@field sendCreatureSquare fun(self: Player, creature: Creature, color: number)
---@field getClientExpDisplay fun(self: Player): number
---@field setClientExpDisplay fun(self: Player, display: number)
---@field getClientStaminaBonusDisplay fun(self: Player): number
---@field setClientStaminaBonusDisplay fun(self: Player, display: number)
---@field getClientLowLevelBonusDisplay fun(self: Player): number
---@field setClientLowLevelBonusDisplay fun(self: Player, display: number)
Player = {}

---@class Monster : Creature
---@field create fun(): Monster
---@field __eq fun(self: Monster, other: Monster): boolean
---@field isMonster fun(self: Monster): boolean
---@field getId fun(self: Monster): number
---@field getType fun(self: Monster): string
---@field rename fun(self: Monster, newName: string)
---@field getSpawnPosition fun(self: Monster): Position
---@field isInSpawnRange fun(self: Monster, position: Position): boolean
---@field isIdle fun(self: Monster): boolean
---@field setIdle fun(self: Monster, idle: boolean)
---@field isTarget fun(self: Monster, creature: Creature): boolean
---@field isOpponent fun(self: Monster, creature: Creature): boolean
---@field isFriend fun(self: Monster, creature: Creature): boolean
---@field addFriend fun(self: Monster, creature: Creature)
---@field removeFriend fun(self: Monster, creature: Creature)
---@field getFriendList fun(self: Monster): table
---@field getFriendCount fun(self: Monster): number
---@field addTarget fun(self: Monster, creature: Creature)
---@field removeTarget fun(self: Monster, creature: Creature)
---@field getTargetList fun(self: Monster): table
---@field getTargetCount fun(self: Monster): number
---@field selectTarget fun(self: Monster, creature: Creature): boolean
---@field searchTarget fun(self: Monster): boolean
---@field isWalkingToSpawn fun(self: Monster): boolean
---@field walkToSpawn fun(self: Monster)
---@field hasSpecialIcon fun(self: Monster): boolean
---@field setSpecialIcon fun(self: Monster, iconId: number)
---@field getSpecialIcon fun(self: Monster): number
---@field removeSpecialIcon fun(self: Monster)
Monster = {}

---@class Npc : Creature
---@field create fun(): Npc
---@field __eq fun(self: Npc, other: Npc): boolean
---@field isNpc fun(self: Npc): boolean
---@field setMasterPos fun(self: Npc, position: Position)
---@field getSpeechBubble fun(self: Npc): number
---@field setSpeechBubble fun(self: Npc, bubbleType: number)
---@field getSpectators fun(self: Npc, centerPos: Position, rangeX: number, rangeY: number, multifloor: boolean): table
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

-- Constants: CONST_ME
CONST_ME_NONE = 0
CONST_ME_DRAWBLOOD = 1
CONST_ME_LOSEENERGY = 2
CONST_ME_POFF = 3
CONST_ME_BLOCKHIT = 4
CONST_ME_EXPLOSIONAREA = 5
CONST_ME_EXPLOSIONHIT = 6
CONST_ME_FIREAREA = 7
CONST_ME_YELLOW_RINGS = 8
CONST_ME_GREEN_RINGS = 9
CONST_ME_HITAREA = 10
CONST_ME_TELEPORT = 11
CONST_ME_ENERGYHIT = 12
CONST_ME_MAGIC_BLUE = 13
CONST_ME_MAGIC_RED = 14
CONST_ME_MAGIC_GREEN = 15
CONST_ME_HITBYFIRE = 16
CONST_ME_HITBYPOISON = 17
CONST_ME_MORTAREA = 18
CONST_ME_SOUND_GREEN = 19
CONST_ME_SOUND_RED = 20
CONST_ME_POISONAREA = 21
CONST_ME_SOUND_YELLOW = 22
CONST_ME_SOUND_PURPLE = 23
CONST_ME_SOUND_BLUE = 24
CONST_ME_SOUND_WHITE = 25
CONST_ME_BUBBLES = 26
CONST_ME_CRAPS = 27
CONST_ME_GIFT_WRAPS = 28
CONST_ME_FIREWORK_YELLOW = 29
CONST_ME_FIREWORK_RED = 30
CONST_ME_FIREWORK_BLUE = 31
CONST_ME_STUN = 32
CONST_ME_SLEEP = 33
CONST_ME_WATERCREATURE = 34
CONST_ME_GROUNDSHAKER = 35
CONST_ME_HEARTS = 36
CONST_ME_FIREATTACK = 37
CONST_ME_ENERGYAREA = 38
CONST_ME_SMALLCLOUDS = 39
CONST_ME_HOLYDAMAGE = 40
CONST_ME_BIGCLOUDS = 41
CONST_ME_ICEAREA = 42
CONST_ME_ICETORNADO = 43
CONST_ME_ICEATTACK = 44
CONST_ME_STONES = 45
CONST_ME_SMALLPLANTS = 46
CONST_ME_CARNIPHILA = 47
CONST_ME_PURPLEENERGY = 48
CONST_ME_YELLOWENERGY = 49
CONST_ME_HOLYAREA = 50
CONST_ME_BIGPLANTS = 51
CONST_ME_CAKE = 52
CONST_ME_GIANTICE = 53
CONST_ME_WATERSPLASH = 54
CONST_ME_PLANTATTACK = 55
CONST_ME_TUTORIALARROW = 56
CONST_ME_TUTORIALSQUARE = 57
CONST_ME_MIRRORHORIZONTAL = 58
CONST_ME_MIRRORVERTICAL = 59
CONST_ME_SKULLHORIZONTAL = 60
CONST_ME_SKULLVERTICAL = 61
CONST_ME_ASSASSIN = 62
CONST_ME_STEPSHORIZONTAL = 63
CONST_ME_BLOODYSTEPS = 64
CONST_ME_STEPSVERTICAL = 65
CONST_ME_YALAHARIGHOST = 66
CONST_ME_BATS = 67
CONST_ME_SMOKE = 68
CONST_ME_INSECTS = 69
CONST_ME_DRAGONHEAD = 70
CONST_ME_ORCSHAMAN = 71
CONST_ME_ORCSHAMAN_FIRE = 72
CONST_ME_THUNDER = 73
CONST_ME_FERUMBRAS = 74
CONST_ME_CONFETTI_HORIZONTAL = 75
CONST_ME_CONFETTI_VERTICAL = 76
-- 77-157 are empty
CONST_ME_BLACKSMOKE = 158
-- 159-166 are empty
CONST_ME_REDSMOKE = 167
CONST_ME_YELLOWSMOKE = 168
CONST_ME_GREENSMOKE = 169
CONST_ME_PURPLESMOKE = 170
CONST_ME_EARLY_THUNDER = 171
CONST_ME_RAGIAZ_BONECAPSULE = 172
CONST_ME_CRITICAL_DAMAGE = 173
-- 174 is empty
CONST_ME_PLUNGING_FISH = 175
CONST_ME_BLUECHAIN = 176
CONST_ME_ORANGECHAIN = 177
CONST_ME_GREENCHAIN = 178
CONST_ME_PURPLECHAIN = 179
CONST_ME_GREYCHAIN = 180
CONST_ME_YELLOWCHAIN = 181
CONST_ME_YELLOWSPARKLES = 182
-- 183 is empty
CONST_ME_FAEEXPLOSION = 184
CONST_ME_FAECOMING = 185
CONST_ME_FAEGOING = 186
-- 187 is empty
CONST_ME_BIGCLOUDSSINGLESPACE = 188
CONST_ME_STONESSINGLESPACE = 189
-- 190 is empty
CONST_ME_BLUEGHOST = 191
-- 192 is empty
CONST_ME_POINTOFINTEREST = 193
CONST_ME_MAPEFFECT = 194
CONST_ME_PINKSPARK = 195
CONST_ME_FIREWORK_GREEN = 196
CONST_ME_FIREWORK_ORANGE = 197
CONST_ME_FIREWORK_PURPLE = 198
CONST_ME_FIREWORK_TURQUOISE = 199
-- 200 is empty
CONST_ME_THECUBE = 201
CONST_ME_DRAWINK = 202
CONST_ME_PRISMATICSPARKLES = 203
CONST_ME_THAIAN = 204
CONST_ME_THAIANGHOST = 205
CONST_ME_GHOSTSMOKE = 206
-- 207 is empty
CONST_ME_FLOATINGBLOCK = 208
CONST_ME_BLOCK = 209
CONST_ME_ROOTING = 210
-- 211-212 were removed from the client
CONST_ME_GHOSTLYSCRATCH = 213
CONST_ME_GHOSTLYBITE = 214
CONST_ME_BIGSCRATCHING = 215
CONST_ME_SLASH = 216
CONST_ME_BITE = 217
-- 218 is empty
CONST_ME_CHIVALRIOUSCHALLENGE = 219
CONST_ME_DIVINEDAZZLE = 220
CONST_ME_ELECTRICALSPARK = 221
CONST_ME_PURPLETELEPORT = 222
CONST_ME_REDTELEPORT = 223
CONST_ME_ORANGETELEPORT = 224
CONST_ME_GREYTELEPORT = 225
CONST_ME_LIGHTBLUETELEPORT = 226
-- 227-229 are empty
CONST_ME_FATAL = 230
CONST_ME_DODGE = 231
CONST_ME_HOURGLASS = 232
CONST_ME_FIREWORKSSTAR = 233
CONST_ME_FIREWORKSCIRCLE = 234
CONST_ME_FERUMBRAS_1 = 235
CONST_ME_GAZHARAGOTH = 236
CONST_ME_MAD_MAGE = 237
CONST_ME_HORESTIS = 238
CONST_ME_DEVOVORGA = 239
CONST_ME_FERUMBRAS_2 = 240
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

-- Constants: CONST_SLOT
CONST_SLOT_WHEREEVER = 0
CONST_SLOT_HEAD = 1
CONST_SLOT_NECKLACE = 2
CONST_SLOT_BACKPACK = 3
CONST_SLOT_ARMOR = 4
CONST_SLOT_RIGHT = 5
CONST_SLOT_LEFT = 6
CONST_SLOT_LEGS = 7
CONST_SLOT_FEET = 8
CONST_SLOT_RING = 9
CONST_SLOT_AMMO = 10
CONST_SLOT_STORE_INBOX = 11
CONST_SLOT_FIRST = CONST_SLOT_HEAD
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

ACCOUNT_TYPE_NORMAL = 1
ACCOUNT_TYPE_TUTOR = 2
ACCOUNT_TYPE_SENIORTUTOR = 3
ACCOUNT_TYPE_GAMEMASTER = 4
ACCOUNT_TYPE_COMMUNITYMANAGER = 5
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

SKULL_NONE = 0
SKULL_YELLOW = 1
SKULL_GREEN = 2
SKULL_WHITE = 3
SKULL_RED = 4
SKULL_BLACK = 5
SKULL_ORANGE = 6

MESSAGE_STATUS_DEFAULT = 17  -- White, bottom + console
MESSAGE_STATUS_WARNING = 18  -- Red, over player + console
MESSAGE_EVENT_ADVANCE = 19   -- White, over player + console
MESSAGE_STATUS_WARNING2 = 20 -- Red, over player + console
MESSAGE_STATUS_SMALL = 21    -- White, bottom of the screen
MESSAGE_INFO_DESCR = 22      -- Green, over player + console
MESSAGE_DAMAGE_DEALT = 23
MESSAGE_DAMAGE_RECEIVED = 24
MESSAGE_HEALED = 25
MESSAGE_EXPERIENCE = 26
MESSAGE_DAMAGE_OTHERS = 27
MESSAGE_HEALED_OTHERS = 28
MESSAGE_EXPERIENCE_OTHERS = 29
MESSAGE_EVENT_DEFAULT = 30 -- White, bottom + console
MESSAGE_LOOT = 31          -- White, over player + console, supports colors as {text|itemClientId}
MESSAGE_TRADE = 32         -- Green, over player + console
MESSAGE_GUILD = 33
MESSAGE_PARTY_MANAGEMENT = 34
MESSAGE_PARTY = 35
MESSAGE_REPORT = 38         -- White, over player + conosle
MESSAGE_HOTKEY_PRESSED = 39 -- Green, over player + console
-- MESSAGE_TUTORIAL_HINT = 40 -- not working (?)
-- MESSAGE_THANK_YOU = 41 -- not working (?)
MESSAGE_MARKET = 42 -- Window "Market Message" + "Ok" button
-- MESSAGE_MANA = 43 -- not working (?)
MESSAGE_BEYOND_LAST = 44     -- White, console only
MESSAGE_TOURNAMENT_INFO = 45 -- Window "Tournament" + "Ok" button
-- unused 46?
-- unused 47?
MESSAGE_ATTENTION = 48        -- White, console only
MESSAGE_BOOSTED_CREATURE = 49 -- White, console only
MESSAGE_OFFLINE_TRAINING = 50 -- White, over player + console
MESSAGE_TRANSACTION = 51      -- White, console only

INDEX_WHEREEVER = -1
VIRTUAL_PARENT = true

FLAG_NOLIMIT = 1 * 2 ^ 0             -- Bypass limits like capacity/container limits, blocking items/creatures etc.
FLAG_IGNOREBLOCKITEM = 1 * 2 ^ 1     -- Bypass movable blocking item checks
FLAG_IGNOREBLOCKCREATURE = 1 * 2 ^ 2 -- Bypass creature checks
FLAG_CHILDISOWNER = 1 * 2 ^ 3        -- Used by containers to query capacity of the carrier (player)
FLAG_PATHFINDING = 1 * 2 ^ 4         -- An additional check is done for floor changing/teleport items
FLAG_IGNOREFIELDDAMAGE = 1 * 2 ^ 5   -- Bypass field damage checks
FLAG_IGNORENOTMOVEABLE = 1 * 2 ^ 6   -- Bypass check for mobility
FLAG_IGNOREAUTOSTACK = 1 * 2 ^ 7     -- queryDestination will not try to stack items together

RETURNVALUE_NOERROR = 0
RETURNVALUE_NOTPOSSIBLE = 1
RETURNVALUE_NOTENOUGHROOM = 2
RETURNVALUE_PLAYERISPZLOCKED = 3
RETURNVALUE_PLAYERISNOTINVITED = 4
RETURNVALUE_CANNOTTHROW = 5
RETURNVALUE_THEREISNOWAY = 6
RETURNVALUE_DESTINATIONOUTOFREACH = 7
RETURNVALUE_CREATUREBLOCK = 8
RETURNVALUE_NOTMOVEABLE = 9
RETURNVALUE_DROPTWOHANDEDITEM = 10
RETURNVALUE_BOTHHANDSNEEDTOBEFREE = 11
RETURNVALUE_CANONLYUSEONEWEAPON = 12
RETURNVALUE_NEEDEXCHANGE = 13
RETURNVALUE_CANNOTBEDRESSED = 14
RETURNVALUE_PUTTHISOBJECTINYOURHAND = 15
RETURNVALUE_PUTTHISOBJECTINBOTHHANDS = 16
RETURNVALUE_TOOFARAWAY = 17
RETURNVALUE_FIRSTGODOWNSTAIRS = 18
RETURNVALUE_FIRSTGOUPSTAIRS = 19
RETURNVALUE_CONTAINERNOTENOUGHROOM = 20
RETURNVALUE_NOTENOUGHCAPACITY = 21
RETURNVALUE_CANNOTPICKUP = 22
RETURNVALUE_THISISIMPOSSIBLE = 23
RETURNVALUE_DEPOTISFULL = 24
RETURNVALUE_CREATUREDOESNOTEXIST = 25
RETURNVALUE_CANNOTUSETHISOBJECT = 26
RETURNVALUE_PLAYERWITHTHISNAMEISNOTONLINE = 27
RETURNVALUE_NOTREQUIREDLEVELTOUSERUNE = 28
RETURNVALUE_YOUAREALREADYTRADING = 29
RETURNVALUE_THISPLAYERISALREADYTRADING = 30
RETURNVALUE_YOUMAYNOTLOGOUTDURINGAFIGHT = 31
RETURNVALUE_DIRECTPLAYERSHOOT = 32
RETURNVALUE_NOTENOUGHLEVEL = 33
RETURNVALUE_NOTENOUGHMAGICLEVEL = 34
RETURNVALUE_NOTENOUGHMANA = 35
RETURNVALUE_NOTENOUGHSOUL = 36
RETURNVALUE_YOUAREEXHAUSTED = 37
RETURNVALUE_YOUCANNOTUSEOBJECTSTHATFAST = 38
RETURNVALUE_PLAYERISNOTREACHABLE = 39
RETURNVALUE_CANONLYUSETHISRUNEONCREATURES = 40
RETURNVALUE_ACTIONNOTPERMITTEDINPROTECTIONZONE = 41
RETURNVALUE_YOUMAYNOTATTACKTHISPLAYER = 42
RETURNVALUE_YOUMAYNOTATTACKAPERSONINPROTECTIONZONE = 43
RETURNVALUE_YOUMAYNOTATTACKAPERSONWHILEINPROTECTIONZONE = 44
RETURNVALUE_YOUMAYNOTATTACKTHISCREATURE = 45
RETURNVALUE_YOUCANONLYUSEITONCREATURES = 46
RETURNVALUE_CREATUREISNOTREACHABLE = 47
RETURNVALUE_TURNSECUREMODETOATTACKUNMARKEDPLAYERS = 48
RETURNVALUE_YOUNEEDPREMIUMACCOUNT = 49
RETURNVALUE_YOUNEEDTOLEARNTHISSPELL = 50
RETURNVALUE_YOURVOCATIONCANNOTUSETHISSPELL = 51
RETURNVALUE_YOUNEEDAWEAPONTOUSETHISSPELL = 52
RETURNVALUE_PLAYERISPZLOCKEDLEAVEPVPZONE = 53
RETURNVALUE_PLAYERISPZLOCKEDENTERPVPZONE = 54
RETURNVALUE_ACTIONNOTPERMITTEDINANOPVPZONE = 55
RETURNVALUE_YOUCANNOTLOGOUTHERE = 56
RETURNVALUE_YOUNEEDAMAGICITEMTOCASTSPELL = 57
RETURNVALUE_NAMEISTOOAMBIGUOUS = 58
RETURNVALUE_CANONLYUSEONESHIELD = 59
RETURNVALUE_NOPARTYMEMBERSINRANGE = 60
RETURNVALUE_YOUARENOTTHEOWNER = 61
RETURNVALUE_TRADEPLAYERFARAWAY = 62
RETURNVALUE_YOUDONTOWNTHISHOUSE = 63
RETURNVALUE_TRADEPLAYERALREADYOWNSAHOUSE = 64
RETURNVALUE_TRADEPLAYERHIGHESTBIDDER = 65
RETURNVALUE_YOUCANNOTTRADETHISHOUSE = 66
RETURNVALUE_YOUDONTHAVEREQUIREDPROFESSION = 67
RETURNVALUE_CANNOTMOVEITEMISNOTSTOREITEM = 68
RETURNVALUE_ITEMCANNOTBEMOVEDTHERE = 69
RETURNVALUE_YOUCANNOTUSETHISBED = 70
RETURNVALUE_QUIVERAMMOONLY = 71

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

WEAPON_NONE = 0
WEAPON_SWORD = 1
WEAPON_CLUB = 2
WEAPON_AXE = 3
WEAPON_SHIELD = 4
WEAPON_DISTANCE = 5
WEAPON_WAND = 6
WEAPON_AMMO = 7
WEAPON_QUIVER = 8

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

ORIGIN_NONE = 0
ORIGIN_CONDITION = 1
ORIGIN_SPELL = 2
ORIGIN_MELEE = 3
ORIGIN_RANGED = 4
ORIGIN_WAND = 5
ORIGIN_REFLECT = 6

PLAYERSEX_FEMALE = 0
PLAYERSEX_MALE = 1

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

RELOAD_TYPE_ALL = 0
RELOAD_TYPE_ACTIONS = 1
RELOAD_TYPE_CHAT = 2
RELOAD_TYPE_CONFIG = 3
RELOAD_TYPE_CREATURESCRIPTS = 4
RELOAD_TYPE_EVENTS = 5
RELOAD_TYPE_GLOBAL = 6
RELOAD_TYPE_GLOBALEVENTS = 7
RELOAD_TYPE_ITEMS = 8
RELOAD_TYPE_MONSTERS = 9
RELOAD_TYPE_MOUNTS = 10
RELOAD_TYPE_MOVEMENTS = 11
RELOAD_TYPE_NPCS = 12
RELOAD_TYPE_QUESTS = 13
RELOAD_TYPE_SCRIPTS = 14
RELOAD_TYPE_SPELLS = 15
RELOAD_TYPE_TALKACTIONS = 16
RELOAD_TYPE_WEAPONS = 17

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

ITEM_ATTRIBUTE_NONE = 0

ITEM_ATTRIBUTE_ACTIONID = 1 * 2 ^ 0
ITEM_ATTRIBUTE_UNIQUEID = 1 * 2 ^ 1
ITEM_ATTRIBUTE_DESCRIPTION = 1 * 2 ^ 2
ITEM_ATTRIBUTE_TEXT = 1 * 2 ^ 3
ITEM_ATTRIBUTE_DATE = 1 * 2 ^ 4
ITEM_ATTRIBUTE_WRITER = 1 * 2 ^ 5
ITEM_ATTRIBUTE_NAME = 1 * 2 ^ 6
ITEM_ATTRIBUTE_ARTICLE = 1 * 2 ^ 7
ITEM_ATTRIBUTE_PLURALNAME = 1 * 2 ^ 8
ITEM_ATTRIBUTE_WEIGHT = 1 * 2 ^ 9
ITEM_ATTRIBUTE_ATTACK = 1 * 2 ^ 10
ITEM_ATTRIBUTE_DEFENSE = 1 * 2 ^ 11
ITEM_ATTRIBUTE_EXTRADEFENSE = 1 * 2 ^ 12
ITEM_ATTRIBUTE_ARMOR = 1 * 2 ^ 13
ITEM_ATTRIBUTE_HITCHANCE = 1 * 2 ^ 14
ITEM_ATTRIBUTE_SHOOTRANGE = 1 * 2 ^ 15
ITEM_ATTRIBUTE_OWNER = 1 * 2 ^ 16
ITEM_ATTRIBUTE_DURATION = 1 * 2 ^ 17
ITEM_ATTRIBUTE_DECAYSTATE = 1 * 2 ^ 18
ITEM_ATTRIBUTE_CORPSEOWNER = 1 * 2 ^ 19
ITEM_ATTRIBUTE_CHARGES = 1 * 2 ^ 20
ITEM_ATTRIBUTE_FLUIDTYPE = 1 * 2 ^ 21
ITEM_ATTRIBUTE_DOORID = 1 * 2 ^ 22
ITEM_ATTRIBUTE_DECAYTO = 1 * 2 ^ 23
ITEM_ATTRIBUTE_WRAPID = 1 * 2 ^ 24
ITEM_ATTRIBUTE_STOREITEM = 1 * 2 ^ 25
ITEM_ATTRIBUTE_ATTACK_SPEED = 1 * 2 ^ 26
ITEM_ATTRIBUTE_OPENCONTAINER = 1 * 2 ^ 27
ITEM_ATTRIBUTE_DURATION_MIN = ITEM_ATTRIBUTE_DURATION
ITEM_ATTRIBUTE_DURATION_MAX = 1 * 2 ^ 27
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
	STATUS_PORT = 24,
	HTTP_PORT = 25,
	HTTP_WORKERS = 26,
	STAIRHOP_DELAY = 27,
	MARKET_OFFER_DURATION = 28,
	CHECK_EXPIRED_MARKET_OFFERS_EACH_MINUTES = 29,
	MAX_MARKET_OFFERS_AT_A_TIME_PER_PLAYER = 30,
	EXP_FROM_PLAYERS_LEVEL_RANGE = 31,
	MAX_PACKETS_PER_SECOND = 32,
	SERVER_SAVE_NOTIFY_DURATION = 33,
	YELL_MINIMUM_LEVEL = 34,
	MINIMUM_LEVEL_TO_SEND_PRIVATE = 35,
	VIP_FREE_LIMIT = 36,
	VIP_PREMIUM_LIMIT = 37,
	DEPOT_FREE_LIMIT = 38,
	DEPOT_PREMIUM_LIMIT = 39,
	QUEST_TRACKER_FREE_LIMIT = 40,
	QUEST_TRACKER_PREMIUM_LIMIT = 41,
	STAMINA_REGEN_MINUTE = 42,
	STAMINA_REGEN_PREMIUM = 43,
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

TALKTYPE_SAY = 1
TALKTYPE_WHISPER = 2
TALKTYPE_YELL = 3
TALKTYPE_PRIVATE_FROM = 4 -- Received private message
TALKTYPE_PRIVATE_TO = 5   -- Sent private message
-- TALKTYPE_CHANNEL_M = 6 -- not working (?)
TALKTYPE_CHANNEL_Y = 7
TALKTYPE_CHANNEL_O = 8
TALKTYPE_SPELL = 9               -- Like SAY but with "casts" instead of "says"
TALKTYPE_PRIVATE_NP = 10         -- NPC speaking to player
TALKTYPE_PRIVATE_NP_CONSOLE = 11 -- NPC channel message, no text on game screen, for sendPrivateMessage use only
TALKTYPE_PRIVATE_PN = 12         -- Player speaking to NPC
TALKTYPE_BROADCAST = 13
TALKTYPE_CHANNEL_R1 = 14       -- red - #c text
TALKTYPE_PRIVATE_RED_FROM = 15 -- @name@text
TALKTYPE_PRIVATE_RED_TO = 16   -- @name@text
TALKTYPE_MONSTER_SAY = 36
TALKTYPE_MONSTER_YELL = 37
TALKTYPE_POTION = 52 -- Like MONSTER_SAY but can be disabled in client settings

VARIANT_NUMBER = 0
VARIANT_POSITION = 1
VARIANT_TARGETPOSITION = 2
VARIANT_STRING = 3

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

GAME_STATE_STARTUP = 0
GAME_STATE_INIT = 1
GAME_STATE_NORMAL = 2
GAME_STATE_CLOSED = 3
GAME_STATE_SHUTDOWN = 4
GAME_STATE_CLOSING = 5
GAME_STATE_MAINTAIN = 6

REPORT_TYPE_NAME = 0
REPORT_TYPE_STATEMENT = 1
REPORT_TYPE_BOT = 2

TEXTCOLOR_BLUE = 5
TEXTCOLOR_LIGHTGREEN = 30
TEXTCOLOR_LIGHTBLUE = 35
TEXTCOLOR_DARKGREY = 86
TEXTCOLOR_MAYABLUE = 95
TEXTCOLOR_DARKRED = 108
TEXTCOLOR_LIGHTGREY = 129
TEXTCOLOR_SKYBLUE = 143
TEXTCOLOR_PURPLE = 154
TEXTCOLOR_ELECTRICPURPLE = 155
TEXTCOLOR_RED = 180
TEXTCOLOR_PASTELRED = 194
TEXTCOLOR_ORANGE = 198
TEXTCOLOR_YELLOW = 210
TEXTCOLOR_WHITE_EXP = 215
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

WORLD_TYPE_NO_PVP = 1
WORLD_TYPE_PVP = 2
WORLD_TYPE_PVP_ENFORCED = 3
