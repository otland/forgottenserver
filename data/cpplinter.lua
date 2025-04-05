---@meta

---@alias os.mtime fun(): number

---@alias table.create fun(arrayLength: number, keyLength: number): table
---@alias table.pack fun(...): table

--- UserClientVersion is a table that contains information about the client version of the user. 
--- <br>
--- It will be returned when using player:getClient() 
--- <br><br>
--- Example Usage:
--- ```lua
--- local clientTable = player:getClient()
--- print("Client Version: " .. clientTable.version) -- Example: 1310
--- print("Client OS: " .. clientTable.os) -- Example: 2 (CLIENTOS_WINDOWS)
--- ```
--- <br>
---@class UserClientVersion
---@field version uint16 The version number of the client
---@field os OperatingSystem The operating system of the user 

--- SpellsTable is a table that contains a small subset of information about an instant spell. 
--- <br>
--- It is returned when using player:getInstantSpells() 
--- <br><br>
--- Example Usage:
--- ```lua
---local playerSpells = player:getInstantSpells() -- Note: for this example, this player only has "Light Healing"
---for i, spell in ipairs(playerSpells) do
---    print("Spell Name: " .. spell.name) -- Light Healing
---    print("Spell Words: " .. spell.words) -- exura
---    print("Spell Level: " .. spell.level) -- 8
---    print("Spell Magic Level: " .. spell.mlevel) -- 0
---    print("Spell Mana: " .. spell.mana) -- 0
---    print("Spell Mana Percent: " .. spell.manaPercent) -- 0
---    print("Spell Params: " .. spell.params) -- false
---end -- As per this example, this for loop will only run once 
--- ```
--- <br>
---@class SpellsTable
---@field name string The name of the spell
---@field words string The words used to cast the spell
---@field level uint32 The level required to cast the spell
---@field mlevel uint32 The magic level required to cast the spell
---@field mana uint32 The mana cost of the spell
---@field manaPercent uint32 The mana percentage required to cast the spell
---@field params boolean Whether the spell has parameters or not

---MountInfoTable is a table that contains all information about a mount.
---<br>
---It is returned when using Game.getMounts() 
---<br><br>
---Example Return:
--- ```lua
---local gameMounts = Game.getMounts()
---for i, mount in ipairs(gameMounts) do -- This will iterate through every mount registered
---	print("Mount Name: " .. mount.name) -- Example: "Coolest Mount Alive"
---	print("Mount Speed: " .. mount.speed) -- Example: 100
---	print("Mount Client ID: " .. mount.clientId) -- Example: 1234
---	print("Mount ID: " .. mount.id) -- Example: 1
---	print("Mount Premium: " .. mount.premium) -- Example: true
---end
---```
---<br>
---@class MountInfoTable
---@field name string The name of the mount
---@field speed int32 The speed of the mount
---@field clientId uint16 The client ID of the mount
---@field id uint16 The ID of the mount
---@field premium boolean Whether the mount is requires a premium account or not

--- GameClientVersion is a table that contains information about the client version of the game. 
--- <br>
--- It is returned when using Game.getClientVersion() 
--- <br><br>
--- Example Usage:
--- ```lua
--- local clientTable = Game.getClientVersion()
--- print("Client Min: " .. clientTable.min) -- Example: 1310
--- print("Client Max: " .. clientTable.max) -- Example: 1311
--- print("Client String: " .. clientTable.string) -- Example: 13.10
--- ```
--- <br>
---@class GameClientVersion
---@field min number -- Mininum version of the client allowed (Current 1310)
---@field max number -- Maximum version of the client allowed (Current 1311)
---@field string string -- String representation of the version (Current "13.10")

---Reflect is a table that will contain the percent and chance of reflecting a certain type of damagedamage 
---<br>
---It is used to set a reflect value to an item
---<br><br>
---Example Usage:
--- ```lua
--- -- Assuming we already have an item object
--- local reflect = {percent = 50, chance = 100} -- 100% chance to reflect 50% of the damage
--- item:setReflect(COMBAT_PHYSICALDAMAGE, reflect) -- Sets the item to reflect 50% of the physical damage with 100% chance
--- ```
---@class Reflect
---@field percent uint16 -- The percentage of reflect that will happen (0-100% of damage dealt will be reflected to attacker)
---@field chance uint16 -- The chance of the reflect happening (0-100% chance of reflect)


--- GuildRankInfoTable is a table that gets return after using guild:getRankById(rankId) or guild:getRankByName(rankName)
--- <br>
--- It contains the id, name, and level of the rank
--- <br><br>
--- Example Usage:
--- ```lua
--- local guild = Player(playerId):getGuild() -- Gets the guild of the player
--- local rank = guild:getRankById(1) -- Gets the rank by ID
--- tdump("RankInfo", rank) -- Prints the rank information
--- -- Example Output:
--- --- RankInfo: {id = 1, name = "Leader", level = 3}
---@class GuildRankInfoTable
---@field id uint32
---@field name string
---@field level uint32

--- A town is a table that contains information about a town.
--- <br>
--- It is returned when using `Game.getTowns()`, `player:getTown()` or `house:getTown()`
--- <br><br>
--- Example Usage:
--- ```lua
--- local towns = Game.getTowns() -- Gets all the towns in the game
--- 
--- for i, town in ipairs(towns) do -- This will iterate through every town registered
---	    print("Town Name: " .. town.name) -- Example: "Thais"
---	    print("Town ID: " .. town.id) -- Example: 1
---	    print("Town Position: " .. town.position) -- Example: Position(100, 100, 7)
--- end
--- ```
---@class Town
---@field id uint32 -- The ID of the town
---@field name string -- The name of the town
---@field position Position -- The position of the town

--- Item Abilities is a table which contains all the information about what an ItemType will give a player when equipped.
--- <br>
--- It is returned when using `itemType:getAbilities()`
--- <br><br>
--- Example Usage:
--- ```lua
--- local itemType = ItemType(1234) -- Gets the item type of the item with ID 1234
--- local itemAbilities = itemType:getAbilities() -- Gets the abilities of the item type
--- print("Item healthGain: " .. itemAbilities.healthGain) -- Example: 1
--- print("Gives Manashield: " .. itemAbilities.manashield) -- Example: true
--- print("Mana Gain: " .. itemAbilities.manaGain) -- Example: 1
--- print("Speed: " .. itemAbilities.speed) -- Example: 50
--- for stat, value in ipairs(itemAbilities.speicalSkills) do -- This will iterate through every specialSkill (crit chance, crit hit, etc ) in the item abilities
--- 	print("Special Skill: " .. stat .. " Value: " .. value) -- Example: Special Skill: 0 Value: 10
--- 	-- Note, this is going to print the index of the special skill and not the name of the special skill.
--- end
--- ```
--- <br>
---@class ItemAbilities
---@field healthGain uint32 -- The health gain per amount of ticks
---@field healthTicks uint32 -- The amount of ticks the health gain will happen
---@field manaGain uint32 -- The mana gain per amount of ticks
---@field manaTicks uint32 -- The amount of ticks the mana gain will happen
---@field conditionImmunities uint32 
---@field conditionSuppressions uint32
---@field speed int32
---@field elementDamage uint16
---@field elementType CombatTypes
---@field manaShield boolean
---@field invisible boolean
---@field regeneration boolean
---@field stats table<PlayerStats, number> -- This table will be indexed to the playerStat and the value of the stat
---@field statsPercent table<PlayerStats, number> -- This table will be indexed to the playerStat and how much % of the stat it will give
---@field skills table<SkillTypes, number> -- This table will be indexed to the playerSkill and the value of the skill
---@field specialSkills table<SpecialSkills, number> -- This table will be indexed to the specialSkill and the value of the specialSkill
---@field fieldAbsorbPercent table<CombatTypes, number> -- This table will be indexed to the combat type and the value of the absorb percent for field items
---@field absorbPercent table<CombatTypes, number> -- This table will be indexed to the combat type and the value of the absorb percent
---@field specialMagicLevel table<CombatTypes, number> -- This table will be indexed to the combat type and the value of the special magic level 
---@field boostPercent table<CombatTypes, number> -- This table will be indexed to the combat type and the value of the boost percent 
---@field reflectChance table<CombatTypes, number> -- This table will be indexed to the combat type and the how much reflect chance for that combat type
---@field reflectPercent table<CombatTypes, number> -- This table will be indexed to the combat type and the how much damage will be reflected for that combat type 


--- MonsterSpellInfo is a table that contains information about a monsterType's attack or defense.
--- <br>
--- It is returned when using `monsterType:getAttackList()` or `monsterType:getDefenseList()`
--- <br><br>
--- Example Usage:
--- ```lua
--- local monsterType = MonsterType("Dragon") -- Gets the monster type of the dragon
--- local monsterAttackInfo = monsterType:getAttackList() -- Gets the attack list of the monster type
--- for i, attack in ipairs(monsterAttackInfo) do -- This will iterate through every attack in the monster type
--- 	print("Chance of Attack: " .. attack.chance)
--- 	print("IsMelee?: " .. attack.isMelee) -- Example: true
--- end
--- ```
---@class MonsterSpellInfo
---@field chance uint32 -- The chance of the attack happening
---@field isMelee boolean -- Whether the attack is a melee attack or not
---@field isCombatSpell boolean -- Whether the attack is a combat spell or a support/utility spell for the monster
---@field minCombatValue int32 -- The minimum combat value of the attack
---@field maxCombatValue int32 -- The maximum combat value of the attack
---@field range uint32 -- The range of the attack
---@field speed uint32 -- The speed/interval of the attack

--- MonsterLootBlock is a table that contains information about a monsterType's loot.
--- <br>
--- It is returned when using `monsterType:getLoot()`
--- <br><br>
--- Example Usage:
--- ```lua
--- local monsterType = MonsterType("Dragon") -- Gets the monster type of the dragon
--- local monsterLootInfo = monsterType:getLoot() -- Gets the table of all the loot a Dragon could drop
--- for i, loot in ipairs(monsterLootInfo) do -- This will iterate through every loot block in the monster type
--- 	print("Loot ID: " .. loot.id) -- Example: 1234
--- end
--- ```
--- Full definition is found in `monsters.h:18`
---@class MonsterLootBlock
---@field id uint16 -- the Item ID of the item that would be looted
---@field countmax uint32 -- The max amount of items that would be looted
---@field chance uint32 -- The chance of the item being looted
---@field subType? int32 -- The subType of the item, optional if applicable
---@field actionId? int32 -- The actionId of the item, optional
---@field text? string -- The text of the item, optional
---@field childLoot? MonsterLootBlock -- If the item is a container, this will be the loot inside the container

--- MonsterBestiaryInfo contains all the information about a bestiary entry for a specific monster type.
--- <br>
--- It is returned when using `monsterType:getBestiaryInfo()`
--- <br><br>
--- Example Usage:
--- ```lua
--- local monsterType = MonsterType("Dragon") -- Gets the monster type of the dragon
--- local monsterBestiaryInfo = monsterType:getBestiaryInfo() -- Gets the bestiary info of the monster type
--- for key, bestiaryInfo in pairs(monsterBestiaryInfo) do -- This will iterate through every bestiary info in the monster type
---     print("Key: " .. key)
---     print("Value: " .. bestiaryInfo)
--- end
--- ```
---@class MonsterBestiaryInfo
---@field class string
---@field raceId uint32
---@field prowess uint32
---@field expertise uint32
---@field mastery uint32
---@field charmPoints uint32
---@field difficulty uint32
---@field occurence uint32
---@field locations string

---@class rawgetmetatable
---@field __index fun(self: table, key: any): any
rawgetmetatable = {}

---@class bit
---*const luaL_Reg LuaScriptInterface::luaBitReg[] = {
---@source ../src/luascript.cpp:4117
---@field band fun(a: number, b: number): number
---*const luaL_Reg LuaScriptInterface::luaBitReg[] = {
---@source ../src/luascript.cpp:4117
---@field bor fun(a: number, b: number): number
---*const luaL_Reg LuaScriptInterface::luaBitReg[] = {
---@source ../src/luascript.cpp:4117
---@field bxor fun(a: number, b: number): number
---*const luaL_Reg LuaScriptInterface::luaBitReg[] = {
---@source ../src/luascript.cpp:4117
---@field lshift fun(a: number, b: number): number
---*const luaL_Reg LuaScriptInterface::luaBitReg[] = {
---@source ../src/luascript.cpp:4117
---@field rshift fun(a: number, b: number): number
bit = {}

---@alias Outfit_t table<string, integer>
---@alias MagicEffect_t table<string, integer>

---@class DBInsert
---@field __gc fun(self: DBInsert)
---*int LuaScriptInterface::luaDBInsertAddRow(lua_State* L)
---@source ../src/luascript.cpp:4463
---@field addRow fun(self: DBInsert, row: string): boolean|nil
---*int LuaScriptInterface::luaDBInsertAddRow(lua_State* L)
---@source ../src/luascript.cpp:4475
---@field execute fun(self: DBInsert): boolean|nil
DBInsert = {}

---@class DBTransaction
---@field __eq fun(self: DBTransaction, other: DBTransaction): boolean
---@field __gc fun(self: DBTransaction)
---*int LuaScriptInterface::luaDBTransactionBegin(lua_State* L)
---@source ../src/luascript.cpp:4508
---@field begin fun(self: DBTransaction): boolean|nil
---*int LuaScriptInterface::luaDBTransactionCommit(lua_State* L)
---@source ../src/luascript.cpp:4518
---@field commit fun(self: DBTransaction): boolean|nil
---*int LuaScriptInterface::luaDBTransactionDelete(lua_State* L)
---@source ../src/luascript.cpp:4530
---@field rollback fun(self: DBTransaction)
DBTransaction = {}

---@class db
---*int LuaScriptInterface::luaDatabaseExecute(lua_State* L)
---@source ../src/luascript.cpp:4199
---@field query fun(query: string): any
---*int LuaScriptInterface::luaDatabaseAsyncExecute(lua_State* L)
---@source ../src/luascript.cpp:4206
---@field asyncQuery fun(query: string): boolean
---*int LuaScriptInterface::luaDatabaseStoreQuery(lua_State* L)
---@source ../src/luascript.cpp:4237
---@field storeQuery fun(query: string): any
---*int LuaScriptInterface::luaDatabaseAsyncStoreQuery(lua_State* L)
---@source ../src/luascript.cpp:4248
---@field asyncStoreQuery fun(query: string): boolean
---*int LuaScriptInterface::luaDatabaseEscapeString(lua_State* L)
---@source ../src/luascript.cpp:4283
---@field escapeString fun(value: string): string -- Escapes a string for query
---*int LuaScriptInterface::luaDatabaseEscapeBlob(lua_State* L)
---@source ../src/luascript.cpp:4290
---@field escapeBlob fun(value: string, length: uint32): string -- Escapes a binary stream for query
---*int LuaScriptInterface::luaDatabaseLastInsertId(lua_State* L)
---@source ../src/luascript.cpp:4298
---@field lastInsertId fun(): uint64 -- Returns the last inserted row's id
db = {}

---@class result
---*int LuaScriptInterface::luaResultFree(lua_State* L)
---@source ../src/luascript.cpp:4369
---@field free fun(resultId: uint32)
---*int LuaScriptInterface::luaResultNext(lua_State* L)
---@source ../src/luascript.cpp:4357
---@field next fun(resultId: uint32): number
---*int LuaScriptInterface::luaResultGetNumber(lua_State* L)
---@source ../src/luascript.cpp:4317
---@field getNumber fun(resultId: uint32, column: string): number
---*int LuaScriptInterface::luaResultGetString(lua_State* L)
---@source ../src/luascript.cpp:4330
---@field getString fun(resultId: uint32, column: string): string
---@field getBoolean fun(resultId: uint32, column: string): boolean -- No source in luascript?
---*int LuaScriptInterface::luaResultGetStream(lua_State* L)
---@source ../src/luascript.cpp:4343
---@field getStream fun(resultId: uint32, column: string): string
result = {}

---@class configManager
---*int LuaScriptInterface::luaConfigManagerGetBoolean(lua_State* L)
---@source ../src/luascript.cpp:4182
---@field getBoolean fun(key: string): boolean
---*int LuaScriptInterface::luaConfigManagerGetNumber(lua_State* L)
---@source ../src/luascript.cpp:4176
---@field getNumber fun(key: string): number
---*int LuaScriptInterface::luaConfigManagerGetString(lua_State* L)
---@source ../src/luascript.cpp:4170
---@field getString fun(key: string): string
configManager = {}
-- These functions do not exist in luascript.cpp
-- -@field setBoolean fun(key: string, value: boolean): boolean
-- -@field setNumber fun(key: string, value: number): boolean
-- -@field setString fun(key: string, value: string): boolean
-- -@field remove fun(key: string): boolean
-- -@field getKeys fun(): table


--- The Game class is the main class for the game engine.
--- <br>
--- It is used to get large scale information about the current game state, such as all monsters spawned, all players online, current world type, etc.
--- <br>
--- It is also used to create new items, monsters, npcs, and tiles.
--- <br><br>
--- Example Usage:
--- ```lua
--- local spectators = Game.getSpectators(Position(100, 100, 7), true, true) -- Gets all the players in an area around position 100, 100, 7
--- for i, spectator in ipairs(spectators) do -- This will iterate through every player in the area
---   print("Spectator Name: " .. spectator:getName()) -- Example: "Player1"
--- end
--- local monster = Game.createMonster("Dragon", Position(100, 100, 7)) -- Creates a dragon at position 100, 100, 7
--- local newItem = Game.createItem(1234, 1, Position(100, 100, 7)) -- Creates an item with ID 1234 at position 100, 100, 7
--- ```
--- Further definiton of the Game class can be found in `game.h`
---@class Game
---*int LuaScriptInterface::luaGameGetSpectators(lua_State* L)
---@source ../src/luascript.cpp:4541
---@field getSpectators fun(position: Position, multifloor?: boolean, onlyPlayers?: boolean, minRangeX?: int32, minRangeY?: int32, maxRangeX?: int32, maxRangeY?: int32): table<number, Creature> -- Returns a table of all creatures (spectators) in a defined range. Recommended whenever possible not define a min/max range, as it will default to maxViewPortX/maxViewPortY which is cached
---*int LuaScriptInterface::luaGameGetPlayers(lua_State* L)
---@source ../src/luascript.cpp:4567
---@field getPlayers fun(): table<number, Player> 
---*int LuaScriptInterface::luaGameGetNpcs(lua_State* L)
---@source ../src/luascript.cpp:4581
---@field getNpcs fun(): table<number, Npc>
---*int LuaScriptInterface::luaGameGetMonsters(lua_State* L)
---@source ../src/luascript.cpp:4595
---@field getMonsters fun(): table<number, Monster>
---*int LuaScriptInterface::luaGameLoadMap(lua_State* L)
---@source ../src/luascript.cpp:4609
---@field loadMap fun(path: string)
---*int LuaScriptInterface::luaGameGetExperienceStage(lua_State* L)
---@source ../src/luascript.cpp:4624
---@field getExperienceStage fun(level: uint32): number
---*int LuaScriptInterface::luaGameGetExperienceForLevel(lua_State* L)
---@source ../src/luascript.cpp:4632
---@field getExperienceForLevel fun(level: uint32): uint64
---*int LuaScriptInterface::luaGameGetMonsterCount(lua_State* L)
---@source ../src/luascript.cpp:4644
---@field getMonsterCount fun(): number
---*int LuaScriptInterface::luaGameGetPlayerCount(lua_State* L)
---@source ../src/luascript.cpp:4651
---@field getPlayerCount fun(): number
---*int LuaScriptInterface::luaGameGetNpcCount(lua_State* L)
---@source ../src/luascript.cpp:4658
---@field getNpcCount fun(): number
---*int LuaScriptInterface::luaGameGetMonsterTypes(lua_State* L)
---@source ../src/luascript.cpp:4665
---@field getMonsterTypes fun(): table<number, MonsterType> -- Table of all monster types
---*int LuaScriptInterface::luaGameGetBestiary(lua_State* L)
---@source ../src/luascript.cpp:4679
---@field getBestiary fun(): table<string, table<number, MonsterType>> -- Table of all monsters in the bestiary. Sorted by {className = {index = monsterType, ...}, ...}
---*int LuaScriptInterface::luaGameGetCurrencyItems(lua_State* L)
---@source ../src/luascript.cpp:4703
---@field getCurrencyItems fun(): table<number, ItemType>
---*int LuaScriptInterface::luaGameGetItemTypeByClientId(lua_State* L)
---@source ../src/luascript.cpp:4719
---@field getItemTypeByClientId fun(clientId: uint16): ItemType
---*int LuaScriptInterface::luaGameGetMountIdByLookType(lua_State* L)
---@source ../src/luascript.cpp:4734
---@field getMountIdByLookType fun(lookType: uint16): number
---*int LuaScriptInterface::luaGameGetTowns(lua_State* L)
---@source ../src/luascript.cpp:4750
---@field getTowns fun(): table<number, Town>
---*int LuaScriptInterface::luaGameGetHouses(lua_State* L)
---@source ../src/luascript.cpp:4764
---@field getHouses fun(): table<number, House>
---*int LuaScriptInterface::luaGameGetOutfits(lua_State* L)
---@source ../src/luascript.cpp:4779
---@field getOutfits fun(sex: PlayerSexes): table<number, Outfit>
---*int LuaScriptInterface::luaGameGetMounts(lua_State* L)
---@source ../src/luascript.cpp:4805
---@field getMounts fun(): table<number, MountInfoTable>
---*int LuaScriptInterface::luaGameGetVocations(lua_State* L)
---@source ../src/luascript.cpp:4827
---@field getVocations fun(): table<number, Vocation>
---*int LuaScriptInterface::luaGameGetGameState(lua_State* L)
---@source ../src/luascript.cpp:4843
---@field getGameState fun(): GameStates
---*int LuaScriptInterface::luaGameSetGameState(lua_State* L)
---@source ../src/luascript.cpp:4850
---@field setGameState fun(state: GameStates): boolean -- Will always return true
---*int LuaScriptInterface::luaGameGetWorldType(lua_State* L)
---@source ../src/luascript.cpp:4859
---@field getWorldType fun(): WorldTypes
---*int LuaScriptInterface::luaGameSetWorldType(lua_State* L)
---@source ../src/luascript.cpp:4866
---@field setWorldType fun(type: WorldTypes): boolean -- Will always return true
---*int LuaScriptInterface::luaGameGetReturnMessage(lua_State* L)
---@source ../src/luascript.cpp:4875
---@field getReturnMessage fun(value: ReturnValues): string
---*int LuaScriptInterface::luaGameGetItemAttributeByName(lua_State* L)
---@source ../src/luascript.cpp:4883
---@field getItemAttributeByName fun(attribute: string): ItemAttributes
---*int LuaScriptInterface::luaGameCreateItem(lua_State* L)
---@source ../src/luascript.cpp:4890
---@field createItem fun(itemInfo: uint16|string, count?: uint16, position?: Position): Item
---*int LuaScriptInterface::luaGameCreateContainer(lua_State* L)
---@source ../src/luascript.cpp:4936
---@field createContainer fun(containerInfo: uint16|string, size: number, position?: Position): Container
---*int LuaScriptInterface::luaGameCreateMonster(lua_State* L)
---@source ../src/luascript.cpp:4977
---@field createMonster fun(name: string, position: Position, extended?: boolean, force?: boolean, magicEffect?: MagicEffectConsts): Monster
---*int LuaScriptInterface::luaGameCreateNpc(lua_State* L)
---@source ../src/luascript.cpp:5005
---@field createNpc fun(name: string, position: Position, extended?: boolean, force?: boolean, magicEffect?: MagicEffectConsts): Npc
---*int LuaScriptInterface::luaGameCreateTile(lua_State* L)
---@source ../src/luascript.cpp:5028
---@field createTile fun(position: Position, isDynamic?: boolean): Tile -- Position can either be given as three params x, y, z or as a Position, Position(x,y,z) 
---*int LuaScriptInterface::luaGameCreateMonsterType(lua_State* L)
---@source ../src/luascript.cpp:5060
---@field createMonsterType fun(name: string): MonsterType
---*int LuaScriptInterface::luaGameStartEvent(lua_State* L)
---@source ../src/luascript.cpp:5097
---@field startEvent fun(eventName: string): boolean
---*int LuaScriptInterface::luaGameGetClientVersion(lua_State* L)
---@source ../src/luascript.cpp:5111
---@field getClientVersion fun(): GameClientVersion
---*int LuaScriptInterface::luaGameReload(lua_State* L)
---@source ../src/luascript.cpp:5121
---@field reload fun(reloadType: number): boolean -- NOTE: this can lead to unexpected behaviour depending how you set up scripts. Please be careful when using. If errors occur in your scripts after reloading, restart the server and see if the same error is happening
Game = {}

---A Variant can either be a Thing userdata, a number, a position, or a string <br>
---In general, it represents something during combat (Spells, weapon hits, etc) <br>
---Further definiton of the Variant class can be found in luavariant.h <br>
---@source ../src/luavariant.h:14
---@class Variant
---*int LuaScriptInterface::luaVariantCreate(lua_State* L)
---@source ../src/luascript.cpp:5138
---@field create fun(variantInfo: uint32|Thing|string|Position): Variant
---*int LuaScriptInterface::luaVariantGetNumber(lua_State* L)
---@source ../src/luascript.cpp:5157
---@field getNumber fun(self: Variant): uint32
---*int LuaScriptInterface::luaVariantGetString(lua_State* L)
---@source ../src/luascript.cpp:5169
---@field getString fun(self: Variant): string
---*int LuaScriptInterface::luaVariantGetPosition(lua_State* L)
---@source ../src/luascript.cpp:5181
---@field getPosition fun(self: Variant): Position
---@source ../src/luavariant.h:14
Variant = {}

---A position class represents an x,y,z (and sometimes stackpos) of somewhere in the game world <br>
---Once a position is created, it can be used to send a magic effect to the position, check if the sight is clear between two positions,
--- or send a distance effect between the self position and a target position <br> <br>
---Positions can be created similar to this example;
---```lua
---local position = Position(x, y, z) -- Creates a position with x,y,z
--- -- another example
---local player = Player(playerId) -- Gets the player object
---local position2 = player:getPosition() -- Gets the position of the player
---```
---Further definiton of the position class can be found in `position.h`
---@source ../src/position.h:30
---@class Position
---*int LuaScriptInterface::luaPositionCreate(lua_State* L)
---@source ../src/luascript.cpp:5196
---@field create fun(positionX: uint16, positionY: uint16, positionZ: uint8, stackPos?: uint32): Position -- Can be passed x, y, z, stackPos, or another Position
---*int LuaScriptInterface::luaPositionIsSightClear(lua_State* L)
---@source ../src/luascript.cpp:5220
---@field isSightClear fun(self: Position, positionEx: Position, sameFloor?: boolean): boolean
---*int LuaScriptInterface::luaPositionSendMagicEffect(lua_State* L)
---@source ../src/luascript.cpp:5230
---@field sendMagicEffect fun(self: Position, effectType: MagicEffectConsts, creature?: Creature): boolean
---*int LuaScriptInterface::luaPositionSendDistanceEffect(lua_State* L)
---@source ../src/luascript.cpp:5258
---@field sendDistanceEffect fun(self: Position, targetPos: Position, effectType: MagicEffectConsts, player?: Player): boolean
---@source ../src/position.h:30
Position = {}

---A tile class is a userdata representing the ground item at a specific position <br>
---The tile at a specific position can be retrieved by calling Tile(Position) <br>
---A tile can also be gotten from using getTile() on a creature, or an item <br>
---Finally, all house tiles can be retrived by using house:getTiles() <br>
---
---Example usage:
---```lua
---local player = Player(playerId) -- Gets the player object
---local tilePlayerIsOn = player:getTile() -- Gets the tile the player is on
---
---local tile2 = Tile(Position(100, 100, 7)) -- Gets the tile at position 100, 100, 7
---local tile3 = Tile(200, 200, 7) -- Gets the tile at position 200, 200, 7
---
--- local samePlayer = tilePlayerIsOn:getTopCreature() -- Will return the same player object "player"
---``` 
--- Further definiton of the tile class can be found in `tile.h`
---@source ../src/tile.h
---@class Tile
---*int LuaScriptInterface::luaTileCreate(lua_State* L)
---@source ../src/luascript.cpp:5283
---@field create fun(): Tile
---@field __eq fun(self: Tile, other: Tile): boolean
---*int LuaScriptInterface::luaTileRemove(lua_State* L)
---@source ../src/luascript.cpp:5306
---@field remove fun(self: Tile): boolean
---*int LuaScriptInterface::luaTileGetPosition(lua_State* L)
---@source ../src/luascript.cpp:5324
---@field getPosition fun(self: Tile): Position
---*int LuaScriptInterface::luaTileGetGround(lua_State* L)
---@source ../src/luascript.cpp:5349
---@field getGround fun(self: Tile): Item
---*int LuaScriptInterface::luaTileGetThing(lua_State* L)
---@source ../src/luascript.cpp:5349
---@field getThing fun(self: Tile, index: int32): Creature|Item|nil
---*int LuaScriptInterface::luaTileGetThingCount(lua_State* L)
---@source ../src/luascript.cpp:5377
---@field getThingCount fun(self: Tile): number
---*int LuaScriptInterface::luaTileGetTopVisibleThing(lua_State* L)
---@source ../src/luascript.cpp:5389
---@field getTopVisibleThing fun(self: Tile): Creature|Item|nil
---*int LuaScriptInterface::luaTileGetTopTopItem(lua_State* L)
---@source ../src/luascript.cpp:5417
---@field getTopTopItem fun(self: Tile): Item
---*int LuaScriptInterface::luaTileGetTopDownItem(lua_State* L)
---@source ../src/luascript.cpp:5436
---@field getTopDownItem fun(self: Tile): Item
---*int LuaScriptInterface::luaTileGetFieldItem(lua_State* L)
---@source ../src/luascript.cpp:5455
---@field getFieldItem fun(self: Tile): Item
---*int LuaScriptInterface::luaTileGetItemById(lua_State* L)
---@source ../src/luascript.cpp:5474
---@field getItemById fun(self: Tile, itemInfo: uint16|string): Item
---*int LuaScriptInterface::luaTileGetItemByType(lua_State* L)
---@source ../src/luascript.cpp:5505
---@field getItemByType fun(self: Tile, itemType: ItemTypes): Item -- This is checking if an item on the tile has a specific type 
---*int LuaScriptInterface::luaTileGetItemByTopOrder(lua_State* L)
---@source ../src/luascript.cpp:5570
---@field getItemByTopOrder fun(self: Tile, order: int32): Item
---*int LuaScriptInterface::luaTileGetItemCountById(lua_State* L)
---@source ../src/luascript.cpp:5592
---@field getItemCountById fun(self: Tile, itemInfo: uint16|string, subtype?: int32): uint32
---*int LuaScriptInterface::luaTileGetBottomCreature(lua_State* L)
---@source ../src/luascript.cpp:5618
---@field getBottomCreature fun(self: Tile): Creature
---*int LuaScriptInterface::luaTileGetTopCreature(lua_State* L)
---@source ../src/luascript.cpp:5638
---@field getTopCreature fun(self: Tile): Creature
---*int LuaScriptInterface::luaTileGetBottomVisibleCreature(lua_State* L)
---@source ../src/luascript.cpp:5658
---@field getBottomVisibleCreature fun(self: Tile): Creature
---*int LuaScriptInterface::luaTileGetTopVisibleCreature(lua_State* L)
---@source ../src/luascript.cpp:5683
---@field getTopVisibleCreature fun(self: Tile): Creature
---*int LuaScriptInterface::luaTileGetItems(lua_State* L)
---@source ../src/luascript.cpp:5708
---@field getItems fun(self: Tile): table<number, Item>
---*int LuaScriptInterface::luaTileGetItemCount(lua_State* L)
---@source ../src/luascript.cpp:5734
---@field getItemCount fun(self: Tile): number
---*int LuaScriptInterface::luaTileGetDownItemCount(lua_State* L)
---@source ../src/luascript.cpp:5747
---@field getDownItemCount fun(self: Tile): number
---*int LuaScriptInterface::luaTileGetTopItemCount(lua_State* L)
---@source ../src/luascript.cpp:5759
---@field getTopItemCount fun(self: Tile): number
---*int LuaScriptInterface::luaTileGetCreatures(lua_State* L)
---@source ../src/luascript.cpp:5772
---@field getCreatures fun(self: Tile): table<number, Creature>
---*int LuaScriptInterface::luaTileGetCreatureCount(lua_State* L)
---@source ../src/luascript.cpp:5798
---@field getCreatureCount fun(self: Tile): number
---*int LuaScriptInterface::luaTileHasProperty(lua_State* L)
---@source ../src/luascript.cpp:5811
---@field hasProperty fun(self: Tile, property: ItemProperties): boolean
---*int LuaScriptInterface::luaTileGetThingIndex(lua_State* L)
---@source ../src/luascript.cpp:5836
---@field getThingIndex fun(self: Tile, thing: Creature|Item): number
---*int LuaScriptInterface::luaTileHasFlag(lua_State* L)
---@source ../src/luascript.cpp:5854
---@field hasFlag fun(self: Tile, flag: TileFlags): boolean
---*int LuaScriptInterface::luaTileQueryAdd(lua_State* L)
---@source ../src/luascript.cpp:5867
---@field queryAdd fun(self: Tile, thing: Creature|Item, index: uint32): boolean
---*int LuaScriptInterface::luaTileAddItem(lua_State* L)
---@source ../src/luascript.cpp:5886
---@field addItem fun(self: Tile, item: Item, countOrSubtype?: uint32, flags?: uint32): boolean
---*int LuaScriptInterface::luaTileAddItemEx(lua_State* L)
---@source ../src/luascript.cpp:5927
---@field addItemEx fun(self: Tile, item: Item, flags?: uint32): boolean
---*int LuaScriptInterface::luaTileGetHouse(lua_State* L)
---@source ../src/luascript.cpp:5957
---@field getHouse fun(self: Tile): House
---@source ../src/tile.h
Tile = {}

---A NetworkMessage is a userdata representing a message that has either been recieved by the client, or is going to be sent to the client 
---<br>
---It can be used to send a specifically built message to the client, or to read a built message that has been sent to the server 
---<br><br>
---**Critical Note:** A NetworkMessage MUST be read in the same order as sent. If not, it will cause issues up to a crash. 
---<br><br>
---If using OTClient and some bytes or other information is not read in the right order, it will discard the rest of the packet being read. 
---<br>
---**Always verify the order of the NetworkMessage being sent and read** 
---<br><br>
---Example usage:
--- ```lua
--- local player = Player(playerId) -- Gets the player object
--- 
--- local networkMessage = NetworkMessage() -- Constructs a new NetworkMessage
---	networkMessage:addByte(0x00)
---	networkMessage:addString("some string")
---	networkMessage:addPosition(player:getPosition())
---	networkMessage:sendToPlayer(player)
---	networkMessage:delete()
--- ```
--- Further definiton of the NetworkMessage class can be found in `networkmessage.h`
---@source ../src/networkmessage.h:15
---@class NetworkMessage
---*int LuaScriptInterface::luaNetworkMessageCreate(lua_State* L)
---@source ../src/luascript.cpp:5976
---@field create fun(): NetworkMessage
---@field __eq fun(self: NetworkMessage, other: NetworkMessage): boolean
---@field __gc fun(self: NetworkMessage)
---*int LuaScriptInterface::luaNetworkMessageDelete(lua_State* L)
---@source ../src/luascript.cpp:5984
---@field delete fun(self: NetworkMessage)
---*int LuaScriptInterface::luaNetworkMessageGetByte(lua_State* L)
---@source ../src/luascript.cpp:5994
---@field getByte fun(self: NetworkMessage): uint8
---*int LuaScriptInterface::luaNetworkMessageGetU16(lua_State* L)
---@source ../src/luascript.cpp:6006
---@field getU16 fun(self: NetworkMessage): uint16
---*int LuaScriptInterface::luaNetworkMessageGetU32(lua_State* L)
---@source ../src/luascript.cpp:6018
---@field getU32 fun(self: NetworkMessage): uint32
---*int LuaScriptInterface::luaNetworkMessageGetU64(lua_State* L)
---@source ../src/luascript.cpp:6030
---@field getU64 fun(self: NetworkMessage): uint64
---*int LuaScriptInterface::luaNetworkMessageGetString(lua_State* L)
---@source ../src/luascript.cpp:6042
---@field getString fun(self: NetworkMessage): string
---*int LuaScriptInterface::luaNetworkMessageGetPosition(lua_State* L)
---@source ../src/luascript.cpp:6054
---@field getPosition fun(self: NetworkMessage): Position
---*int LuaScriptInterface::luaNetworkMessageAddByte(lua_State* L)
---@source ../src/luascript.cpp:6066
---@field addByte fun(self: NetworkMessage, byte: uint8)
---*int LuaScriptInterface::luaNetworkMessageAddU16(lua_State* L)
---@source ../src/luascript.cpp:6078
---@field addU16 fun(self: NetworkMessage, u16: uint16)
---*int LuaScriptInterface::luaNetworkMessageAddU32(lua_State* L)
---@source ../src/luascript.cpp:6094
---@field addU32 fun(self: NetworkMessage, u32: uint32)
---*int LuaScriptInterface::luaNetworkMessageAddU64(lua_State* L)
---@source ../src/luascript.cpp:6108
---@field addU64 fun(self: NetworkMessage, u64: uint64)
---*int LuaScriptInterface::luaNetworkMessageAddString(lua_State* L)
---@source ../src/luascript.cpp:6122
---@field addString fun(self: NetworkMessage, string: string)
---*int LuaScriptInterface::luaNetworkMessageAddPosition(lua_State* L)
---@source ../src/luascript.cpp:6136
---@field addPosition fun(self: NetworkMessage, position: Position)
---*int LuaScriptInterface::luaNetworkMessageAddDouble(lua_State* L)
---@source ../src/luascript.cpp:6150
---@field addDouble fun(self: NetworkMessage, double: double)
---*int LuaScriptInterface::luaNetworkMessageAddItem(lua_State* L)
---@source ../src/luascript.cpp:6164
---@field addItem fun(self: NetworkMessage, item: Item)
---*int LuaScriptInterface::luaNetworkMessageAddItemId(lua_State* L)
---@source ../src/luascript.cpp:6184
---@field addItemId fun(self: NetworkMessage, itemId: uint16|string)
---*int LuaScriptInterface::luaNetworkMessageReset(lua_State* L)
---@source ../src/luascript.cpp:6209
---@field reset fun(self: NetworkMessage)
---*int LuaScriptInterface::luaNetworkMessageSeek(lua_State* L)
---@source ../src/luascript.cpp:6222
---@field seek fun(self: NetworkMessage, position: uint16): boolean
---*int LuaScriptInterface::luaNetworkMessageTell(lua_State* L)
---@source ../src/luascript.cpp:6234
---@field tell fun(self: NetworkMessage): number
---*int LuaScriptInterface::luaNetworkMessageLength(lua_State* L)
---@source ../src/luascript.cpp:6246
---@field len fun(self: NetworkMessage): number
---*int LuaScriptInterface::luaNetworkMessageSkipBytes(lua_State* L)
---@source ../src/luascript.cpp:6258
---@field skipBytes fun(self: NetworkMessage, count: number)
---*int LuaScriptInterface::luaNetworkMessageSendToPlayer(lua_State* L)
---@source ../src/luascript.cpp:6272
---@field sendToPlayer fun(self: NetworkMessage, player: Player)
---@source ../src/networkmessage.h:15
NetworkMessage = {}

-- NOTE: ModalWindow definitions moved to modal_window_helper.lua

---The Item class, extending a Thing, is a representation of an item in the game world 
---<br><br>
---Once an item has either been created by using various implmentations of createItem, or addItem, the return value can be used to manipulate the item further
---<br><br>
---Example usage:
---```lua
---local player = Player(playerId) -- Gets a player that you would like to add an item to
---local item = player:addItem(itemId, count) -- Adds an item to the player and returns the item object
---
---print(item:getId()) -- Returns the item id of the item
---print(item:getCount()) -- Returns the count of the item
---
---item:setActionId(actionId) -- Sets the action id of the item
---print(item:getActionId()) -- Returns the action id just set
---
---item:setCustomAttribute(key, value) -- Sets a custom attribute on the item
---print(item:getCustomAttribute(key)) -- Returns the value of the custom attribute just set
---```
---Further definiton of the Item class can be found in `item.h`
---@source ../src/item.h
---@class Item : Thing
---*int LuaScriptInterface::luaItemCreate(lua_State* L)
---@source ../src/luascript.cpp:6530
---@field create fun(uid: uint32): Item
---@field __eq fun(self: Item, other: Item): boolean
---*int LuaScriptInterface::luaItemIsItem(lua_State* L)
---@source ../src/luascript.cpp:6545
---@field isItem fun(self: Item): boolean
---*int LuaScriptInterface::luaItemHasParent(lua_State* L)
---@source ../src/luascript.cpp:6552
---@field hasParent fun(self: Item): boolean?
---*int LuaScriptInterface::luaItemGetParent(lua_State* L)
---@source ../src/luascript.cpp:6565
---@field getParent fun(self: Item): Item?
---*int LuaScriptInterface::luaItemGetTopParent(lua_State* L)
---@source ../src/luascript.cpp:6584
---@field getTopParent fun(self: Item): Item?
---*int LuaScriptInterface::luaItemGetId(lua_State* L)
---@source ../src/luascript.cpp:6603
---@field getId fun(self: Item): uint16
---*int LuaScriptInterface::luaItemClone(lua_State* L)
---@source ../src/luascript.cpp:6615
---@field clone fun(self: Item): Item
---*int LuaScriptInterface::luaItemSplit(lua_State* L)
---@source ../src/luascript.cpp:6638
---@field split fun(self: Item, count?: uint16): Item
---*int LuaScriptInterface::luaItemRemove(lua_State* L)
---@source ../src/luascript.cpp:6686
---@field remove fun(self: Item, count?: int32): boolean
---*int LuaScriptInterface::luaItemGetUniqueId(lua_State* L)
---@source ../src/luascript.cpp:6699
---@field getUniqueId fun(self: Item): uint16
---*int LuaScriptInterface::luaItemGetActionId(lua_State* L)
---@source ../src/luascript.cpp:6715
---@field getActionId fun(self: Item): uint16
---*int LuaScriptInterface::luaItemSetActionId(lua_State* L)
---@source ../src/luascript.cpp:6727
---@field setActionId fun(self: Item, actionId: uint16)
---*int LuaScriptInterface::luaItemGetCount(lua_State* L)
---@source ../src/luascript.cpp:6741
---@field getCount fun(self: Item): uint16
---*int LuaScriptInterface::luaItemGetCharges(lua_State* L)
---@source ../src/luascript.cpp:6753
---@field getCharges fun(self: Item): uint16
---*int LuaScriptInterface::luaItemGetFluidType(lua_State* L)
---@source ../src/luascript.cpp:6765
---@field getFluidType fun(self: Item): uint16
---*int LuaScriptInterface::luaItemGetWeight(lua_State* L)
---@source ../src/luascript.cpp:6777
---@field getWeight fun(self: Item): uint32
---*int LuaScriptInterface::luaItemGetWorth(lua_State* L)
---@source ../src/luascript.cpp:6789
---@field getWorth fun(self: Item): uint32
---*int LuaScriptInterface::luaItemGetSubType(lua_State* L)
---@source ../src/luascript.cpp:6801
---@field getSubType fun(self: Item): uint16
---*int LuaScriptInterface::luaItemGetName(lua_State* L)
---@source ../src/luascript.cpp:6813
---@field getName fun(self: Item): string
---*int LuaScriptInterface::luaItemGetPluralName(lua_State* L)
---@source ../src/luascript.cpp:6825
---@field getPluralName fun(self: Item): string
---*int LuaScriptInterface::luaItemGetArticle(lua_State* L)
---@source ../src/luascript.cpp:6837
---@field getArticle fun(self: Item): string
---*int LuaScriptInterface::luaItemGetPosition(lua_State* L)
---@source ../src/luascript.cpp:6849
---@field getPosition fun(self: Item): Position
---*int LuaScriptInterface::luaItemGetTile(lua_State* L)
---@source ../src/luascript.cpp:6861
---@field getTile fun(self: Item): Tile
---*int LuaScriptInterface::luaItemHasAttribute(lua_State* L)
---@source ../src/luascript.cpp:6880
---@field hasAttribute fun(self: Item, attribute: ItemAttributes|string): boolean
---*int LuaScriptInterface::luaItemGetAttribute(lua_State* L)
---@source ../src/luascript.cpp:6902
---@field getAttribute fun(self: Item, attribute: ItemAttributes|string): number|string|nil
---*int LuaScriptInterface::luaItemSetAttribute(lua_State* L)
---@source ../src/luascript.cpp:6930
---@field setAttribute fun(self: Item, attribute: ItemAttributes|string, value: number|string)
---*int LuaScriptInterface::luaItemRemoveAttribute(lua_State* L)
---@source ../src/luascript.cpp:6966
---@field removeAttribute fun(self: Item, attribute: ItemAttributes|string)
---*int LuaScriptInterface::luaItemGetCustomAttribute(lua_State* L)
---@source ../src/luascript.cpp:6994
---@field getCustomAttribute fun(self: Item, key: string): any
---*int LuaScriptInterface::luaItemSetCustomAttribute(lua_State* L)
---@source ../src/luascript.cpp:7021
---@field setCustomAttribute fun(self: Item, key: string, value: any)
---*int LuaScriptInterface::luaItemRemoveCustomAttribute(lua_State* L)
---@source ../src/luascript.cpp:7062
---@field removeCustomAttribute fun(self: Item, key: string)
---*int LuaScriptInterface::luaItemMoveTo(lua_State* L)
---@source ../src/luascript.cpp:7081
---@field moveTo fun(self: Item, destination: Position|Thing, flags?: CylinderFlag)
---*int LuaScriptInterface::luaItemTransform(lua_State* L)
---@source ../src/luascript.cpp:7145
---@field transform fun(self: Item, newItem: number|string, count?: number)
---*int LuaScriptInterface::luaItemDecay(lua_State* L)
---@source ../src/luascript.cpp:7199
---@field decay fun(self: Item, decayToId?: int32)
---*int LuaScriptInterface::luaItemGetSpecialDescription(lua_State* L)
---@source ../src/luascript.cpp:7216
---@field getSpecialDescription fun(self: Item): string
---*int LuaScriptInterface::luaItemHasProperty(lua_State* L)
---@source ../src/luascript.cpp:7228
---@field hasProperty fun(self: Item, property: ItemProperties): boolean
---*int LuaScriptInterface::luaItemIsLoadedFromMap(lua_State* L)
---@source ../src/luascript.cpp:7241
---@field isLoadedFromMap fun(self: Item): boolean
---*int LuaScriptInterface::luaItemSetStoreItem(lua_State* L)
---@source ../src/luascript.cpp:7253
---@field setStoreItem fun(self: Item, store: boolean)
---*int LuaScriptInterface::luaItemIsStoreItem(lua_State* L)
---@source ../src/luascript.cpp:7266
---@field isStoreItem fun(self: Item): boolean
---*int LuaScriptInterface::luaItemSetReflect(lua_State* L)
---@source ../src/luascript.cpp:7278
---@field setReflect fun(self: Item, combatType: CombatTypes, reflect: Reflect)
---*int LuaScriptInterface::luaItemGetReflect(lua_State* L)
---@source ../src/luascript.cpp:7298
---@field getReflect fun(self: Item, combatType: CombatTypes, total?: boolean): Reflect -- Returns a reflect table for the specific combat type. If total = true, it'll return the values for both the ItemType as well as any reflect assigned to the item specifically
---*int LuaScriptInterface::luaItemSetBoostPercent(lua_State* L)
---@source ../src/luascript.cpp:7315
---@field setBoostPercent fun(self: Item, combatType: CombatTypes, percent: uint16)
---*int LuaScriptInterface::luaItemGetBoostPercent(lua_State* L)
---@source ../src/luascript.cpp:7329
---@field getBoostPercent fun(self: Item, combatType: CombatTypes, total?: boolean): number
---@field addItem fun(self: Item, id: number, subtype?: number): boolean
---@source ../src/item.h
Item = {}

--- A container, extending an item, is a representation of a container in the game world 
--- <br>
--- Once an item is gotten from the various methods possible, if it is a container then it has additional functions defined in this class
--- <br><br>
--- Example Usage:
--- ```lua
--- local player = Player(playerId) -- Gets the player object
--- local backpack = player:getSlotItem(CONST_SLOT_BACKPACK) -- Gets the backpack of the player
--- for i, item in ipairs(backpack:getItems()) do -- Loops through all items in the backpack
--- 	print(item:getId()) -- Prints the id of the item
--- end
--- ```
---@source ../src/container.h
---@class Container : Item
---*int LuaScriptInterface::luaContainerGetSize(lua_State* L)
---@source ../src/luascript.cpp:7358
---@field getSize fun(self: Container): number
---*int LuaScriptInterface::luaContainerGetCapacity(lua_State* L)
---@source ../src/luascript.cpp:7370
---@field getCapacity fun(self: Container): number
---*int LuaScriptInterface::luaContainerGetEmptySlots(lua_State* L)
---@source ../src/luascript.cpp:7382
---@field getEmptySlots fun(self: Container): number
---*int LuaScriptInterface::luaContainerGetItems(lua_State* L)
---@source ../src/luascript.cpp:7601
---@field getItems fun(self: Container): table
---*int LuaScriptInterface::luaContainerGetItemCountById(lua_State* L)
---@source ../src/luascript.cpp:7576
---@field getItemCountById fun(self: Container, itemId: number, subType: number): number
---*int LuaScriptInterface::luaContainerGetItem(lua_State* L)
---@source ../src/luascript.cpp:7416
---@field getItem fun(self: Container, index: number): Item
---*int LuaScriptInterface::luaContainerHasItem(lua_State* L)
---@source ../src/luascript.cpp:7436
---@field hasItem fun(self: Container, itemId: number, subType: number): boolean
---*int LuaScriptInterface::luaContainerAddItem(lua_State* L)
---@source ../src/luascript.cpp:7449
---@field addItem fun(self: Container, itemId: number, countOrSubtype: number, index?: number, flags?: number): Item
---*int LuaScriptInterface::luaContainerAddItemEx(lua_State* L)
---@source ../src/luascript.cpp:7533
---@field addItemEx fun(self: Container, item: Item, index?: number, flags?: number): boolean
---*int LuaScriptInterface::luaContainerGetCorpseOwner(lua_State* L)
---@source ../src/luascript.cpp:7564
---@field getCorpseOwner fun(self: Container): number
---@source ../src/container.h
Container = {}

--- A teleport class is a special item that is on the ground that is used to move a player from one position to another when they step on the tile
--- <br>
--- A teleport item needs to be defined in items.xml with
--- <br>
--- <attribute key="type" value="teleport" />
--- <br>
--- Once an item is created and is a teleport, it can have a destination set to it, and when a player steps on the tile, they will be teleported to that position.
--- <br>
--- Or, it can be set in a map editor.
--- <br><br>
--- Example Usage:
--- <br>
--- ```lua
--- local tile = Tile(Position(100, 100, 7)) -- Gets the tile at position 100, 100, 7
--- local teleport = tile:addItem(1387) -- Common teleport item
--- teleport:setDestination(Position(200, 200, 7)) -- Sets the destination of the teleport to position 200, 200, 7
--- ```
---@class Teleport : Item
---*int LuaScriptInterface::luaTeleportGetDestination(lua_State* L)
---@source ../src/luascript.cpp:7640
---@field getDestination fun(self: Teleport): Position
---*int LuaScriptInterface::luaTeleportSetDestination(lua_State* L)
---@source ../src/luascript.cpp:7652
---@field setDestination fun(self: Teleport, position: Position)
Teleport = {}

--- A podium class is a special item that is used to represent a podium in the game world.
--- <br>
--- A podium can be used to display an outfit or a mount, and at a certain direction
--- <br> <br>
--- Example Useage:
--- ```lua
--- local player = Player(playerId) -- Gets the player object
--- local playerOutfit = player:getOutfit() -- Gets the outfit of the player
--- local podiumItem = Game.createItem(#####, 1, Position(100, 100, 7)) -- Creates a podium item at position 100, 100, 7
--- podiumItem:setOutfit(playerOutfit) -- Sets the outfit of the podium to the players outfit
--- ```
--- A podium needs to be defined through flags in `items.otb`
--- <br>
--- More definitions can be found in `podium.h`, and in `data\events\scripts\player.lua`
---@source ../src/podium.h
---@class Podium : Item
---*int LuaScriptInterface::luaPodiumGetOutfit(lua_State* L)
---@source ../src/luascript.cpp:7681
---@field getOutfit fun(self: Podium): Outfit
---*int LuaScriptInterface::luaPodiumSetOutfit(lua_State* L)
---@source ../src/luascript.cpp:7693
---@field setOutfit fun(self: Podium, outfit: Outfit)
---*int LuaScriptInterface::luaPodiumHasFlag(lua_State* L)
---@source ../src/luascript.cpp:7707
---@field hasFlag fun(self: Podium, flag: number): boolean
---*int LuaScriptInterface::luaPodiumSetFlag(lua_State* L)
---@source ../src/luascript.cpp:7720
---@field setFlag fun(self: Podium, flag: number)
---*int LuaScriptInterface::luaPodiumGetDirection(lua_State* L)
---@source ../src/luascript.cpp:7737
---@field getDirection fun(self: Podium): number
---*int LuaScriptInterface::luaPodiumSetDirection(lua_State* L)
---@source ../src/luascript.cpp:7749
---@field setDirection fun(self: Podium, direction: number)
---@source ../src/podium.h
Podium = {}

--- A creature class is the base class for every player/monster/npc that is spawned in the world.
--- <br>
--- When interacting with any player/monster/npc, every Creature function is inherited
--- <br><br>
--- Example usage is skipped here due to player / monsters / npc having proper examples
--- <br>
--- Further definiton of the Creature class can be found in creature.h <br>
---@source ../src/creature.h
---@class Creature : Thing
---*int LuaScriptInterface::luaCreatureCreate(lua_State* L)
---@source ../src/luascript.cpp:7764
---@field create fun(): Creature
---@field __eq fun(self: Creature, other: Creature): boolean
---*int LuaScriptInterface::luaCreatureGetEvents(lua_State* L)
---@source ../src/luascript.cpp:7792
---@field getEvents fun(self: Creature): table<number, string> -- Returns a table of all event names registered to the creature
---*int LuaScriptInterface::luaCreatureRegisterEvent(lua_State* L)
---@source ../src/luascript.cpp:7813
---@field registerEvent fun(self: Creature, eventName: string)
---*int LuaScriptInterface::luaCreatureUnregisterEvent(lua_State* L)
---@source ../src/luascript.cpp:7826
---@field unregisterEvent fun(self: Creature, eventName: string)
---*int LuaScriptInterface::luaCreatureIsRemoved(lua_State* L)
---@source ../src/luascript.cpp:7839
---@field isRemoved fun(self: Creature): boolean
---*int LuaScriptInterface::luaCreatureIsCreature(lua_State* L)
---@source ../src/luascript.cpp:7851
---@field isCreature fun(self: Creature): boolean
---*int LuaScriptInterface::luaCreatureIsInGhostMode(lua_State* L)
---@source ../src/luascript.cpp:7858
---@field isInGhostMode fun(self: Creature): boolean
---*int LuaScriptInterface::luaCreatureIsHealthHidden(lua_State* L)
---@source ../src/luascript.cpp:7870
---@field isHealthHidden fun(self: Creature): boolean
---*int LuaScriptInterface::luaCreatureIsMovementBlocked(lua_State* L)
---@source ../src/luascript.cpp:7882
---@field isMovementBlocked fun(self: Creature): boolean
---*int LuaScriptInterface::luaCreatureCanSee(lua_State* L)
---@source ../src/luascript.cpp:7894
---@field canSee fun(self: Creature, position: Position): boolean
---*int LuaScriptInterface::luaCreatureCanSeeCreature(lua_State* L)
---@source ../src/luascript.cpp:790
---@field canSeeCreature fun(self: Creature, creature: Creature): boolean
---*int LuaScriptInterface::luaCreatureCanSeeGhostMode(lua_State* L)
---@source ../src/luascript.cpp:7926
---@field canSeeGhostMode fun(self: Creature): boolean
---*int LuaScriptInterface::luaCreatureCanSeeInvisibility(lua_State* L)
---@source ../src/luascript.cpp:7945
---@field canSeeInvisibility fun(self: Creature): boolean
---*int LuaScriptInterface::luaCreatureHasParent(lua_State* L)
---@source ../src/luascript.cpp:7967
---@field hasParent fun(self: Creature): boolean
---*int LuaScriptInterface::luaCreatureGetParent(lua_State* L)
---@source ../src/luascript.cpp:7970
---@field getParent fun(self: Creature): Creature
---*int LuaScriptInterface::luaCreatureGetId(lua_State* L)
---@source ../src/luascript.cpp:7989
---@field getId fun(self: Creature): uint32
---*int LuaScriptInterface::luaCreatureGetName(lua_State* L)
---@source ../src/luascript.cpp:8001
---@field getName fun(self: Creature): string
---*int LuaScriptInterface::luaCreatureGetTarget(lua_State* L)
---@source ../src/luascript.cpp:8013
---@field getTarget fun(self: Creature): Creature
---*int LuaScriptInterface::luaCreatureSetTarget(lua_State* L)
---@source ../src/luascript.cpp:8032
---@field setTarget fun(self: Creature, target: Creature)
---*int LuaScriptInterface::luaCreatureGetFollowCreature(lua_State* L)
---*source ../src/luascript.cpp:8053
---@field getFollowCreature fun(self: Creature): Creature
---*int LuaScriptInterface::luaCreatureSetFollowCreature(lua_State* L)
---@source ../src/luascript.cpp:8072
---@field setFollowCreature fun(self: Creature, creature: Creature)
---*int LuaScriptInterface::luaCreatureGetMaster(lua_State* L)
---@source ../src/luascript.cpp:8092
---@field getMaster fun(self: Creature): Creature
---*int LuaScriptInterface::luaCreatureSetMaster(lua_State* L)
---@source ../src/luascript.cpp:8112
---@field setMaster fun(self: Creature, master: Creature)
---*int LuaScriptInterface::luaCreatureGetLight(lua_State* L)
---@source ../src/luascript.cpp:8128
---@field getLight fun(self: Creature): table -- Returns a table with the light level and color of the creature 
---*int LuaScriptInterface::luaCreatureSetLight(lua_State* L)
---@source ../src/luascript.cpp:8143
---@field setLight fun(self: Creature, lightColour: uint8, lightLevel: uint8)
---*int LuaScriptInterface::luaCreatureGetSpeed(lua_State* L)
---@source ../src/luascript.cpp:8161
---@field getSpeed fun(self: Creature): int32
---*int LuaScriptInterface::luaCreatureGetBaseSpeed(lua_State* L)
---@source ../src/luascript.cpp:8173
---@field getBaseSpeed fun(self: Creature): int32
---*int LuaScriptInterface::luaCreatureChangeSpeed(lua_State* L)
---@source ../src/luascript.cpp:8185
---@field changeSpeed fun(self: Creature, speedDelta: int32)
---*int LuaScriptInterface::luaCreatureSetDropLoot(lua_State* L)
---@source ../src/luascript.cpp:8201
---@field setDropLoot fun(self: Creature, dropLoot: boolean)
---*int LuaScriptInterface::luaCreatureSetSkillLoss(lua_State* L)
---@source ../src/luascript.cpp:8214
---@field setSkillLoss fun(self: Creature, skillLoss: boolean)
---*int LuaScriptInterface::luaCreatureGetPosition(lua_State* L)
---@source ../src/luascript.cpp:8227
---@field getPosition fun(self: Creature): Position
---*int LuaScriptInterface::luaCreatureGetTile(lua_State* L)
---@source ../src/luascript.cpp:8239
---@field getTile fun(self: Creature): Tile
---*int LuaScriptInterface::luaCreatureGetDirection(lua_State* L)
---@source ../src/luascript.cpp:8258
---@field getDirection fun(self: Creature): Directions
---*int LuaScriptInterface::luaCreatureSetDirection(lua_State* L)
---@source ../src/luascript.cpp:8270
---@field setDirection fun(self: Creature, direction: Directions)
---*int LuaScriptInterface::luaCreatureGetHealth(lua_State* L)
---@source ../src/luascript.cpp:8282
---@field getHealth fun(self: Creature): int32
---*int LuaScriptInterface::luaCreatureSetHealth(lua_State* L)
---@source ../src/luascript.cpp:8294
---@field setHealth fun(self: Creature, health: int32)
---*int LuaScriptInterface::luaCreatureAddHealth(lua_State* L)
---@source ../src/luascript.cpp:8314
---@field addHealth fun(self: Creature, healthDelta: int32)
---*int LuaScriptInterface::luaCreatureGetMaxHealth(lua_State* L)
---@source ../src/luascript.cpp:8334
---@field getMaxHealth fun(self: Creature): int32
---*int LuaScriptInterface::luaCreatureSetMaxHealth(lua_State* L)
---@source ../src/luascript.cpp:8346
---@field setMaxHealth fun(self: Creature, maxHealth: int32)
---*int LuaScriptInterface::luaCreatureSetHiddenHealth(lua_State* L)
---@source ../src/luascript.cpp:8367
---@field setHiddenHealth fun(self: Creature, hidden: boolean)
---*int LuaScriptInterface::luaCreatureSetMovementBlocked(lua_State* L)
---@source ../src/luascript.cpp:8381
---@field setMovementBlocked fun(self: Creature, blocked: boolean)
---*int LuaScriptInterface::luaCreatureGetSkull(lua_State* L)
---@source ../src/luascript.cpp:8394
---@field getSkull fun(self: Creature): Skulls
---*int LuaScriptInterface::luaCreatureSetSkull(lua_State* L)
---@source ../src/luascript.cpp:8406
---@field setSkull fun(self: Creature, skullType: Skulls)
---*int LuaScriptInterface::luaCreatureGetOutfit(lua_State* L)
---@source ../src/luascript.cpp:8418
---@field getOutfit fun(self: Creature): Outfit
---*int LuaScriptInterface::luaCreatureSetOutfit(lua_State* L)
---@source ../src/luascript.cpp:8431
---@field setOutfit fun(self: Creature, outfit: Outfit)
---*int LuaScriptInterface::luaCreatureGetCondition(lua_State* L)
---@source ../src/luascript.cpp:8445
---@field getCondition fun(self: Creature, conditionType: ConditionType, conditionId?: ConditionIds, subId?: number): Condition
---*int LuaScriptInterface::luaCreatureAddCondition(lua_State* L)
---@source ../src/luascript.cpp:8468
---@field addCondition fun(self: Creature, condition: Condition)
---*int LuaScriptInterface::luaCreatureRemoveCondition(lua_State* L)
---@source ../src/luascript.cpp:8482
---@field removeCondition fun(self: Creature, conditionType: ConditionType, conditionId?: ConditionIds, subId?: number, force?: boolean): boolean
---*int LuaScriptInterface::luaCreatureHasCondition(lua_State* L)
---@source ../src/luascript.cpp:8519
---@field hasCondition fun(self: Creature, conditionType: ConditionType, subId?: number): boolean
---*int LuaScriptInterface::luaCreatureIsImmune(lua_State* L)
---@source ../src/luascript.cpp:8534
---@field isImmune fun(self: Creature, conditionInfo: Condition|ConditionType): boolean
---*int LuaScriptInterface::luaCreatureRemove(lua_State* L)
---@source ../src/luascript.cpp:8553
---@field remove fun(self: Creature)
---*int LuaScriptInterface::luaCreatureTeleportTo(lua_State* L)
---@source ../src/luascript.cpp:8580
---@field teleportTo fun(self: Creature, position: Position, pushMovement?: boolean): boolean
---*int LuaScriptInterface::luaCreatureSay(lua_State* L)
---@source ../src/luascript.cpp:8615
---@field say fun(self: Creature, text: string, type?: SpeakClasses, ghost?: boolean, receiver?: Creature, position?: Position): boolean
---*int LuaScriptInterface::luaCreatureGetDamageMap(lua_State* L)
---@source ../src/luascript.cpp:8661
---@field getDamageMap fun(self: Creature): table<number, {total: int32, ticks: int32}>
---*int LuaScriptInterface::luaCreatureGetSummons(lua_State* L)
---@source ../src/luascript.cpp:8680
---@field getSummons fun(self: Creature): table<number, Creature>
---*int LuaScriptInterface::luaCreatureGetDescription(lua_State* L)
---@source ../src/luascript.cpp:8700
---@field getDescription fun(self: Creature, distance: int32): string
---*int LuaScriptInterface::luaCreatureGetPathTo(lua_State* L)
---@source ../src/luascript.cpp:8713
---@field getPathTo fun(self: Creature, position: Position, minTargetDist?: int32, maxTargetDist?: int32, fullPathSearch?: boolean, clearSight?: boolean, maxSearchDist?: int32): table<number, Directions>
---*int LuaScriptInterface::luaCreatureMove(lua_State* L)
---@source ../src/luascript.cpp:8747
---@field move fun(self: Creature, directionOrTile: Directions|Tile, flags?: TileFlags): number -- Returns either RETURNVALUE_NOTPOSSIBLE if the creature cannot be moved there, or RETURNVALUE_NOERROR if success
---*int LuaScriptInterface::luaCreatureGetZone(lua_State* L)
---@source ../src/luascript.cpp:8775
---@field getZone fun(self: Creature): ZoneTypes
---*int LuaScriptInterface::luaCreatureHasIcon(lua_State* L)
---@source ../src/luascript.cpp:8787
---@field hasIcon fun(self: Creature, iconId: CreatureIcons): boolean
---*int LuaScriptInterface::luaCreatureSetIcon(lua_State* L)
---@source ../src/luascript.cpp:8799
---@field setIcon fun(self: Creature, iconId: CreatureIcons, value: uint16)
---*int LuaScriptInterface::luaCreatureGetIcon(lua_State* L)
---@source ../src/luascript.cpp:8822
---@field getIcon fun(self: Creature, iconId: CreatureIcons): uint16
---*int LuaScriptInterface::luaCreatureRemoveIcon(lua_State* L)
---@source ../src/luascript.cpp:8842
---@field removeIcon fun(self: Creature, iconId: CreatureIcons): boolean
---*int LuaScriptInterface::luaCreatureGetStorageValue(lua_State* L)
---@source ../src/luascript.cpp:8865
---@field getStorageValue fun(self: Creature, key: uint32): int32
---*int LuaScriptInterface::luaCreatureSetStorageValue(lua_State* L)
---@source ../src/luascript.cpp:8883
---@field setStorageValue fun(self: Creature, key: uint32, value: int32)
Creature = {}

--- The Player class is representing a logged in and active player in the game world
--- <br>
--- It extends the Creature class, and inherits all functions and variables from Creature.
--- <br><br>
--- Example usage:
--- ```lua
--- local player = Player(playerId) -- Gets the player object
--- local playerName = player:getName() -- Gets the name of the player
--- print(playerName) -- Prints the name of the player
--- player:addExperience(1000) -- Adds 1000 experience to the player
--- player:setMaxHealth(200) -- Sets the max health of the player to 200
--- player:teleportTo(Position(100, 100, 7)) -- Teleports the player to position 100, 100, 7
--- --- etc...
--- ```
--- *Note*: Once you edit something about a player, setting maximum health, adding experience, etc... It WILL be saved to the player once the player saves
--- <br>
--- To temporarily add stats to a player, use a condition
--- <br><br>
--- Further definition of the player class can be found in:
--- <ol>
--- <li> player.h </li>
--- <li> data/lib/core/player.lua </li>
--- </ol>
---@source ../src/player.h
---@class Player : Creature
---*int LuaScriptInterface::luaPlayerCreate(lua_State* L)
---@source ../src/luascript.cpp:8911
---@field create fun(playerId_GUID_Name_UserData: uint32|string|Player): Player -- To get the userdata of a player, use Player(id/guid/name/userdata)
---@field __eq fun(self: Player, other: Player): boolean
---*int LuaScriptInterface::luaPlayerIsPlayer(lua_State* L)
---@source ../src/luascript.cpp:8948
---@field isPlayer fun(self: Player): boolean
---*int LuaScriptInterface::luaPlayerGetGuid(lua_State* L)
---@source ../src/luascript.cpp:8955
---@field getGuid fun(self: Player): uint32 -- Unique identifier for the player, used in the database
---*int LuaScriptInterface::luaPlayerGetIp(lua_State* L)
---@source ../src/luascript.cpp:8967
---@field getIp fun(self: Player): string -- IP address of the player as a string
---*int LuaScriptInterface::luaPlayerGetAccountId(lua_State* L)
---@source ../src/luascript.cpp:8979
---@field getAccountId fun(self: Player): uint32 -- Account ID of the player, used in the database
---*int LuaScriptInterface::luaPlayerGetLastLoginSaved(lua_State* L)
---@source ../src/luascript.cpp:8991
---@field getLastLoginSaved fun(self: Player): number -- Returns timestamp of the last login saved in the database
---*int LuaScriptInterface::luaPlayerGetLastLogout(lua_State* L)
---@source ../src/luascript.cpp:9003
---@field getLastLogout fun(self: Player): number -- Returns timestamp of the last logout saved in the database
---*int LuaScriptInterface::luaPlayerGetAccountType(lua_State* L)
---@source ../src/luascript.cpp:9015
---@field getAccountType fun(self: Player): AccountTypes
---*int LuaScriptInterface::luaPlayerSetAccountType(lua_State* L)
---@source ../src/luascript.cpp:9027
---@field setAccountType fun(self: Player, accountType: AccountTypes)
---*int LuaScriptInterface::luaPlayerGetCapacity(lua_State* L)
---@source ../src/luascript.cpp:9041
---@field getCapacity fun(self: Player): uint32
---*int LuaScriptInterface::luaPlayerSetCapacity(lua_State* L)
---@source ../src/luascript.cpp:9053
---@field setCapacity fun(self: Player, capacity: uint32)
---*int LuaScriptInterface::luaPlayerGetFreeCapacity(lua_State* L)
---@source ../src/luascript.cpp:9067
---@field getFreeCapacity fun(self: Player): uint32
---*int LuaScriptInterface::luaPlayerGetDepotChest(lua_State* L)
---@source ../src/luascript.cpp:9079
---@field getDepotChest fun(self: Player, depotId: uint32, autoCreate?: boolean): Item
---*int LuaScriptInterface::luaPlayerGetInbox(lua_State* L)
---@source ../src/luascript.cpp:9100
---@field getInbox fun(self: Player): Item
---*int LuaScriptInterface::luaPlayerGetSkullTime(lua_State* L)
---@source ../src/luascript.cpp:9119
---@field getSkullTime fun(self: Player): int64
---*int LuaScriptInterface::luaPlayerSetSkullTime(lua_State* L)
---@source ../src/luascript.cpp:9131
---@field setSkullTime fun(self: Player, time: int64)
---*int LuaScriptInterface::luaPlayerGetDeathPenalty(lua_State* L)
---@source ../src/luascript.cpp:9144
---@field getDeathPenalty fun(self: Player): double
---*int LuaScriptInterface::luaPlayerGetExperience(lua_State* L)
---@source ../src/luascript.cpp:9156
---@field getExperience fun(self: Player): uint64
---*int LuaScriptInterface::luaPlayerAddExperience(lua_State* L)
---@source ../src/luascript.cpp:9168
---@field addExperience fun(self: Player, amount: uint64): boolean? -- Will return true if player, nil if there is no player 
---*int LuaScriptInterface::luaPlayerRemoveExperience(lua_State* L)
---@source ../src/luascript.cpp:9183
---@field removeExperience fun(self: Player, amount: uint64): boolean? --- Will return true if player, nil if there is no player
---*int LuaScriptInterface::luaPlayerGetLevel(lua_State* L)
---@source ../src/luascript.cpp:9198
---@field getLevel fun(self: Player): uint32
---*int LuaScriptInterface::luaPlayerGetLevelPercent(lua_State* L)
---@source ../src/luascript.cpp:9210
---@field getLevelPercent fun(self: Player): uint8
---*int LuaScriptInterface::luaPlayerGetMagicLevel(lua_State* L)
---@source ../src/luascript.cpp:9222
---@field getMagicLevel fun(self: Player): uint32
---*int LuaScriptInterface::luaPlayerGetMagicLevelPercent(lua_State* L)
---@source ../src/luascript.cpp:9234
---@field getMagicLevelPercent fun(self: Player): uint16
---*int LuaScriptInterface::luaPlayerGetBaseMagicLevel(lua_State* L)
---@source ../src/luascript.cpp:9246
---@field getBaseMagicLevel fun(self: Player): uint32
---*int LuaScriptInterface::luaPlayerGetMana(lua_State* L)
---@source ../src/luascript.cpp:9258
---@field getMana fun(self: Player): uint32
---*int LuaScriptInterface::luaPlayerAddMana(lua_State* L)
---@source ../src/luascript.cpp:9270
---@field addMana fun(self: Player, amount: int32, animationOnLoss?: boolean)
---*int LuaScriptInterface::luaPlayerGetMaxMana(lua_State* L)
---@source ../src/luascript.cpp:9293
---@field getMaxMana fun(self: Player): uint32
---*int LuaScriptInterface::luaPlayerSetMaxMana(lua_State* L)
---@source ../src/luascript.cpp:9305
---@field setMaxMana fun(self: Player, maxMana: int32)
---*int LuaScriptInterface::luaPlayerSetManaShieldBar(lua_State* L)
---@source ../src/luascript.cpp:9320
---@field setManaShieldBar fun(self: Player, capacity: uint16, value: uint16) -- Capacity is the "max mana", value is the current mana
---*int LuaScriptInterface::luaPlayerGetManaSpent(lua_State* L)
---@source ../src/luascript.cpp:9335
---@field getManaSpent fun(self: Player): uint64
---*int LuaScriptInterface::luaPlayerAddManaSpent(lua_State* L)
---@source ../src/luascript.cpp:9347
---@field addManaSpent fun(self: Player, amount: uint64)
---*int LuaScriptInterface::luaPlayerRemoveManaSpent(lua_State* L)
---@source ../src/luascript.cpp:9360
---@field removeManaSpent fun(self: Player, amount: uint64, notify?: boolean) -- Notify is set to true by default
---*int LuaScriptInterface::luaPlayerGetBaseMaxHealth(lua_State* L)
---@source ../src/luascript.cpp:9373
---@field getBaseMaxHealth fun(self: Player): int32
---*int LuaScriptInterface::luaPlayerGetBaseMaxMana(lua_State* L)
---@source ../src/luascript.cpp:9385
---@field getBaseMaxMana fun(self: Player): uint32
---*int LuaScriptInterface::luaPlayerGetSkillLevel(lua_State* L)
---@source ../src/luascript.cpp:9397
---@field getSkillLevel fun(self: Player, skill: SkillTypes): uint8
---*int LuaScriptInterface::luaPlayerGetEffectiveSkillLevel(lua_State* L)
---@source ../src/luascript.cpp:9410
---@field getEffectiveSkillLevel fun(self: Player, skill: SkillTypes): uint16
---*int LuaScriptInterface::luaPlayerGetSkillPercent(lua_State* L)
---@source ../src/luascript.cpp:9423
---@field getSkillPercent fun(self: Player, skill: SkillTypes): uint16
---*int LuaScriptInterface::luaPlayerGetSkillTries(lua_State* L)
---@source ../src/luascript.cpp:9436
---@field getSkillTries fun(self: Player, skill: SkillTypes): uint64
---*int LuaScriptInterface::luaPlayerAddSkillTries(lua_State* L)
---@source ../src/luascript.cpp:9449
---@field addSkillTries fun(self: Player, skill: SkillTypes, amount: uint64)
---*int LuaScriptInterface::luaPlayerRemoveSkillTries(lua_State* L)
---@source ../src/luascript.cpp:9464
---@field removeSkillTries fun(self: Player, skill: SkillTypes, amount: uint64, notify?: boolean) -- Notify is set to true by default
---*int LuaScriptInterface::luaPlayerGetSpecialSkill(lua_State* L)
---@source ../src/luascript.cpp:9479
---@field getSpecialSkill fun(self: Player, skill: SpecialSkills): uint16 --! Why is this uint16. getSpecialSkill returns uint16, however varSpecialSkills is a int32
---*int LuaScriptInterface::luaPlayerAddSpecialSkill(lua_State* L)
---@source ../src/luascript.cpp:9492
---@field addSpecialSkill fun(self: Player, skill: SpecialSkills, amount: int32)
---*int LuaScriptInterface::luaPlayerAddOfflineTrainingTime(lua_State* L)
---@source ../src/luascript.cpp:9513
---@field addOfflineTrainingTime fun(self: Player, time: int32)
---*int LuaScriptInterface::luaPlayerGetOfflineTrainingTime(lua_State* L)
---@source ../src/luascript.cpp:9528
---@field getOfflineTrainingTime fun(self: Player): int32
---*int LuaScriptInterface::luaPlayerRemoveOfflineTrainingTime(lua_State* L)
---@source ../src/luascript.cpp:9540
---@field removeOfflineTrainingTime fun(self: Player, time: int32)
---*int LuaScriptInterface::luaPlayerAddOfflineTrainingTries(lua_State* L)
---@source ../src/luascript.cpp:9555
---@field addOfflineTrainingTries fun(self: Player, skill: SkillTypes, amount: uint64): boolean? -- Returns nil if no player, true if player had offline training tries added, false if not
---*int LuaScriptInterface::luaPlayerGetOfflineTrainingSkill(lua_State* L)
---@source ../src/luascript.cpp:9569
---@field getOfflineTrainingSkill fun(self: Player): int32
---*int LuaScriptInterface::luaPlayerSetOfflineTrainingSkill(lua_State* L)
---@source ../src/luascript.cpp:9581
---@field setOfflineTrainingSkill fun(self: Player, skill: SkillTypes)
---*int LuaScriptInterface::luaPlayerGetItemCount(lua_State* L)
---@source ../src/luascript.cpp:9595
---@field getItemCount fun(self: Player, itemId: uint16, subType?: int32): uint32
---*int LuaScriptInterface::luaPlayerGetItemById(lua_State* L)
---@source ../src/luascript.cpp:9620
---@field getItemById fun(self: Player, itemId: uint16, deepSearch?:boolean, subType?: int32): Item
---*int LuaScriptInterface::luaPlayerGetVocation(lua_State* L)
---@source ../src/luascript.cpp:9652
---@field getVocation fun(self: Player): Vocation
---*int LuaScriptInterface::luaPlayerSetVocation(lua_State* L)
---@source ../src/luascript.cpp:9665
---@field setVocation fun(self: Player, vocationId: uint16|string|Vocation): boolean
---*int LuaScriptInterface::luaPlayerGetSex(lua_State* L)
---@source ../src/luascript.cpp:9695
---@field getSex fun(self: Player): PlayerSexes
---*int LuaScriptInterface::luaPlayerSetSex(lua_State* L)
---@source ../src/luascript.cpp:9707
---@field setSex fun(self: Player, sexId: PlayerSexes)
---*int LuaScriptInterface::luaPlayerGetTown(lua_State* L)
---@source ../src/luascript.cpp:9721
---@field getTown fun(self: Player): Town
---*int LuaScriptInterface::luaPlayerSetTown(lua_State* L)
---@source ../src/luascript.cpp:9733
---@field setTown fun(self: Player, townId: uint32)
---*int LuaScriptInterface::luaPlayerGetGuild(lua_State* L)
---@source ../src/luascript.cpp:9757
---@field getGuild fun(self: Player): Guild
---*int LuaScriptInterface::luaPlayerSetGuild(lua_State* L)
---@source ../src/luascript.cpp:9775
---@field setGuild fun(self: Player, guild: Guild)
---*int LuaScriptInterface::luaPlayerGetGuildLevel(lua_State* L)
---@source ../src/luascript.cpp:9789
---@field getGuildLevel fun(self: Player): uint8 -- GuildLevel is the rank of the player in the guild
---*int LuaScriptInterface::luaPlayerSetGuildLevel(lua_State* L)
---@source ../src/luascript.cpp:9801
---@field setGuildLevel fun(self: Player, level: uint8) -- GuildLevel is the rank of the player in the guild
---*int LuaScriptInterface::luaPlayerGetGuildNick(lua_State* L)
---@source ../src/luascript.cpp:9826
---@field getGuildNick fun(self: Player): string
---*int LuaScriptInterface::luaPlayerSetGuildNick(lua_State* L)
---@source ../src/luascript.cpp:9838
---@field setGuildNick fun(self: Player, nick: string)
---*int LuaScriptInterface::luaPlayerGetGroup(lua_State* L)
---@source ../src/luascript.cpp:9852
---@field getGroup fun(self: Player): Group
---*int LuaScriptInterface::luaPlayerSetGroup(lua_State* L)
---@source ../src/luascript.cpp:9865
---@field setGroup fun(self: Player, groupId: uint16)
---*int LuaScriptInterface::luaPlayerGetStamina(lua_State* L)
---@source ../src/luascript.cpp:9884
---@field getStamina fun(self: Player): uint16
---*int LuaScriptInterface::luaPlayerSetStamina(lua_State* L)
---@source ../src/luascript.cpp:9896
---@field setStamina fun(self: Player, stamina: uint16)
---*int LuaScriptInterface::luaPlayerGetSoul(lua_State* L)
---@source ../src/luascript.cpp:9911
---@field getSoul fun(self: Player): uint8
---*int LuaScriptInterface::luaPlayerAddSoul(lua_State* L)
---@source ../src/luascript.cpp:9923
---@field addSoul fun(self: Player, amount: int32)
---*int LuaScriptInterface::luaPlayerGetMaxSoul(lua_State* L)
---@source ../src/luascript.cpp:9937
---@field getMaxSoul fun(self: Player): uint8
---*int LuaScriptInterface::luaPlayerGetBankBalance(lua_State* L)
---@source ../src/luascript.cpp:9949
---@field getBankBalance fun(self: Player): uint64
---*int LuaScriptInterface::luaPlayerSetBankBalance(lua_State* L)
---@source ../src/luascript.cpp:9961
---@field setBankBalance fun(self: Player, balance: uint64)
---*int LuaScriptInterface::luaPlayerAddItem(lua_State* L)
---@source ../src/luascript.cpp:9982
---@field addItem fun(self: Player, itemId: uint16|string, count?: int32, canDropOnMap?: boolean, subType?: int32, slot?: PlayerEquipmentSlots): Item
---*int LuaScriptInterface::luaPlayerAddItemEx(lua_State* L)
---@source ../src/luascript.cpp:10068
---@field addItemEx fun(self: Player, item: Item, dropOnMap?: boolean, indexOrSlot?: PlayerEquipmentSlots|int32): boolean -- If dropOnMap is FALSE, index = INDEXWHEREVER unless defined and returns internalPlayerAddItem, if dropOnMap is TRUE, slot = CONST_SLOT_WHEREEVER, and returns internalAddItem
---*int LuaScriptInterface::luaPlayerRemoveItem(lua_State* L)
---@source ../src/luascript.cpp:10109 
---@field removeItem fun(self: Player, itemId: uint16|string, count?: uint32, subtype?: int32, ignoreEquipped?: boolean): boolean 
---*int LuaScriptInterface::luaPlayerSendSupplyUsed(lua_State* L)
---@source ../src/luascript.cpp:10136
---@field sendSupplyUsed fun(self: Player, item: Item)
---*int LuaScriptInterface::luaPlayerGetMoney(lua_State* L)
---@source ../src/luascript.cpp:10158
---@field getMoney fun(self: Player): uint64
---*int LuaScriptInterface::luaPlayerAddMoney(lua_State* L)
---@source ../src/luascript.cpp:10170
---@field addMoney fun(self: Player, amount: uint64)
---*int LuaScriptInterface::luaPlayerRemoveMoney(lua_State* L)
---@source ../src/luascript.cpp:10184
---@field removeMoney fun(self: Player, amount: uint64)
---*int LuaScriptInterface::luaPlayerShowTextDialog(lua_State* L)
---@source ../src/luascript.cpp:10197
---@field showTextDialog fun(self: Player, itemToShow: uint16|string|Item, text: string, canWrite: boolean, length: int32): boolean?|uint32 -- Returns nil if no player. Returns false if cannot show the window. Push 
---*int LuaScriptInterface::luaPlayerSendTextMessage(lua_State* L)
---@source ../src/luascript.cpp:10255
---@field sendTextMessage fun(self: Player, messageType: MessageTypes, message: string, position?: Position|number, primaryValue?: int32, primaryColor?: TextColors, secondaryValue?: int32, secondaryColor?: TextColors): boolean? -- Returns nil if no player, true if message sent, false if cannot send message (e.g. player is not part of that channel)
---*int LuaScriptInterface::luaPlayerSendChannelMessage(lua_State* L)
---@source ../src/luascript.cpp:10296
---@field sendChannelMessage fun(self: Player, author: string, message: string, type: SpeakClasses, channelId: uint16): boolean? -- Returns nil if no player, true if message sent
---*int LuaScriptInterface::luaPlayerSendPrivateMessage(lua_State* L)
---@source ../src/luascript.cpp:10314
---@field sendPrivateMessage fun(self: Player, receiver: Player, message: string, type?: SpeakClasses)
---*int LuaScriptInterface::luaPlayerChannelSay(lua_State* L)
---@source ../src/luascript.cpp:10331
---@field channelSay fun(self: Player, speaker: Creature, type: SpeakClasses, message: string, channelId: uint16)
---*int LuaScriptInterface::luaPlayerOpenChannel(lua_State* L)
---@source ../src/luascript.cpp:10349
---@field openChannel fun(self: Player, channelId: uint16)
---*int LuaScriptInterface::luaPlayerLeaveChannel(lua_State* L)
---@source ../src/luascript.cpp:10363
---@field leaveChannel fun(self: Player, channelId: uint16)
---*int LuaScriptInterface::luaPlayerGetSlotItem(lua_State* L)
---@source ../src/luascript.cpp:10377
---@field getSlotItem fun(self: Player, slot: PlayerEquipmentSlots): Item
---*int LuaScriptInterface::luaPlayerGetParty(lua_State* L)
---@source ../src/luascript.cpp:10403
---@field getParty fun(self: Player): Party
---*int LuaScriptInterface::luaPlayerAddOutfit(lua_State* L)
---@source ../src/luascript.cpp:10422
---@field addOutfit fun(self: Player, outfitId: uint16)
---*int LuaScriptInterface::luaPlayerAddOutfitAddon(lua_State* L)
---@source ../src/luascript.cpp:10435
---@field addOutfitAddon fun(self: Player, outfitId: uint16, addonId: uint8)
---*int LuaScriptInterface::luaPlayerRemoveOutfit(lua_State* L)
---@source ../src/luascript.cpp:10450
---@field removeOutfit fun(self: Player, outfitId: uint16)
---*int LuaScriptInterface::luaPlayerRemoveOutfitAddon(lua_State* L)
---@source ../src/luascript.cpp:10463
---@field removeOutfitAddon fun(self: Player, outfitId: uint16, addonId: uint8)
---*int LuaScriptInterface::luaPlayerHasOutfit(lua_State* L)
---@source ../src/luascript.cpp:10477
---@field hasOutfit fun(self: Player, outfitId: uint16, addon?: uint8): boolean
---*int LuaScriptInterface::luaPlayerCanWearOutfit(lua_State* L)
---@source ../src/luascript.cpp:10491
---@field canWearOutfit fun(self: Player, outfitId: uint16, addonId?: uint8): boolean
---*int LuaScriptInterface::luaPlayerSendOutfitWindow(lua_State* L)
---@source ../src/luascript.cpp:10505
---@field sendOutfitWindow fun(self: Player)
---*int LuaScriptInterface::luaPlayerSendEditPodium(lua_State* L)
---@source ../src/luascript.cpp:10518
---@field sendEditPodium fun(self: Player, item: Item)
---*int LuaScriptInterface::luaPlayerAddMount(lua_State* L)
---@source ../src/luascript.cpp:10533
---@field addMount fun(self: Player, mountId: uint16): boolean? -- Returns nil if no player or no mount, false if player already has mount, true if tamed
---*int LuaScriptInterface::luaPlayerRemoveMount(lua_State* L)
---@source ../src/luascript.cpp:10556
---@field removeMount fun(self: Player, mountId: uint16): boolean? -- Returns nil if no player or no mount, false if player does not have mount, true if untamed
---*int LuaScriptInterface::luaPlayerHasMount(lua_State* L)
---@source ../src/luascript.cpp:10580
---@field hasMount fun(self: Player, mountId: uint16): boolean
---*int LuaScriptInterface::luaPlayerToggleMount(lua_State* L)
---@source ../src/luascript.cpp:10604
---@field toggleMount fun(self: Player, active: boolean)
---*int LuaScriptInterface::luaPlayerGetPremiumEndsAt(lua_State* L)
---@source ../src/luascript.cpp:10618
---@field getPremiumEndsAt fun(self: Player): number
---*int LuaScriptInterface::luaPlayerSetPremiumEndsAt(lua_State* L)
---@source ../src/luascript.cpp:10630
---@field setPremiumEndsAt fun(self: Player, timestamp: number)
---*int LuaScriptInterface::luaPlayerHasBlessing(lua_State* L)
---@source ../src/luascript.cpp:10647
---@field hasBlessing fun(self: Player, blessingId: uint8): boolean? -- Returns nil if no player, true if player has blessing, false if player does not have blessing
---*int LuaScriptInterface::luaPlayerAddBlessing(lua_State* L)
---@source ../src/luascript.cpp:10660
---@field addBlessing fun(self: Player, blessingId: uint8): boolean? -- Returns nil if no player, true if player did not have the blessing and it was added, false if player already had the blessing
---*int LuaScriptInterface::luaPlayerRemoveBlessing(lua_State* L)
---@source ../src/luascript.cpp:10680
---@field removeBlessing fun(self: Player, blessingId: uint8): boolean? -- Returns nil if no player, true if player had the blessing and it was removed, false if player did not have the blessing
---*int LuaScriptInterface::luaPlayerCanLearnSpell(lua_State* L)
---@source ../src/luascript.cpp:10700
---@field canLearnSpell fun(self: Player, spellName: string): boolean? -- Returns nil if no player, true if player can learn the spell, false if player cannot learn the spell
---*int LuaScriptInterface::luaPlayerLearnSpell(lua_State* L)
---@source ../src/luascript.cpp:10734
---@field learnSpell fun(self: Player, spellName: string): boolean? -- Returns nil if no player, true if player. Does not check if player already had the spell
---*int LuaScriptInterface::luaPlayerForgetSpell(lua_State* L)
---@source ../src/luascript.cpp:10748
---@field forgetSpell fun(self: Player, spellName: string): boolean? -- Returns nil if no player, true if player. Does not check if player forgot the spell
---*int LuaScriptInterface::luaPlayerHasLearnedSpell(lua_State* L)
---@source ../src/luascript.cpp:10762
---@field hasLearnedSpell fun(self: Player, spellName: string): boolean -- Retrusn nil if no player, true if player has learned the spell, false if player has not learned the spell
---*int LuaScriptInterface::luaPlayerSendTutorial(lua_State* L)
---@source ../src/luascript.cpp:10775
---@field sendTutorial fun(self: Player, tutorialId: uint8)
---*int LuaScriptInterface::luaPlayerAddMapMark(lua_State* L)
---@source ../src/luascript.cpp:10789
---@field addMapMark fun(self: Player, position: Position, type: MapMarks, description?: string)
---*int LuaScriptInterface::luaPlayerSave(lua_State* L)
---@source ../src/luascript.cpp:10805
---@field save fun(self: Player)
---*int LuaScriptInterface::luaPlayerPopupFYI(lua_State* L)
---@source ../src/luascript.cpp:10818
---@field popupFYI fun(self: Player, message: string)
---*int LuaScriptInterface::luaPlayerIsPzLocked(lua_State* L)
---@source ../src/luascript.cpp:10832
---@field isPzLocked fun(self: Player): boolean
---*int LuaScriptInterface::luaPlayerGetClient(lua_State* L)
---@source ../src/luascript.cpp:10844
---@field getClient fun(self: Player): UserClientVersion -- ClientVersion is a table {version = uint16, os = OperatingSystem}
---*int LuaScriptInterface::luaPlayerGetHouse(lua_State* L)
---@source ../src/luascript.cpp:10858
---@field getHouse fun(self: Player): House
---*int LuaScriptInterface::luaPlayerSendHouseWindow(lua_State* L)
---@source ../src/luascript.cpp:10877
---@field sendHouseWindow fun(self: Player, houseId: uint32, listId: uint32)
---*int LuaScriptInterface::luaPlayerSetEditHouse(lua_State* L)
---@source ../src/luascript.cpp:10898
---@field setEditHouse fun(self: Player, houseId: uint32, listId: uint32)
---*int LuaScriptInterface::luaPlayerSetGhostMode(lua_State* L)
---@source ../src/luascript.cpp:10919
---@field setGhostMode fun(self: Player, enabled: boolean, magicEffect?: MagicEffectConsts)
---*int LuaScriptInterface::luaPlayerGetContainerId(lua_State* L)
---@source ../src/luascript.cpp:10983
---@field getContainerId fun(self: Player, container: Container): int8
---*int LuaScriptInterface::luaPlayerGetContainerById(lua_State* L) 
---@source ../src/luascript.cpp:11001
---@field getContainerById fun(self: Player, containerId: int8): Container
---*int LuaScriptInterface::luaPlayerGetContainerIndex(lua_State* L)
---@source ../src/luascript.cpp:11020
---@field getContainerIndex fun(self: Player, containerIndex: uint8): uint16
---*int LuaScriptInterface::luaPlayerGetInstantSpells(lua_State* L)
---@source ../src/luascript.cpp:11032
---@field getInstantSpells fun(self: Player): table<number, SpellsTable> -- Spells table has the following information in the table: {name = string, words = string, level = uint32, mlevel = uint32, mana = uint32, manapercent = uint32, params = boolean}
---*int LuaScriptInterface::luaPlayerCanCast(lua_State* L)
---@source ../src/luascript.cpp:11068
---@field canCast fun(self: Player, spellId: number): boolean
---*int LuaScriptInterface::luaPlayerHasChaseMode(lua_State* L)
---@source ../src/luascript.cpp:11081
---@field hasChaseMode fun(self: Player): boolean
---*int LuaScriptInterface::luaPlayerHasSecureMode(lua_State* L)
---@source ../src/luascript.cpp:11093
---@field hasSecureMode fun(self: Player): boolean
---*int LuaScriptInterface::luaPlayerGetFightMode(lua_State* L)
---@source ../src/luascript.cpp:11105
---@field getFightMode fun(self: Player): FightModes
---*int LuaScriptInterface::luaPlayerGetStoreInbox(lua_State* L)
---@source ../src/luascript.cpp:11117
---@field getStoreInbox fun(self: Player): Container
---*int LuaScriptInterface::luaPlayerIsNearDepotBox(lua_State* L)
---@source ../src/luascript.cpp:11137
---@field isNearDepotBox fun(self: Player): boolean
---*int LuaScriptInterface::luaPlayerGetIdleTime(lua_State* L)
---@source ../src/luascript.cpp:11150
---@field getIdleTime fun(self: Player): int32
---*int LuaScriptInterface::luaPlayerResetIdleTime(lua_State* L)
---@source ../src/luascript.cpp:11163
---@field resetIdleTime fun(self: Player)
---*int LuaScriptInterface::luaPlayerSendCreatureSquare(lua_State* L)
---@source ../src/luascript.cpp:11177
---@field sendCreatureSquare fun(self: Player, creature: Creature, color: SquareColours)
---*int LuaScriptInterface::luaPlayerGetClientExpDisplay(lua_State* L)
---@source ../src/luascript.cpp:11198
---@field getClientExpDisplay fun(self: Player): uint16
---*int LuaScriptInterface::luaPlayerSetClientExpDisplay(lua_State* L)
---@source ../src/luascript.cpp:11210
---@field setClientExpDisplay fun(self: Player, display: uint16)
---*int LuaScriptInterface::luaPlayerGetClientStaminaBonusDisplay(lua_State* L)
---@source ../src/luascript.cpp:11224
---@field getClientStaminaBonusDisplay fun(self: Player): uint16
---*int LuaScriptInterface::luaPlayerSetClientStaminaBonusDisplay(lua_State* L)
---@source ../src/luascript.cpp:11236
---@field setClientStaminaBonusDisplay fun(self: Player, display: uint16)
---*int LuaScriptInterface::luaPlayerGetClientLowLevelBonusDisplay(lua_State* L)
---@source ../src/luascript.cpp:11250
---@field getClientLowLevelBonusDisplay fun(self: Player): uint16
---*int LuaScriptInterface::luaPlayerSetClientLowLevelBonusDisplay(lua_State* L)
---@source ../src/luascript.cpp:11262
---@field setClientLowLevelBonusDisplay fun(self: Player, display: uint16)
---*int LuaScriptInterface::luaPlayerSendResourceBalance(lua_State* L)
---@source ../src/luascript.cpp:11276
---@field sendResourceBalance fun(self: Player, resource: ResourceTypes, amount: uint64): boolean?
---*int LuaScriptInterface::luaPlayerSendEnterMarket(lua_State* L)
---@source ../src/luascript.cpp:11291
---@field sendEnterMarket fun(self: Player): boolean?
---@source ../src/player.h
Player = {}

--- The Monster class, extending Creature, is used to represent monsters in the game.
--- <br>
--- To get the userdata for a Monster, use the function `= Monster(monsterId)`.
--- <br>
--- The monster creature is defined in monster.h. When loading the server, all types of monsters are loaded as MonsterType classes.
--- <br>
--- When a monster spawns, it will take all the information from a MonsterType class and apply it to the monster. 
--- <br><br>
--- Example Usage:
--- ```lua
--- local player = Player(playerId) -- Getting a player
--- local targetMonster = player:getTarget() -- Getting the target of the player. For this example, it is a monster
--- if not targetMonster and not targetMonster:isMonster() then
---   print("Target is not a monster.") -- Player was either not targeting anything, or targeting something not a player.
--- end
--- 
--- print("Target monster ID: " .. targetMonster:getId()) -- Prints the ID of the monster
--- print("Target monster name: " .. targetMonster:getName()) -- Prints the name of the monster
--- tdump("MonsterDamageMap", targetMonster:getDamageMap()) -- Prints the damage map of the monster
---
--- local monster = Game.createMonster("rat") -- Creates a monster that has the name "rat", if it exists
--- print("Monster ID: " .. monster:getId()) -- Prints the ID of the monster
--- print("Monster name: " .. monster:getName()) -- Prints the name of the monster
--- print("Monster type: " .. monster:getType()) -- Prints the type of the monster
--- ```
--- <br>
--- <br>
--- Further definitions of the Monster class can be found in monster.h.
---@source ../src/monster.h
---@class Monster : Creature
---*int LuaScriptInterface::luaMonsterCreate(lua_State* L)
---@source ../src/luascript.cpp:11305
---@field create fun(monsterInfo: uint32|Monster): Monster -- Monster info can either be the ID of the monster, or user data of the monster
---@field __eq fun(self: Monster, other: Monster): boolean
---*int LuaScriptInterface::luaMonsterIsMonster(lua_State* L)
---@source ../src/luascript.cpp:11330
---@field isMonster fun(self: Monster): boolean
---*int LuaScriptInterface::luaMonsterGetId(lua_State* L)
---@source ../src/luascript.cpp:11337
---@field getId fun(self: Monster): uint32
---*int LuaScriptInterface::luaMonsterGetType(lua_State* L)
---@source ../src/luascript.cpp:11354
---@field getType fun(self: Monster): MonsterType
---*int LuaScriptInterface::luaMonsterRename(lua_State* L)
---@source ../src/luascript.cpp:11367
---@field rename fun(self: Monster, newName: string, nameDescription?: string)
---*int LuaScriptInterface::luaMonsterGetSpawnPosition(lua_State* L)
---@source ../src/luascript.cpp:11385
---@field getSpawnPosition fun(self: Monster): Position
---*int LuaScriptInterface::luaMonsterIsInSpawnRange(lua_State* L)
---@source ../src/luascript.cpp:11397
---@field isInSpawnRange fun(self: Monster, position?: Position): boolean
---*int LuaScriptInterface::luaMonsterIsIdle(lua_State* L)
---@source ../src/luascript.cpp:11410
---@field isIdle fun(self: Monster): boolean
---*int LuaScriptInterface::luaMonsterSetIdle(lua_State* L)
---@source ../src/luascript.cpp:11422
---@field setIdle fun(self: Monster, idle: boolean)
---*int LuaScriptInterface::luaMonsterIsTarget(lua_State* L)
---@source ../src/luascript.cpp:11436
---@field isTarget fun(self: Monster, creature: Creature): boolean
---*int LuaScriptInterface::luaMonsterIsOpponent(lua_State* L)
---@source ../src/luascript.cpp:114555
---@field isOpponent fun(self: Monster, creature: Creature): boolean
---*int LuaScriptInterface::luaMonsterIsFriend(lua_State* L)
---@source ../src/luascript.cpp:11474
---@field isFriend fun(self: Monster, creature: Creature): boolean
---*int LuaScriptInterface::luaMonsterAddFriend(lua_State* L)
---@source ../src/luascript.cpp:11493
---@field addFriend fun(self: Monster, creature: Creature)
---*int LuaScriptInterface::luaMonsterRemoveFriend(lua_State* L)
---@source ../src/luascript.cpp:11513
---@field removeFriend fun(self: Monster, creature: Creature)
---*int LuaScriptInterface::luaMonsterGetFriendList(lua_State* L)
---@source ../src/luascript.cpp:11533
---@field getFriendList fun(self: Monster): table<number, Creature>
---*int LuaScriptInterface::luaMonsterGetFriendCount(lua_State* L)
---@source ../src/luascript.cpp:11554
---@field getFriendCount fun(self: Monster): number
---*int LuaScriptInterface::luaMonsterAddTarget(lua_State* L)
---@source ../src/luascript.cpp:11566
---@field addTarget fun(self: Monster, creature: Creature, pushFront?: boolean): boolean? -- Returns nil if no monster, false if no creature to target, true if success
---*int LuaScriptInterface::luaMonsterRemoveTarget(lua_State* L)
---@source ../src/luascript.cpp:11588
---@field removeTarget fun(self: Monster, creature: Creature)
---*int LuaScriptInterface::luaMonsterGetTargetList(lua_State* L)
---@source ../src/luascript.cpp:11609
---@field getTargetList fun(self: Monster): table<number, Creature>
---*int LuaScriptInterface::luaMonsterGetTargetCount(lua_State* L)
---@source ../src/luascript.cpp:11630
---@field getTargetCount fun(self: Monster): number
---*int LuaScriptInterface::luaMonsterSelectTarget(lua_State* L)
---@source ../src/luascript.cpp:11642
---@field selectTarget fun(self: Monster, creature: Creature): boolean? -- Returns nil if no monster, false if no creature to target, true/false if success
---*int LuaScriptInterface::luaMonsterSearchTarget(lua_State* L)
---@source ../src/luascript.cpp:11661
---@field searchTarget fun(self: Monster, searchType: TargetSearchType): boolean
---*int LuaScriptInterface::luaMonsterIsWalkingToSpawn(lua_State* L)
---@source ../src/luascript.cpp:11674
---@field isWalkingToSpawn fun(self: Monster): boolean
---*int LuaScriptInterface::luaMonsterWalkToSpawn(lua_State* L)
---@source ../src/luascript.cpp:11686
---@field walkToSpawn fun(self: Monster): boolean
---*int LuaScriptInterface::luaMonsterHasIcon(lua_State* L)
---@source ../src/luascript.cpp:11698
---@field hasSpecialIcon fun(self: Monster, iconId: MonsterIcons): boolean
---*int LuaScriptInterface::luaMonsterSetIcon(lua_State* L)
---@source ../src/luascript.cpp:11710
---@field setSpecialIcon fun(self: Monster, iconId: MonsterIcons, value: uint16)
---*int LuaScriptInterface::luaMonsterSetIcon(lua_State* L)
---@source ../src/luascript.cpp:11733
---@field getSpecialIcon fun(self: Monster): number -- Returns the value stored with the special icon
---*int LuaScriptInterface::luaMonsterGetIcon(lua_State* L)
---@source ../src/luascript.cpp:11753
---@field removeSpecialIcon fun(self: Monster, iconId: MonsterIcons)
---@source ../src/monster.h
Monster = {}

--- The Npc class, extending Creature, is used to represent NPCs (Non-Player Characters) in the game.
--- <br>
--- NPCs are a special type of creature that players can interact with.
--- <br>
--- Each one is scriptable through lua found in the `data/npc/scripts` folder.
--- <br>
--- Once scripts for the NPC is finished, they must be defined in an .xml file in `data/npc` folder.
--- <br>
--- Some basic ones are found by default in `data/npc`. 
--- <br><br>
--- Example Usage:
--- ```lua
--- local position = Position(100, 100, 7) -- Creates a position object with the coordinates (100, 100, 7)
--- local npc = Game.createNpc("Test", position) -- Creates a NPC that has the name "Test" at position (100, 100, 7)
--- print("NPC ID: " .. npc:getId()) -- Prints the ID of the NPC
--- print("Is NPC?: " .. npc:isNpc()) -- Prints true
--- for i, spectator in ipairs(npc:getSpectators()) do
---  print("Spectator ID: " .. spectator:getId()) -- Prints the ID of each spectator (players) that the NPC can see
--- end
--- ```
--- Further definitions for NPCs can be found in;
--- <ol>
--- <li> npc.h </li>
--- <li> data/npc/lib </li>
--- </ol>
---@source ../src/npc.h
---@class Npc : Creature
---*int LuaScriptInterface::luaNpcCreate(lua_State* L)
---@source ../src/luascript.cpp:11766
---@field create fun(npcInfo: uint32|string|Npc): Npc
---@field __eq fun(self: Npc, other: Npc): boolean
---*int LuaScriptInterface::luaNpcIsNpc(lua_State* L)
---@source ../src/luascript.cpp:11807
---@field isNpc fun(self: Npc): boolean
---*int LuaScriptInterface::luaNpcSetMasterPos(lua_State* L)
---@source ../src/luascript.cpp:11814
---@field setMasterPos fun(self: Npc, position: Position, radius?: int32)
---*int LuaScriptInterface::luaNpcGetSpeechBubble(lua_State* L)
---@source ../src/luascript.cpp:11830
---@field getSpeechBubble fun(self: Npc): uint8
---*int LuaScriptInterface::luaNpcSetSpeechBubble(lua_State* L)
---@source ../src/luascript.cpp:11842
---@field setSpeechBubble fun(self: Npc, bubbleType: uint8)
---*int LuaScriptInterface::luaNpcGetSpectators(lua_State* L)
---@source ../src/luascript.cpp:11869
---@field getSpectators fun(self: Npc): table<number, Player> -- Returns a table of all players the npc can see
---@source ../src/npc.h
Npc = {}

--- The guild class is used to represent a guild in the game.
--- <br>
--- Guilds are a group of players that join together to do things. 
--- <br>
--- Each guild gets a guild channel that can only have guild members in it.
--- <br><br>
--- Example Usage
--- ```lua
--- local player = Player(playerId) -- Getting a player
--- local guild = player:getGuild() -- Getting the guild of the player
--- if not guild then
---  print("Player is not in a guild.") -- Player is not in a guild
--- end
--- print(guild:getName()) -- Prints the name of the guild
--- print(guild:getId()) -- Prints the ID of the guild
--- guild:setMotd("New message of the day") -- Sets the message of the day for the guild
--- ```
--- Further definitions of the guild class can be found in `guild.h`.
---@source ../src/guild.h
---@class Guild
---*int LuaScriptInterface::luaGuildCreate(lua_State* L)
---@source ../src/luascript.cpp:11891
---@field create fun(): Guild
---@field __eq fun(self: Guild, other: Guild): boolean
---*int LuaScriptInterface::luaGuildGetId(lua_State* L)
---@source ../src/luascript.cpp:11905
---@field getId fun(self: Guild): uint32
---*int LuaScriptInterface::luaGuildGetName(lua_State* L)
---@source ../src/luascript.cpp:11916
---@field getName fun(self: Guild): string
---*int LuaScriptInterface::luaGuildGetMembersOnline(lua_State* L)
---@source ../src/luascript.cpp:11927
---@field getMembersOnline fun(self: Guild): number
---*int LuaScriptInterface::luaGuildAddRank(lua_State* L)
---@source ../src/luascript.cpp:11948
---@field addRank fun(self: Guild, rankId: uint32, rankName: string, rankLevel: uint8)
---*int LuaScriptInterface::luaGuildGetRankById(lua_State* L)
---@source ../src/luascript.cpp:11963
---@field getRankById fun(self: Guild, rankId: uint32): GuildRankInfoTable -- Returns a table with the rank ID, name and level
---*int LuaScriptInterface::luaGuildGetRankByLevel(lua_State* L)
---@source ../src/luascript.cpp:11984
---@field getRankByLevel fun(self: Guild, level: uint8): GuildRankInfoTable -- Returns a table with the rank ID, name and level
---*int LuaScriptInterface::luaGuildGetMotd(lua_State* L)
---@source ../src/luascript.cpp:12005
---@field getMotd fun(self: Guild): string
---*int LuaScriptInterface::luaGuildSetMotd(lua_State* L)
---@source ../src/luascript.cpp:12016
---@field setMotd fun(self: Guild, motd: string)
Guild = {}

--- The group class defines the access that a player has 
--- <br>
--- Groups are defined in `data/xml/groups.xml`.
--- <br>
--- Each group has a set of flags that define what the player has access to, such as different commands. They also have indivudal max depot items and vip entries.
--- <br><br>
--- Example Usage:
--- ```lua
--- local player = Player(playerId) -- Getting a player
--- if not player:getGroup():getAccess() then
---  return false -- Player cannot access this command/something else
--- end
--- player:teleportTo(Position(100, 100, 7)) -- Teleports the player to position (100, 100, 7)
--- ```
---@source ../src/groups.h
---@class Group
---*int LuaScriptInterface::luaGroupCreate(lua_State* L)
---@source ../src/luascript.cpp:12030
---@field create fun(groupId: uint32): Group
---@field __eq fun(self: Group, other: Group): boolean
---*int LuaScriptInterface::luaGroupGetId(lua_State* L)
---@source ../src/luascript.cpp:12045
---@field getId fun(self: Group): uint32
---*int LuaScriptInterface::luaGroupGetName(lua_State* L)
---@source ../src/luascript.cpp:12057
---@field getName fun(self: Group): string
---*int LuaScriptInterface::luaGroupGetFlags(lua_State* L)
---@source ../src/luascript.cpp:12069
---@field getFlags fun(self: Group): uint64
---*int LuaScriptInterface::luaGroupGetAccess(lua_State* L)
---@source ../src/luascript.cpp:12081
---@field getAccess fun(self: Group): boolean
---*int LuaScriptInterface::luaGroupGetMaxDepotItems(lua_State* L)
---@source ../src/luascript.cpp:12093
---@field getMaxDepotItems fun(self: Group): uint32
---*int LuaScriptInterface::luaGroupGetMaxVipEntries(lua_State* L)
---@source ../src/luascript.cpp:12105
---@field getMaxVipEntries fun(self: Group): number
---*int LuaScriptInterface::luaGroupHasFlag(lua_State* L)
---@source ../src/luascript.cpp:12117
---@field hasFlag fun(self: Group, flag: PlayerFlags): boolean
---@source ../src/groups.h
Group = {}

---Vocation is a userdata that will contain all the information about a vocation
---<br>
---You can get vocation by using Player:getVocation(), then start calling functions in this class such as
---<br>
---`getId`, `getName`, `getDescription`, etc.
---<br><br>
---Example Usage:
---```lua
---local player = Player(playerId)
---local vocation = player:getVocation()
---print(vocation:getId()) -- Prints the vocation ID of the player
---print(vocation:getName()) -- Prints the vocation name of the player
---print(vocation:getDescription()) -- Prints the vocation description of the player
---```
---All information about a vocation should be defined in `vocations.xml` file in the server data folder.
---@source ../src/vocation.h
---@class Vocation
---*int LuaScriptInterface::luaVocationCreate(lua_State* L)
---@source ../src/luascript.cpp:12131
---@field create fun(vocationInfo: uint32|string): Vocation
---@field __eq fun(self: Vocation, other: Vocation): boolean
---*int LuaScriptInterface::luaVocationGetId(lua_State* L)
---@source ../src/luascript.cpp:12151
---@field getId fun(self: Vocation): uint16
---*int LuaScriptInterface::luaVocationGetClientId(lua_State* L)
---@source ../src/luascript.cpp:12163
---@field getClientId fun(self: Vocation): uint8
---*int LuaScriptInterface::luaVocationGetName(lua_State* L)
---@source ../src/luascript.cpp:12175
---@field getName fun(self: Vocation): string
---*int LuaScriptInterface::luaVocationGetDescription(lua_State* L)
---@source ../src/luascript.cpp:12187
---@field getDescription fun(self: Vocation): string
---*int LuaScriptInterface::luaVocationGetRequiredSkillTries(lua_State* L)
---@source ../src/luascript.cpp:12199
---@field getRequiredSkillTries fun(self: Vocation, skillType: SkillTypes, skillLevel: uint16): uint64
---*int LuaScriptInterface::luaVocationGetRequiredManaSpent(lua_State* L)
---@source ../src/luascript.cpp:12213
---@field getRequiredManaSpent fun(self: Vocation, magicLevel: uint32): uint64
---*int LuaScriptInterface::luaVocationGetCapacityGain(lua_State* L)
---@source ../src/luascript.cpp:12226
---@field getCapacityGain fun(self: Vocation): uint32
---*int LuaScriptInterface::luaVocationGetHealthGain(lua_State* L)
---@source ../src/luascript.cpp:12238
---@field getHealthGain fun(self: Vocation): uint32
---*int LuaScriptInterface::luaVocationGetHealthGainTicks(lua_State* L)
---@source ../src/luascript.cpp:12250
---@field getHealthGainTicks fun(self: Vocation): uint32
---*int LuaScriptInterface::luaVocationGetHealthGainAmount(lua_State* L)
---@source ../src/luascript.cpp:12262
---@field getHealthGainAmount fun(self: Vocation): uint32
---*int LuaScriptInterface::luaVocationGetManaGain(lua_State* L)
---@source ../src/luascript.cpp:12274
---@field getManaGain fun(self: Vocation): uint32
---*int LuaScriptInterface::luaVocationGetManaGainTicks(lua_State* L)
---@source ../src/luascript.cpp:12286
---@field getManaGainTicks fun(self: Vocation): uint32
---*int LuaScriptInterface::luaVocationGetManaGainAmount(lua_State* L)
---@source ../src/luascript.cpp:12298
---@field getManaGainAmount fun(self: Vocation): uint32
---*int LuaScriptInterface::luaVocationGetMaxSoul(lua_State* L)
---@source ../src/luascript.cpp:12310
---@field getMaxSoul fun(self: Vocation): uint8
---*int LuaScriptInterface::luaVocationGetSoulGainTicks(lua_State* L)
---@source ../src/luascript.cpp:12322
---@field getSoulGainTicks fun(self: Vocation): uint16
---*int LuaScriptInterface::luaVocationGetAttackSpeed(lua_State* L)
---@source ../src/luascript.cpp:12334
---@field getAttackSpeed fun(self: Vocation): uint32
---*int LuaScriptInterface::luaVocationGetBaseSpeed(lua_State* L)
---@source ../src/luascript.cpp:12346
---@field getBaseSpeed fun(self: Vocation): uint32
---*int LuaScriptInterface::luaVocationGetDemotion(lua_State* L)
---@source ../src/luascript.cpp:12358
---@field getDemotion fun(self: Vocation): Vocation
---*int LuaScriptInterface::luaVocationGetPromotion(lua_State* L)
---@source ../src/luascript.cpp:12383
---@field getPromotion fun(self: Vocation): Vocation
---*int LuaScriptInterface::luaVocationAllowsPvp(lua_State* L)
---@source ../src/luascript.cpp:12408
---@field allowsPvp fun(self: Vocation): boolean
Vocation = {}

--- The house class is used to get information about a house in the game.
--- <br>
--- All houses are defined in `MAPNAME-house.xml` file in the `data/world` folder.
--- <br>
--- Each house has a set of information such as the name, town, rent, owner, etc.
--- <br><br>
--- Example Usage:
--- ```lua
--- local player = Player(playerId) -- Getting a player
--- local house = player:getHouse() -- Getting the house of the player
--- if not house then
---     print("player does not have a house")
---     return false
--- end
--- print("House ID: " .. house:getId()) -- Prints the ID of the house
--- print("House name: " .. house:getName()) -- Prints the name of the house
--- print("House town: " .. house:getTown():getName()) -- Prints the name of the town of the house
--- ```
--- Further definitions of the house class can be found in `house.h`.
--- <br>
---@source ../src/house.h
---@class House
---*int LuaScriptInterface::luaHouseCreate(lua_State* L)
---@source ../src/luascript.cpp:12421
---@field create fun(houseId: uint32): House
---@field __eq fun(self: House, other: House): boolean
---*int LuaScriptInterface::luaHouseGetId(lua_State* L)
---@source ../src/luascript.cpp:12434
---@field getId fun(self: House): uint32
---*int LuaScriptInterface::luaHouseGetName(lua_State* L)
---@source ../src/luascript.cpp:12446
---@field getName fun(self: House): string
---*int LuaScriptInterface::luaHouseGetTown(lua_State* L)
---@source ../src/luascript.cpp:12458
---@field getTown fun(self: House): Town
---*int LuaScriptInterface::luaHouseGetExitPosition(lua_State* L)
---@source ../src/luascript.cpp:12476
---@field getExitPosition fun(self: House): Position
---*int LuaScriptInterface::luaHouseGetRent(lua_State* L)
---@source ../src/luascript.cpp:12488
---@field getRent fun(self: House): uint32
---*int LuaScriptInterface::luaHouseSetRent(lua_State* L)
---@source ../src/luascript.cpp:12500
---@field setRent fun(self: House, rent: uint32)
---*int LuaScriptInterface::luaHouseGetPaidUntil(lua_State* L)
---@source ../src/luascript.cpp:12514
---@field getPaidUntil fun(self: House): number -- Returns a standard unix timestamp of when the rent is due
---*int LuaScriptInterface::luaHouseSetPaidUntil(lua_State* L)
---@source ../src/luascript.cpp:12526
---@field setPaidUntil fun(self: House, timestamp: number)
---*int LuaScriptInterface::luaHouseGetPayRentWarnings(lua_State* L)
---@source ../src/luascript.cpp:12538
---@field getPayRentWarnings fun(self: House): uint32
---*int LuaScriptInterface::luaHouseSetPayRentWarnings(lua_State* L)
---@source ../src/luascript.cpp:12552
---@field setPayRentWarnings fun(self: House, warnings: uint32)
---*int LuaScriptInterface::luaHouseGetOwnerName(lua_State* L)
---@source ../src/luascript.cpp:12566
---@field getOwnerName fun(self: House): string
---*int LuaScriptInterface::luaHouseGetOwnerGuid(lua_State* L)
---@source ../src/luascript.cpp:12578
---@field getOwnerGuid fun(self: House): uint32
---*int LuaScriptInterface::luaHouseSetOwnerGuid(lua_State* L)
---@source ../src/luascript.cpp:12590
---@field setOwnerGuid fun(self: House, guid: uint32, updateDatabase?: boolean)
---*int LuaScriptInterface::luaHouseStartTrade(lua_State* L)
---@source ../src/luascript.cpp:12605
---@field startTrade fun(self: House, houseOwner: Player, tradePartner: Player): ReturnValues -- Will return a number depending on the return message in the function (ex; RETURNVALUE_TRADEPLAYERFARAWAY, RETURNVALUE_TRADEPLAYERALREADYOWNSAHOUSE, etc.) See int LuaScriptInterface::luaHouseStartTrade(lua_State* L) for cases to handle
---*int LuaScriptInterface::luaHouseGetBeds(lua_State* L)
---@source ../src/luascript.cpp:12652
---@field getBeds fun(self: House): table<number, Item> -- Returns a table of all bed items
---*int LuaScriptInterface::luaHouseGetBedCount(lua_State* L)
---@source ../src/luascript.cpp:12673
---@field getBedCount fun(self: House): uint32
---*int LuaScriptInterface::luaHouseGetDoors(lua_State* L)
---@source ../src/luascript.cpp:12685
---@field getDoors fun(self: House): table<number, Item> -- Returns a table of all door items
---*int LuaScriptInterface::luaHouseGetDoorCount(lua_State* L)
---@source ../src/luascript.cpp:12697
---@field getDoorCount fun(self: House): number
---*int LuaScriptInterface::luaHouseGetDoorIdByPosition(lua_State* L)
---@source ../src/luascript.cpp:12709
---@field getDoorIdByPosition fun(self: House, position: Position): uint32
---*int LuaScriptInterface::luaHouseGetTiles(lua_State* L)
---@source ../src/luascript.cpp:12736
---@field getTiles fun(self: House): table<number, Item> -- Returns a table of all tiles
---*int LuaScriptInterface::luaHouseGetItems(lua_State* L)
---@source ../src/luascript.cpp:12757
---@field getItems fun(self: House): table<number, Item> -- Returns a table of all items placed in the house
---*int LuaScriptInterface::luaHouseGetTileCount(lua_State* L)
---@source ../src/luascript.cpp:12783
---@field getTileCount fun(self: House): number
---*int LuaScriptInterface::luaHouseCanEditAccessList(lua_State* L)
---@source ../src/luascript.cpp:12795
---@field canEditAccessList fun(self: House, listId: uint32, player: Player): boolean
---*int LuaScriptInterface::luaHouseGetAccessList(lua_State* L)
---@source ../src/luascript.cpp:12811
---@field getAccessList fun(self: House, listId: number): string
---*int LuaScriptInterface::luaHouseSetAccessList(lua_State* L)
---@source ../src/luascript.cpp:12830
---@field setAccessList fun(self: House, listId: number, list: string)
---*int LuaScriptInterface::luaHouseKickPlayer(lua_State* L)
---@source ../src/luascript.cpp:12848
---@field kickPlayer fun(self: House, player: Player, targetPlayer: Player): boolean -- Returns true if the player was kicked, false if not
---*int LuaScriptInterface::luaHouseSave(lua_State* L)
---@source ../src/luascript.cpp:12859
---@field save fun(self: House) -- Will save the house to the database
---@source ../src/house.h
House = {}

--- The ItemType class is used to get information about an item's base attributes in the game.
--- <br>
--- When loading the server, all items will be loaded from items.xml and items.otb files. Every item will be stored as an "ItemType" class.
--- <br>
--- When an item is created, it will take all the information from the ItemType class and apply it to the item. 
--- <br><br>
--- *Note*: All modifications to the item shall be done through the Item class. ItemType has no setters, only getters.
--- <br>
--- When using functions in the Item class, there are some cases where it has an optional "total" parameter, such as `getBoostPercent()`. 
--- When set to true, it will return the total amount of boost percent that is applied to the item, as well as how much would be from the ItemType.
--- <br><br>
--- Example Usage:
--- ```lua
--- local itemType = ItemType(2160) -- Creates an ItemType object with the ID 2160 (crystal coin)
--- print("ItemType isStackable: " .. itemType:isStackable()) -- Prints true
--- print("ItemType isMovable: " .. itemType:isMovable()) -- Prints true
--- print("ItemType isReadable: " .. itemType:isReadable()) -- Prints false
--- 
--- local itemType2 = ItemType(2400) -- Creates an ItemType object with the ID 2400 (magic sword)
--- print("ItemType2 isStackable: " .. itemType2:isStackable()) -- Prints false
--- print("ItemType2 attack: " .. itemType2:getAttack()) -- Prints 48
--- ```
--- Further definitions of the ItemType class can be found in `items.h`.
---@source ../src/items.h
---@class ItemType
---*int LuaScriptInterface::luaItemTypeCreate(lua_State* L)
---@source ../src/luascript.cpp:12873
---@field create fun(itemTypeInfo: uint32|string): ItemType
---@field __eq fun(self: ItemType, other: ItemType): boolean
---*int LuaScriptInterface::luaItemTypeIsCorpse(lua_State* L)
---@source ../src/luascript.cpp:12892
---@field isCorpse fun(self: ItemType): boolean
---*int LuaScriptInterface::luaItemTypeIsDoor(lua_State* L)
---@source ../src/luascript.cpp:12904
---@field isDoor fun(self: ItemType): boolean
---*int LuaScriptInterface::luaItemTypeIsContainer(lua_State* L)
---@source ../src/luascript.cpp:12916
---@field isContainer fun(self: ItemType): boolean
---*int LuaScriptInterface::luaItemTypeIsFluidContainer(lua_State* L)
---@source ../src/luascript.cpp:12928
---@field isFluidContainer fun(self: ItemType): boolean
---*int LuaScriptInterface::luaItemTypeIsMovable(lua_State* L)
---@source ../src/luascript.cpp:12940
---@field isMovable fun(self: ItemType): boolean
---*int LuaScriptInterface::luaItemTypeIsRune(lua_State* L)
---@source ../src/luascript.cpp:12952
---@field isRune fun(self: ItemType): boolean
---*int LuaScriptInterface::luaItemTypeIsStackable(lua_State* L)
---@source ../src/luascript.cpp:12964
---@field isStackable fun(self: ItemType): boolean
---*int LuaScriptInterface::luaItemTypeIsReadable(lua_State* L)
---@source ../src/luascript.cpp:12976
---@field isReadable fun(self: ItemType): boolean
---*int LuaScriptInterface::luaItemTypeIsWritable(lua_State* L)
---@source ../src/luascript.cpp:12988
---@field isWritable fun(self: ItemType): boolean
---*int LuaScriptInterface::luaItemTypeIsBlocking(lua_State* L)
---@source ../src/luascript.cpp:13000
---@field isBlocking fun(self: ItemType): boolean
---*int LuaScriptInterface::luaItemTypeIsGroundTile(lua_State* L)
---@source ../src/luascript.cpp:13012
---@field isGroundTile fun(self: ItemType): boolean
---*int LuaScriptInterface::luaItemTypeIsMagicField(lua_State* L)
---@source ../src/luascript.cpp:13024
---@field isMagicField fun(self: ItemType): boolean
---*int LuaScriptInterface::luaItemTypeIsUseable(lua_State* L)
---@source ../src/luascript.cpp:13036
---@field isUseable fun(self: ItemType): boolean
---*int LuaScriptInterface::luaItemTypeIsPickupable(lua_State* L)
---@source ../src/luascript.cpp:13048
---@field isPickupable fun(self: ItemType): boolean
---*int LuaScriptInterface::luaItemTypeIsRotatable(lua_State* L)
---@source ../src/luascript.cpp:13060
---@field isRotatable fun(self: ItemType): boolean
---*int LuaScriptInterface::luaItemTypeGetType(lua_State* L)
---@source ../src/luascript.cpp:13072
---@field getType fun(self: ItemType): ItemTypes
---*int LuaScriptInterface::luaItemTypeGetGroup(lua_State* L)
---@source ../src/luascript.cpp:13084
---@field getGroup fun(self: ItemType): ItemGroups
---*int LuaScriptInterface::luaItemTypeGetId(lua_State* L)
---@source ../src/luascript.cpp:13096
---@field getId fun(self: ItemType): uint16 -- Returns the server ID of the item
---*int LuaScriptInterface::luaItemTypeGetClientId(lua_State* L)
---@source ../src/luascript.cpp:13108
---@field getClientId fun(self: ItemType): uint16
---*int LuaScriptInterface::luaItemTypeGetName(lua_State* L)
---@source ../src/luascript.cpp:13120
---@field getName fun(self: ItemType): string
---*int LuaScriptInterface::luaItemTypeGetPluralName(lua_State* L)
---@source ../src/luascript.cpp:13132
---@field getPluralName fun(self: ItemType): string
---*int LuaScriptInterface::luaItemTypeGetRotateTo(lua_State* L)
---@source ../src/luascript.cpp:13144
---@field getRotateTo fun(self: ItemType): uint16 -- Returns the server ID of the item that it will rotate to
---*int LuaScriptInterface::luaItemTypeGetArticle(lua_State* L)
---@source ../src/luascript.cpp:13156
---@field getArticle fun(self: ItemType): string
---*int LuaScriptInterface::luaItemTypeGetDescription(lua_State* L)
---@source ../src/luascript.cpp:13168
---@field getDescription fun(self: ItemType): string
---*int LuaScriptInterface::luaItemTypeGetSlotPosition(lua_State* L)
---@source ../src/luascript.cpp:13180
---@field getSlotPosition fun(self: ItemType): SlotPositions
---*int LuaScriptInterface::luaItemTypeGetCharges(lua_State* L)
---@source ../src/luascript.cpp:13192
---@field getCharges fun(self: ItemType): uint32
---*int LuaScriptInterface::luaItemTypeGetFluidSource(lua_State* L)
---@source ../src/luascript.cpp:13204
---@field getFluidSource fun(self: ItemType): FluidTypes
---*int LuaScriptInterface::luaItemTypeGetCapacity(lua_State* L)
---@source ../src/luascript.cpp:13216
---@field getCapacity fun(self: ItemType): uint16
---*int LuaScriptInterface::luaItemTypeGetWeight(lua_State* L)
---@source ../src/luascript.cpp:13228
---@field getWeight fun(self: ItemType, count?: uint16): uint64
---*int LuaScriptInterface::luaItemTypeGetWorth(lua_State* L)
---@source ../src/luascript.cpp:13244
---@field getWorth fun(self: ItemType): uint64
---*int LuaScriptInterface::luaItemTypeGetHitChance(lua_State* L)
---@source ../src/luascript.cpp:13257
---@field getHitChance fun(self: ItemType): int8
---*int LuaScriptInterface::luaItemTypeGetShootRange(lua_State* L)
---@source ../src/luascript.cpp:13269
---@field getShootRange fun(self: ItemType): uint8
---*int LuaScriptInterface::luaItemTypeGetAttack(lua_State* L)
---@source ../src/luascript.cpp:13281
---@field getAttack fun(self: ItemType): int32
---*int LuaScriptInterface::luaItemTypeGetAttackSpeed(lua_State* L)
---@source ../src/luascript.cpp:13293
---@field getAttackSpeed fun(self: ItemType): uint32
---*int LuaScriptInterface::luaItemTypeGetDefense(lua_State* L)
---@source ../src/luascript.cpp:13305
---@field getDefense fun(self: ItemType): int32
---*int LuaScriptInterface::luaItemTypeGetExtraDefense(lua_State* L)
---@source ../src/luascript.cpp:13317
---@field getExtraDefense fun(self: ItemType): int32
---*int LuaScriptInterface::luaItemTypeGetArmor(lua_State* L)
---@source ../src/luascript.cpp:13329
---@field getArmor fun(self: ItemType): int32
---*int LuaScriptInterface::luaItemTypeGetWeaponType(lua_State* L)
---@source ../src/luascript.cpp:13341
---@field getWeaponType fun(self: ItemType): WeaponType
---*int LuaScriptInterface::luaItemTypeGetAmmoType(lua_State* L)
---@source ../src/luascript.cpp:13353
---@field getAmmoType fun(self: ItemType): AmmoTypes
---*int LuaScriptInterface::luaItemTypeGetCorpseType(lua_State* L)
---@source ../src/luascript.cpp:13365
---@field getCorpseType fun(self: ItemType): RaceTypes
---*int LuaScriptInterface::luaItemTypeGetClassification(lua_State* L)
---@source ../src/luascript.cpp:13390
---@field getClassification fun(self: ItemType): uint8
---*int LuaScriptInterface::luaItemTypeGetAbilities(lua_State* L)
---@source ../src/luascript.cpp:13389
---@field getAbilities fun(self: ItemType): ItemAbilities
---*int LuaScriptInterface::luaItemTypeHasShowAttributes(lua_State* L)
---@source ../src/luascript.cpp:13496
---@field hasShowAttributes fun(self: ItemType): boolean
---*int LuaScriptInterface::luaItemTypeHasShowCount(lua_State* L)
---@source ../src/luascript.cpp:13508
---@field hasShowCount fun(self: ItemType): boolean
---*int LuaScriptInterface::luaItemTypeHasShowCharges(lua_State* L)
---@source ../src/luascript.cpp:13520
---@field hasShowCharges fun(self: ItemType): boolean
---*int LuaScriptInterface::luaItemTypeHasShowDuration(lua_State* L)
---@source ../src/luascript.cpp:13532
---@field hasShowDuration fun(self: ItemType): boolean
---*int LuaScriptInterface::luaItemTypeHasAllowDistRead(lua_State* L)
---@source ../src/luascript.cpp:13544
---@field hasAllowDistRead fun(self: ItemType): boolean
---*int LuaScriptInterface::luaItemTypeGetWieldInfo(lua_State* L)
---@source ../src/luascript.cpp:13556
---@field getWieldInfo fun(self: ItemType): string
---*int LuaScriptInterface::luaItemTypeGetDurationMin(lua_State* L)
---@source ../src/luascript.cpp:13568
---@field getDurationMin fun(self: ItemType): uint32
---*int LuaScriptInterface::luaItemTypeGetDurationMax(lua_State* L)
---@source ../src/luascript.cpp:13580
---@field getDurationMax fun(self: ItemType): uint32
---*int LuaScriptInterface::luaItemTypeGetLevelDoor(lua_State* L)
---@source ../src/luascript.cpp:13592
---@field getLevelDoor fun(self: ItemType): uint32
---*int LuaScriptInterface::luaItemTypeGetRuneSpellName(lua_State* L)
---@source ../src/luascript.cpp:13604
---@field getRuneSpellName fun(self: ItemType): string
---*int LuaScriptInterface::luaItemTypeGetVocationString(lua_State* L)
---@source ../src/luascript.cpp:13616
---@field getVocationString fun(self: ItemType): string
---*int LuaScriptInterface::luaItemTypeGetMinReqLevel(lua_State* L)
---@source ../src/luascript.cpp:13628
---@field getMinReqLevel fun(self: ItemType): uint32
---*int LuaScriptInterface::luaItemTypeGetMinReqMagicLevel(lua_State* L)
---@source ../src/luascript.cpp:13640
---@field getMinReqMagicLevel fun(self: ItemType): uint32
---*int LuaScriptInterface::luaItemTypeGetMarketBuyStatistics(lua_State* L)
---@source ../src/luascript.cpp:13652
---@field getMarketBuyStatistics fun(self: ItemType): table
---*int LuaScriptInterface::luaItemTypeGetMarketSellStatistics(lua_State* L)
---@source ../src/luascript.cpp:13673
---@field getMarketSellStatistics fun(self: ItemType): table
---*int LuaScriptInterface::luaItemTypeGetElementType(lua_State* L)
---@source ../src/luascript.cpp:13694
---@field getElementType fun(self: ItemType): CombatTypes
---*int LuaScriptInterface::luaItemTypeGetElementDamage(lua_State* L)
---@source ../src/luascript.cpp:13712
---@field getElementDamage fun(self: ItemType): uint16
---*int LuaScriptInterface::luaItemTypeGetTransformEquipId(lua_State* L)
---@source ../src/luascript.cpp:13730
---@field getTransformEquipId fun(self: ItemType): uint16
---*int LuaScriptInterface::luaItemTypeGetTransformDeEquipId(lua_State* L)
---@source ../src/luascript.cpp:13742
---@field getTransformDeEquipId fun(self: ItemType): uint16
---*int LuaScriptInterface::luaItemTypeGetDestroyId(lua_State* L)
---@source ../src/luascript.cpp:13754
---@field getDestroyId fun(self: ItemType): uint16
---*int LuaScriptInterface::luaItemTypeGetDecayId(lua_State* L)
---@source ../src/luascript.cpp:13766
---@field getDecayId fun(self: ItemType): uint16
---*int LuaScriptInterface::luaItemTypeGetRequiredLevel(lua_State* L)
---@source ../src/luascript.cpp:13778
---@field getRequiredLevel fun(self: ItemType): uint32
---*int LuaScriptInterface::luaItemTypeHasSubType(lua_State* L)
---@source ../src/luascript.cpp:13790
---@field hasSubType fun(self: ItemType): boolean
---*int LuaScriptInterface::luaItemTypeIsStoreItem(lua_State* L)
---@source ../src/luascript.cpp:13802
---@field isStoreItem fun(self: ItemType): boolean
---@source ../src/items.h
ItemType = {}

--- The Combat class is used to create and manage combat actions in the game.
--- <br>
--- Combats are usually defined in a spell or rune, but can also be defined in other scripts such as walking on a tile and having a player be damaged.
--- <br>
--- A combat is defined through a set of parameters and callbacks.
--- <br><br>
--- Example Usage:
--- ```lua
--- local combat = Combat() -- Creates a new combat object
--- combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_FIREDAMAGE) -- Sets the type of combat to fire damage
--- combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_FIREAREA) -- Sets the magic effect effect to fire area
--- combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_FIRE) -- Sets the animation effect to fire
--- 
--- function onGetFormulaValues(player, level, magicLevel)
---	   return -10, -20
--- end
---
--- combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")
---
--- local spell = Spell(SPELL_INSTANT)
---
--- function spell.onCastSpell(creature, variant)
---	   return combat:execute(creature, variant) -- Executes the combat action
--- end
--- spell:id(169)
--- spell:name("Apprentice's Strike")
--- spell:words("exori min flam")
--- spell:register()
--- ```
--- Further definitions of the Combat class can be found in `combat.h`.
--- <br>
--- Notable reading should be done in `combat.cpp` and `spells.cpp` to understand how combat works
---@class Combat
---*int LuaScriptInterface::luaCombatCreate(lua_State* L)
---@source ../src/luascript.cpp:13816
---@field create fun(): Combat
---@field __eq fun(self: Combat, other: Combat): boolean
---@field __gc fun(self: Combat)
---*int LuaScriptInterface::luaCombatDelete(lua_State* L)
---@source ../src/luascript.cpp:13824
---@field delete fun(self: Combat)
---*int LuaScriptInterface::luaCombatSetParameter(lua_State* L)
---@source ../src/luascript.cpp:13833
---@field setParameter fun(self: Combat, key: CombatTypeParams, value: uint32|boolean)
---*int LuaScriptInterface::luaCombatGetParameter(lua_State* L)
---@source ../src/luascript.cpp:13855
---@field getParameter fun(self: Combat, key: CombatTypeParams): uint32|boolean
---*int LuaScriptInterface::luaCombatSetFormula(lua_State* L)
---@source ../src/luascript.cpp:13875
---@field setFormula fun(self: Combat, formulaType: CombatFormulas, minA: number, minB: number, maxA: number, maxB: number)
---*int LuaScriptInterface::luaCombatSetArea(lua_State* L)
---@source ../src/luascript.cpp:13895
---@field setArea fun(self: Combat, area: uint32) -- Area requires to already be set beforehand by calling `createCombatArea`
---*int LuaScriptInterface::luaCombatAddCondition(lua_State* L)
---@source ../src/luascript.cpp:13923
---@field addCondition fun(self: Combat, condition: Condition)
---*int LuaScriptInterface::luaCombatClearConditions(lua_State* L)
---@source ../src/luascript.cpp:13943
---@field clearConditions fun(self: Combat)
---*int LuaScriptInterface::luaCombatSetCallback(lua_State* L)
---@source ../src/luascript.cpp:13958
---@field setCallback fun(self: Combat, callBackParams: CallbackParams, callback: string)
---*int LuaScriptInterface::luaCombatSetOrigin(lua_State* L)
---@source ../src/luascript.cpp:13985
---@field setOrigin fun(self: Combat, originType: DamageOrigin)
---*int LuaScriptInterface::luaCombatExecute(lua_State* L)
---@source ../src/luascript.cpp:14000
---@field execute fun(self: Combat, caster: Creature, target: Variant)
Combat = {}

--- A condition is a temporary effect that can be applied to a creature.
--- <br>
--- It can either be damaging, or provide a buff to the creature.
--- <br>
--- Conditions are usually defined in a spell or rune, but can also be defined in other scripts such as walking on a tile and having a player be damaged.
--- <br><br>
--- *Note*: Conditions are stored with an ID and a subID. Ensure that when creating a condition, the ID is unique. The subID is used to differentiate between multiple conditions of the same type.
--- <br><br>
--- Example Usage:
--- ```lua
--- local condition = Condition(CONDITION_ATTRIBUTES) -- Creates a new condition object of type CONDITION_ATTRIBUTES
--- condition:setParameter(CONDITION_PARAM_TICKS, 2 * 60 * 1000) -- Sets the duration of the condition to 2 minutes
--- condition:setParameter(CONDITION_PARAM_STAT_MAGICPOINTS, 1) -- Sets the magic points to be added to the player
--- condition:setParameter(CONDITION_PARAM_BUFF_SPELL, true) -- Sets the condition to be a buff spell
--- 
--- local spell = Spell(SPELL_INSTANT)
--- 
--- function spell.onCastSpell(creature, variant)
--- 	return creature:addPartyCondition(combat, variant, condition, 120) -- Adds the condition to the players in the caster's party
--- end
--- -- Rest of the spell registers
--- ```
--- Further definitions of the Condition class can be found in `condition.h`.
---@source ../src/condition.h
---@class Condition
---*int LuaScriptInterface::luaConditionCreate(lua_State* L)
---@source ../src/luascript.cpp:14079
---@field create fun(): Condition
---@field __eq fun(self: Condition, other: Condition): boolean
---@field __gc fun(self: Condition)
---*int LuaScriptInterface::luaConditionGetId(lua_State* L)
---@source ../src/luascript.cpp:14106
---@field getId fun(self: Condition): ConditionIds
---*int LuaScriptInterface::luaConditionGetSubId(lua_State* L)
---@source ../src/luascript.cpp:14118
---@field getSubId fun(self: Condition): uint32
---*int LuaScriptInterface::luaConditionGetType(lua_State* L)
---@source ../src/luascript.cpp:14130
---@field getType fun(self: Condition): ConditionType
---*int LuaScriptInterface::luaConditionGetIcons(lua_State* L)
---@source ../src/luascript.cpp:14142
---@field getIcons fun(self: Condition): Icons
---*int LuaScriptInterface::luaConditionGetEndTime(lua_State* L)
---@source ../src/luascript.cpp:14154
---@field getEndTime fun(self: Condition): int64
---*int LuaScriptInterface::luaConditionClone(lua_State* L)
---@source ../src/luascript.cpp:14166
---@field clone fun(self: Condition): Condition
---*int LuaScriptInterface::luaConditionGetTicks(lua_State* L)
---@source ../src/luascript.cpp:14179
---@field getTicks fun(self: Condition): int32
---*int LuaScriptInterface::luaConditionSetTicks(lua_State* L)
---@source ../src/luascript.cpp:14191
---@field setTicks fun(self: Condition, ticks: int32)
---*int LuaScriptInterface::luaConditionSetParameter(lua_State* L)
---@source ../src/luascript.cpp:14203
---@field setParameter fun(self: Condition, key: ConditionParam, value: boolean|int32)
---*int LuaScriptInterface::luaConditionGetParameter(lua_State* L)
---@source ../src/luascript.cpp:14215
---@field getParameter fun(self: Condition, key: ConditionParam): boolean|int32
---*int LuaScriptInterface::luaConditionSetFormula(lua_State* L)
---@source ../src/luascript.cpp:14245
---@field setFormula fun(self: Condition, minA: double, minB: double, maxA: double, maxB: double) -- Used for HASTE or PARALYZE
---*int LuaScriptInterface::luaConditionSetOutfit(lua_State* L)
---@source ../src/luascript.cpp:14262
---@field setOutfit fun(self: Condition, outfit: Outfit)
---*int LuaScriptInterface::luaConditionSetOutfit(lua_State* L)
---@source ../src/luascript.cpp:14262
---@field setOutfit fun(self: Condition, lookTypeEx: uint16, lookType: uint16, lookHead: uint8, lookBody: uint8, lookLegs: uint8, lookFeet: uint8, lookAddons?: uint8, lookMount?: uint16)
---*int LuaScriptInterface::luaConditionAddDamage(lua_State* L)
---@source ../src/luascript.cpp:14290
---@field addDamage fun(self: Condition, rounds: int32, time: int32, value: int32)
---@source ../src/condition.h
Condition = {}

--- The Outfit class is used to define a creature's outfit in the game.
--- <br>
--- An outfit has;
--- <ol>
--- <li>Looktype: Outfit ID</li>
--- <li>lookHead: Head Colour</li>
--- <li>lookBody: Body Colour</li>
--- <li>lookLegs: Legs Colour</li>
--- <li>lookFeet: Feet Colour</li>
--- <li>lookAddons: How many addons the outfit has equipped</li>
--- <li>lookMount: Mount ID</li>
--- <li>lookMountHead: Mount Head Colour (If applicable)</li>
--- <li>lookMountBody: Mount Body Colour (If applicable)</li>
--- <li>lookMountLegs: Mount Legs Colour (If applicable)</li>
--- <li>lookMountFeet: Mount Feet Colour (If applicable)</li>
--- </ol>
--- <br><br>
--- Example Usage:
--- ```lua
--- local outfit = Outfit(75) -- Creates a new outfit object with the looktype 75 (Gamemaster)
--- outfit:lookHead(0) -- Sets the head colour to 0 
--- outfit:lookBody(0) -- Sets the body colour to 0
--- outfit:lookLegs(0) -- Sets the legs colour to 0
--- outfit:lookFeet(0) -- Sets the feet colour to 0
--- local player = Player(playerId)
--- player:setOutfit(outfit) -- Sets the player's outfit to the new outfit
--- ```
--- Further definitions of the Outfit class can be found in `outfit.h`.
---@source ../src/outfit.h
---@class Outfit
---*int LuaScriptInterface::luaOutfitCreate(lua_State* L)
---@source ../src/luascript.cpp:14306
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
---@source ../src/outfit.h
Outfit = {}

--- The MonsterType class is used to define a monster's base attributes in the game.
--- <br>
--- MonsterTypes are defined in the monster XML/LUA files, and are loaded into the server when it starts.
--- <br>
--- When a monster is created, it will take all the information from the MonsterType class and apply it to the monster.
--- <br><br>
--- *Note*: Most functions in the MonsterType class **combine** the `set` and `get` functions into one. 
--- This means that if you call a function with a parameter, it will set the value. If you call it without a parameter, it will get the value.
--- This is mostly used while registered a monstertype in a revscript monster file.
--- <br><br>
--- Example usage:
--- ```lua
--- local monsterType = MonsterType("Rat") -- Gets the monster type object with the name "Rat"
--- print("MonsterType name: " .. monsterType:name()) -- Prints "Rat"
--- print("MonsterType Max Health: " .. monsterType:maxHealth()) -- Prints the max health of a rat
--- print("MonsterType experience: " .. monsterType:experience()) -- Prints the experience that a player will get when killing a rat
--- ```
--- Further definitions of the MonsterType class can be found in `monsters.h`.
---@source ../src/monsters.h
---@class MonsterType
---*int LuaScriptInterface::luaMonsterTypeCreate(lua_State* L)
---@source ../src/luascript.cpp:14328
---@field create fun(): MonsterType
---@field __eq fun(self: MonsterType, other: MonsterType): boolean?
---*int LuaScriptInterface::luaMonsterTypeIsAttackable(lua_State* L)
---@source ../src/luascript.cpp:14347
---@field isAttackable fun(self: MonsterType, setIsAttackable?: boolean): boolean?
---*int LuaScriptInterface::luaMonsterTypeIsChallengeable(lua_State* L)
---@source ../src/luascript.cpp:14364
---@field isChallengeable fun(self: MonsterType, setIsChallengable?: boolean): boolean?
---*int LuaScriptInterface::luaMonsterTypeIsConvinceable(lua_State* L)
---@source ../src/luascript.cpp:14381
---@field isConvinceable fun(self: MonsterType, setIsConvinceable?: boolean): boolean?
---*int LuaScriptInterface::luaMonsterTypeIsSummonable(lua_State* L)
---@source ../src/luascript.cpp:14398
---@field isSummonable fun(self: MonsterType, setIsSummonable?: boolean): boolean?
---*int LuaScriptInterface::luaMonsterTypeIsIgnoringSpawnBlock(lua_State* L)
---@source ../src/luascript.cpp:14415
---@field isIgnoringSpawnBlock fun(self: MonsterType, setIsIgnoringSpawnBlock?: boolean): boolean?
---*int LuaScriptInterface::luaMonsterTypeIsIllusionable(lua_State* L)
---@source ../src/luascript.cpp:14432
---@field isIllusionable fun(self: MonsterType, setIsIllusionable?: boolean): boolean?
---*int LuaScriptInterface::luaMonsterTypeIsHostile(lua_State* L)
---@source ../src/luascript.cpp:14449
---@field isHostile fun(self: MonsterType, setIsHostile?: boolean): boolean?
---*int LuaScriptInterface::luaMonsterTypeIsPushable(lua_State* L)
---@source ../src/luascript.cpp:14466
---@field isPushable fun(self: MonsterType, setIsPushable?: boolean): boolean?
---*int LuaScriptInterface::luaMonsterTypeIsHealthHidden(lua_State* L)
---@source ../src/luascript.cpp:14483
---@field isHealthHidden fun(self: MonsterType, setIsHealthHidden?: boolean): boolean?
---*int LuaScriptInterface::luaMonsterTypeIsBoss(lua_State* L)
---@source ../src/luascript.cpp:14500
---@field isBoss fun(self: MonsterType, setIsBoss?: boolean): boolean?
---*int LuaScriptInterface::luaMonsterTypeCanPushItems(lua_State* L)
---@source ../src/luascript.cpp:14517
---@field canPushItems fun(self: MonsterType, setCanPushItems?: boolean): boolean?
---*int LuaScriptInterface::luaMonsterTypeCanPushCreatures(lua_State* L)
---@source ../src/luascript.cpp:14534
---@field canPushCreatures fun(self: MonsterType, setCanPushCreatures?: boolean): boolean?
---*int LuaScriptInterface::luaMonsterTypeCanWalkOnEnergy(lua_State* L)
---@source ../src/luascript.cpp:14551
---@field canWalkOnEnergy fun(self: MonsterType, setCanWalkOnEnergy?: boolean): boolean?
---*int LuaScriptInterface::luaMonsterTypeCanWalkOnFire(lua_State* L)
---@source ../src/luascript.cpp:14568
---@field canWalkOnFire fun(self: MonsterType, setCanWalkOnFire?: boolean): boolean?
---*int LuaScriptInterface::luaMonsterTypeCanWalkOnPoison(lua_State* L)
---@source ../src/luascript.cpp:14585
---@field canWalkOnPoison fun(self: MonsterType, setCanWalkOnPoison?: boolean): boolean?
---*int32_t LuaScriptInterface::luaMonsterTypeName(lua_State* L)
---@source ../src/luascript.cpp:14602
---@field name fun(self: MonsterType, setName?: string): string?
---*int LuaScriptInterface::luaMonsterTypeNameDescription(lua_State* L)
---@source ../src/luascript.cpp:14619
---@field nameDescription fun(self: MonsterType, setNameDescription?: string): string?
---*int LuaScriptInterface::luaMonsterTypeHealth(lua_State* L)
---@source ../src/luascript.cpp:14626
---@field health fun(self: MonsterType, setHealth?: int32): int32?
---*int LuaScriptInterface::luaMonsterTypeMaxHealth(lua_State* L)
---@source ../src/luascript.cpp:14653
---@field maxHealth fun(self: MonsterType, setMaxHealth?: int32): int32?
---*int LuaScriptInterface::luaMonsterTypeRunHealth(lua_State* L)
---@source ../src/luascript.cpp:14670
---@field runHealth fun(self: MonsterType, setRunHealth?: int32): int32?
---*int LuaScriptInterface::luaMonsterTypeExperience(lua_State* L)
---@source ../src/luascript.cpp:14687
---@field experience fun(self: MonsterType, setExperience?: uint64): uint64?
---*int LuaScriptInterface::luaMonsterTypeSkull(lua_State* L)
---@source ../src/luascript.cpp:14704
---@field skull fun(self: MonsterType, setSkullNumberOrString?: Skulls|string): Skulls
---*int LuaScriptInterface::luaMonsterTypeCombatImmunities(lua_State* L)
---@source ../src/luascript.cpp:14725
---@field combatImmunities fun(self: MonsterType, setCombatTypeString?: string): number? -- This will return a number which is a combination of all COMBAT_TYPE values the monster is immune to.
---*int LuaScriptInterface::luaMonsterTypeConditionImmunities(lua_State* L)
---@source ../src/luascript.cpp:14776
---@field conditionImmunities fun(self: MonsterType, setConditionTypeString?: string): number?-- This will return a number which is a combination of all ConditionTypes values the monster is immune to.
---*int LuaScriptInterface::luaMonsterTypeGetAttackList(lua_State* L)
---@source ../src/luascript.cpp:14836
---@field getAttackList fun(self: MonsterType): MonsterSpellInfo
---*int LuaScriptInterface::luaMonsterTypeAddAttack(lua_State* L)
---@source ../src/luascript.cpp:14866
---@field addAttack fun(self: MonsterType, attack: table)
---*int LuaScriptInterface::luaMonsterTypeGetDefenseList(lua_State* L)
---@source ../src/luascript.cpp:14889
---@field getDefenseList fun(self: MonsterType): MonsterSpellInfo
---*int LuaScriptInterface::luaMonsterTypeAddDefense(lua_State* L)
---@source ../src/luascript.cpp:14919
---@field addDefense fun(self: MonsterType, defense: table)
---*int LuaScriptInterface::luaMonsterTypeGetElementList(lua_State* L)
---@source ../src/luascript.cpp:14942
---@field getElementList fun(self: MonsterType): table<CombatTypes, int32> -- Returns a table with the index of a CombatType and the value of how much resistance to that damage the monster type has
---*int LuaScriptInterface::luaMonsterTypeAddElement(lua_State* L)
---@source ../src/luascript.cpp:14959
---@field addElement fun(self: MonsterType, element: CombatTypes, value: int32)
---*int LuaScriptInterface::luaMonsterTypeGetVoices(lua_State* L)
---@source ../src/luascript.cpp:14973
---@field getVoices fun(self: MonsterType): table<number, {text: string, yell: boolean}>
---*int LuaScriptInterface::luaMonsterTypeAddVoice(lua_State* L)
---@source ../src/luascript.cpp:14993
---@field addVoice fun(self: MonsterType, sentence: string, interval: uint32, chance: uint32, yell: boolean)
---*int LuaScriptInterface::luaMonsterTypeGetLoot(lua_State* L)
---@source ../src/luascript.cpp:15011
---@field getLoot fun(self: MonsterType): MonsterLootBlock
---*int LuaScriptInterface::luaMonsterTypeAddLoot(lua_State* L)
---@source ../src/luascript.cpp:15024
---@field addLoot fun(self: MonsterType, loot: Loot)
---*int LuaScriptInterface::luaMonsterTypeGetCreatureEvents(lua_State* L)
---@source ../src/luascript.cpp:15042
---@field getCreatureEvents fun(self: MonsterType): table<number, string>
---*int LuaScriptInterface::luaMonsterTypeRegisterEvent(lua_State* L)
---@source ../src/luascript.cpp:15060
---@field registerEvent fun(self: MonsterType, event: string)
---*int LuaScriptInterface::luaMonsterTypeEventType(lua_State* L)
---@source ../src/luascript.cpp:15093
---@field eventType fun(self: MonsterType): string
---*int LuaScriptInterface::luaMonsterTypeEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:15073
---@field onThink fun(self: MonsterType, callback: function)
---*int LuaScriptInterface::luaMonsterTypeEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:15073
---@field onAppear fun(self: MonsterType, callback: function)
---*int LuaScriptInterface::luaMonsterTypeEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:15073
---@field onDisappear fun(self: MonsterType, callback: function)
---*int LuaScriptInterface::luaMonsterTypeEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:15073
---@field onMove fun(self: MonsterType, callback: function)
---*int LuaScriptInterface::luaMonsterTypeEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:15073
---@field onSay fun(self: MonsterType, callback: function)
---*int LuaScriptInterface::luaMonsterTypeGetSummonList(lua_State* L)
---@source ../src/luascript.cpp:15106
---@field getSummonList fun(self: MonsterType): table<number, {name: string, speed: number, chance: number, max: number, effect: number, masterEffect: number}>
---*int LuaScriptInterface::luaMonsterTypeAddSummon(lua_State* L)
---@source ../src/luascript.cpp:15130
---@field addSummon fun(self: MonsterType, name: string, interval: int32, chance: int32, max: int32, effect?: MagicEffectConsts, masterEffect?: MagicEffectConsts)
---*int LuaScriptInterface::luaMonsterTypeMaxSummons(lua_State* L)
---@source ../src/luascript.cpp:15151
---@field maxSummons fun(self: MonsterType, maxSummons?: uint32): uint32?
---*int LuaScriptInterface::luaMonsterTypeArmor(lua_State* L)
---@source ../src/luascript.cpp:15168
---@field armor fun(self: MonsterType, setArmor?: int32): int32?
---*int LuaScriptInterface::luaMonsterTypeDefense(lua_State* L)
---@source ../src/luascript.cpp:15185
---@field defense fun(self: MonsterType, setDfense?: int32): int32?
---*int LuaScriptInterface::luaMonsterTypeOutfit(lua_State* L)
---@source ../src/luascript.cpp:15202
---@field outfit fun(self: MonsterType, setOutfit?: Outfit): Outfit?
---*int LuaScriptInterface::luaMonsterTypeRace(lua_State* L)
---@source ../src/luascript.cpp:15219
---@field race fun(self: MonsterType, setRaceTypeString?: string): RaceTypes?
---*int LuaScriptInterface::luaMonsterTypeCorpseId(lua_State* L)
---@source ../src/luascript.cpp:15251
---@field corpseId fun(self: MonsterType, setCorpseId?: uint16): uint16?
---*int LuaScriptInterface::luaMonsterTypeManaCost(lua_State* L)
---@source ../src/luascript.cpp:15268
---@field manaCost fun(self: MonsterType, setManaCost?: uint32): uint32?
---*int LuaScriptInterface::luaMonsterTypeBaseSpeed(lua_State* L)
---@source ../src/luascript.cpp:15285
---@field baseSpeed fun(self: MonsterType, setBaseSpeed?: uint32): uint32?
---*int LuaScriptInterface::luaMonsterTypeLight(lua_State* L)
---@source ../src/luascript.cpp:15302
---@field light fun(self: MonsterType, setLightColour?: uint8, setLightLevel?: uint8): uint8, uint8 
---*int LuaScriptInterface::luaMonsterTypeStaticAttackChance(lua_State* L)
---@source ../src/luascript.cpp:15322
---@field staticAttackChance fun(self: MonsterType, setStaticAttackChance?: uint32): uint32?
---*int LuaScriptInterface::luaMonsterTypeTargetDistance(lua_State* L)
---@source ../src/luascript.cpp:15339
---@field targetDistance fun(self: MonsterType, setTargetDistance?: int32): int32?
---*int LuaScriptInterface::luaMonsterTypeYellChance(lua_State* L)
---@source ../src/luascript.cpp:15356
---@field yellChance fun(self: MonsterType, setYellChance?: uint32): uint32?
---*int LuaScriptInterface::luaMonsterTypeYellSpeedTicks(lua_State* L)
---@source ../src/luascript.cpp:15373
---@field yellSpeedTicks fun(self: MonsterType, setYellSpeedTicks?: uint32): uint32?
---*int LuaScriptInterface::luaMonsterTypeChangeTargetChance(lua_State* L)
---@source ../src/luascript.cpp:15390
---@field changeTargetChance fun(self: MonsterType, setChangeTargetChance?: int32): int32?
---*int LuaScriptInterface::luaMonsterTypeChangeTargetSpeed(lua_State* L)
---@source ../src/luascript.cpp:15407
---@field changeTargetSpeed fun(self: MonsterType, setChangeTargetSpeed?: uint32): uint32?
---*int LuaScriptInterface::luaMonsterTypeBestiaryInfo(lua_State* L)
---@source ../src/luascript.cpp:15424
---@field bestiaryInfo fun(self: MonsterType, setBestiaryInfo?: MonsterBestiaryInfo): MonsterBestiaryInfo?
---@source ../src/monsters.h
MonsterType = {}

--- The loot class is used to define a loot drop in the game.
--- <br>
--- Loot drops are defined in a monster XML/LUA file, registered when the server starts.
---@class Loot
---*int LuaScriptInterface::luaCreateLoot(lua_State* L)
---@source ../src/luascript.cpp:15478
---@field create fun(): Loot
---@field __gc fun(self: Loot)
---*int LuaScriptInterface::luaDeleteLoot(lua_State* L)
---@source ../src/luascript.cpp:15486
---@field delete fun(self: Loot)
---*int LuaScriptInterface::luaLootSetId(lua_State* L)
---@source ../src/luascript.cpp:15497
---@field setId fun(self: Loot, id: uint16|string)
---*int LuaScriptInterface::luaLootSetSubType(lua_State* L)
---@source ../src/luascript.cpp:15529
---@field setSubType fun(self: Loot, subtype: uint16)
---*int LuaScriptInterface::luaLootSetChance(lua_State* L)
---@source ../src/luascript.cpp:15542
---@field setChance fun(self: Loot, chance: uint32)
---*int LuaScriptInterface::luaLootSetMaxCount(lua_State* L)
---@source ../src/luascript.cpp:15555
---@field setMaxCount fun(self: Loot, count: uint32)
---*int LuaScriptInterface::luaLootSetActionId(lua_State* L)
---@source ../src/luascript.cpp:15568
---@field setActionId fun(self: Loot, actionId: uint32)
---*int LuaScriptInterface::luaLootSetDescription(lua_State* L)
---@source ../src/luascript.cpp:15581
---@field setDescription fun(self: Loot, description: string)
---*int LuaScriptInterface::luaLootAddChildLoot(lua_State* L)
---@source ../src/luascript.cpp:15594
---@field addChildLoot fun(self: Loot, childLoot: Loot)
Loot = {}

--- The MonsterSpell class is used to define a monster's spell
--- <br>
--- MonsterSpells are typically defined in the monster's XML or LUA file, and are loaded into the server when it starts.
--- <br>
---@class MonsterSpell
---*int LuaScriptInterface::luaCreateMonsterSpell(lua_State* L)
---@source ../src/luascript.cpp:15613
---@field create fun(): MonsterSpell
---@field __gc fun(self: MonsterSpell)
---*int LuaScriptInterface::luaDeleteMonsterSpell(lua_State* L)
---@source ../src/luascript.cpp:15621
---@field delete fun(self: MonsterSpell)
---*int LuaScriptInterface::luaMonsterSpellSetType(lua_State* L)
---@source ../src/luascript.cpp:15632
---@field setType fun(self: MonsterSpell, type: string)
---*int LuaScriptInterface::luaMonsterSpellSetScriptName(lua_State* L)
---@source ../src/luascript.cpp:15645
---@field setScriptName fun(self: MonsterSpell, scriptName: string)
---*int LuaScriptInterface::luaMonsterSpellSetChance(lua_State* L)
---@source ../src/luascript.cpp:15658
---@field setChance fun(self: MonsterSpell, chance: uint8)
---*int LuaScriptInterface::luaMonsterSpellSetInterval(lua_State* L)
---@source ../src/luascript.cpp:15671
---@field setInterval fun(self: MonsterSpell, interval: uint16)
---*int LuaScriptInterface::luaMonsterSpellSetRange(lua_State* L)
---@source ../src/luascript.cpp:15684
---@field setRange fun(self: MonsterSpell, range: uint8)
---*int LuaScriptInterface::luaMonsterSpellSetCombatValue(lua_State* L)
---@source ../src/luascript.cpp:15697
---@field setCombatValue fun(self: MonsterSpell, minCombatValue: int32, maxCombatValue: int32)
---*int LuaScriptInterface::luaMonsterSpellSetCombatType(lua_State* L)
---@source ../src/luascript.cpp:15711
---@field setCombatType fun(self: MonsterSpell, type: CombatTypes)
---*int LuaScriptInterface::luaMonsterSpellSetAttackValue(lua_State* L)
---@source ../src/luascript.cpp:15724
---@field setAttackValue fun(self: MonsterSpell, attackValue: int32, skillValue: int32)
---*int LuaScriptInterface::luaMonsterSpellSetNeedTarget(lua_State* L)
---@source ../src/luascript.cpp:15737
---@field setNeedTarget fun(self: MonsterSpell, needTarget: boolean)
---*int LuaScriptInterface::luaMonsterSpellSetNeedDirection(lua_State* L)
---@source ../src/luascript.cpp:15751
---@field setNeedDirection fun(self: MonsterSpell, needDirection: boolean)
---*int LuaScriptInterface::luaMonsterSpellSetCombatLength(lua_State* L)
---@source ../src/luascript.cpp:15764
---@field setCombatLength fun(self: MonsterSpell, length: int32)
---*int LuaScriptInterface::luaMonsterSpellSetCombatSpread(lua_State* L)
---@source ../src/luascript.cpp:15777
---@field setCombatSpread fun(self: MonsterSpell, spread: int32)
---*int LuaScriptInterface::luaMonsterSpellSetCombatRadius(lua_State* L)
---@source ../src/luascript.cpp:15790
---@field setCombatRadius fun(self: MonsterSpell, radius: int32)
---*int LuaScriptInterface::luaMonsterSpellSetCombatRing(lua_State* L)
---@source ../src/luascript.cpp:15803
---@field setCombatRing fun(self: MonsterSpell, ring: int32)
---*int LuaScriptInterface::luaMonsterSpellSetConditionType(lua_State* L)
---@source ../src/luascript.cpp:15816
---@field setConditionType fun(self: MonsterSpell, type: ConditionType)
---*int LuaScriptInterface::luaMonsterSpellSetConditionDamage(lua_State* L)
---@source ../src/luascript.cpp:15829
---@field setConditionDamage fun(self: MonsterSpell, conditionMinDamage: int32, conditionMaxDamage: int32, conditionStartDamage: int32)
---*int LuaScriptInterface::luaMonsterSpellSetConditionSpeedChange(lua_State* L)
---@source ../src/luascript.cpp:15844
---@field setConditionSpeedChange fun(self: MonsterSpell, minSpeedChange: int32, maxSpeedChange?: int32)
---*int LuaScriptInterface::luaMonsterSpellSetConditionDuration(lua_State* L)
---@source ../src/luascript.cpp:15858
---@field setConditionDuration fun(self: MonsterSpell, duration: int32)
---*int LuaScriptInterface::luaMonsterSpellSetConditionDrunkenness(lua_State* L)
---@source ../src/luascript.cpp:15871
---@field setConditionDrunkenness fun(self: MonsterSpell, drunkenness: uint8)
---*int LuaScriptInterface::luaMonsterSpellSetConditionTickInterval(lua_State* L)
---@source ../src/luascript.cpp:15884
---@field setConditionTickInterval fun(self: MonsterSpell, tickInterval: int32)
---*int LuaScriptInterface::luaMonsterSpellSetCombatShootEffect(lua_State* L)
---@source ../src/luascript.cpp:15897
---@field setCombatShootEffect fun(self: MonsterSpell, effect: ShootTypes)
---*int LuaScriptInterface::luaMonsterSpellSetCombatEffect(lua_State* L)
---@source ../src/luascript.cpp:15910
---@field setCombatEffect fun(self: MonsterSpell, effect: MagicEffectConsts)
---*int LuaScriptInterface::luaMonsterSpellSetOutfit(lua_State* L)
---@source ../src/luascript.cpp:15923
---@field setOutfit fun(self: MonsterSpell, outfit: Outfit)
MonsterSpell = {}

--- The party class is used to define parties in the game.
--- <br>
--- Parties are used to group players together, allowing them to share experience/loot
--- <br><br>
--- Example usage:
--- ```lua
--- local playerParty = Player(playerId):getParty() -- Gets the player's party object
--- local partyMembers = playerParty:getMembers() -- Gets the party members
--- local partyLeader = playerParty:getLeader() -- Gets the party leader
--- for _, member in pairs(partyMembers) do
---     print("Party member: " .. member:getName()) -- Prints the name of each party member
--- end
--- print("Party leader: " .. partyLeader:getName()) -- Prints the name of the party leader
--- ```
--- Further definitions of the Party class can be found in `party.h`.
---@source ../src/party.h
---@class Party
---*int32_t LuaScriptInterface::luaPartyCreate(lua_State* L)
---@source ../src/luascript.cpp:15946
---@field create fun(): Party
---@field __eq fun(self: Party, other: Party): boolean
---*int LuaScriptInterface::luaPartyDisband(lua_State* L)
---@source ../src/luascript.cpp:15968
---@field disband fun(self: Party)
---*int LuaScriptInterface::luaPartyGetLeader(lua_State* L)
---@source ../src/luascript.cpp:15983
---@field getLeader fun(self: Party): Player
---*int LuaScriptInterface::luaPartySetLeader(lua_State* L)
---@source ../src/luascript.cpp:16002
---@field setLeader fun(self: Party, leader: Player)
---*int LuaScriptInterface::luaPartyGetMembers(lua_State* L)
---@source ../src/luascript.cpp:16015
---@field getMembers fun(self: Party): table<number, Player>
---*int LuaScriptInterface::luaPartyGetMemberCount(lua_State* L)
---@source ../src/luascript.cpp:16034
---@field getMemberCount fun(self: Party): number
---*int LuaScriptInterface::luaPartyGetInvitees(lua_State* L)
---@source ../src/luascript.cpp:16046
---@field getInvitees fun(self: Party): table<number, Player>
---*int LuaScriptInterface::luaPartyGetInviteeCount(lua_State* L)
---@source ../src/luascript.cpp:16065
---@field getInviteeCount fun(self: Party): number
---*int LuaScriptInterface::luaPartyAddInvite(lua_State* L)
---@source ../src/luascript.cpp:16077
---@field addInvite fun(self: Party, player: Player)
---*int LuaScriptInterface::luaPartyRemoveInvite(lua_State* L)
---@source ../src/luascript.cpp:16090
---@field removeInvite fun(self: Party, player: Player)
---*int LuaScriptInterface::luaPartyAddMember(lua_State* L)
---@source ../src/luascript.cpp:16103
---@field addMember fun(self: Party, player: Player)
---*int LuaScriptInterface::luaPartyRemoveMember(lua_State* L)
---@source ../src/luascript.cpp:16116
---@field removeMember fun(self: Party, player: Player)
---*int LuaScriptInterface::luaPartyIsSharedExperienceActive(lua_State* L)
---@source ../src/luascript.cpp:16129
---@field isSharedExperienceActive fun(self: Party): boolean
---*int LuaScriptInterface::luaPartyIsSharedExperienceEnabled(lua_State* L)
---@source ../src/luascript.cpp:16142
---@field isSharedExperienceEnabled fun(self: Party): boolean
---*int LuaScriptInterface::luaPartyIsMemberSharingExp(lua_State* L)
---@source ../src/luascript.cpp:16153
---@field isMemberSharingExp fun(self: Party, player: Player): boolean
---*int LuaScriptInterface::luaPartyShareExperience(lua_State* L)
---@source ../src/luascript.cpp:16166
---@field shareExperience fun(self: Party, experience: number)
---*int LuaScriptInterface::luaPartySetSharedExperience(lua_State* L)
---@source ../src/luascript.cpp:16180
---@field setSharedExperience fun(self: Party, active: boolean)
Party = {}

--- The spell class is used to define spells in the game.
--- <br>
--- Spells are defined in the `data/scripts/spells` folder, in revscript lua files.
--- <br><br>
--- **Note**: The spell class used in revscripts combine the `set` and `get` functions into one.
--- This means that if you call a function with a parameter, it will set the value. If you call it without a parameter, it will get the value.
--- <br><br>
--- Example usage:
--- ```lua
--- local combat = Combat()
--- combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_FIREDAMAGE)
--- combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_FIREATTACK)
--- combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_FIRE)
--- 
--- function onGetFormulaValues(player, level, magicLevel)
--- 	return -10, -20
--- end
--- 
--- combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")
--- 
--- local spell = Spell(SPELL_INSTANT) -- Can be the type of spell to create a spell, or or the spellName/spellId to call a previously created spell
--- 
--- function spell.onCastSpell(creature, variant)
--- 	return combat:execute(creature, variant)
--- end
--- 
--- spell:group("attack")
--- spell:id(169)
--- spell:name("Apprentice's Strike")
--- spell:words("exori min flam")
--- spell:level(8)
--- spell:mana(6)
--- spell:isPremium(true)
--- spell:range(3)
--- spell:needCasterTargetOrDirection(true)
--- spell:blockWalls(true)
--- spell:cooldown(2000)
--- spell:groupCooldown(2000)
--- spell:vocation("sorcerer;true", "druid;true", "master sorcerer;true", "elder druid;true")
--- spell:register()
--- ```
--- Further definitions of the Spell class can be found in `spells.h`. Useage in combat can be found in `combat.cpp`, and `spells.cpp`.
---@source ../src/spells.h
---@class Spell
---*int LuaScriptInterface::luaSpellCreate(lua_State* L)
---@source ../src/luascript.cpp:16194
---@field create fun(): Spell
---*int LuaScriptInterface::luaSpellOnCastSpell(lua_State* L)
---@source ../src/luascript.cpp:16266
---@field onCastSpell fun(player: Player, variant: Variant, isHotkey: boolean|nil): boolean
---*int LuaScriptInterface::luaSpellRegister(lua_State* L)
---@source ../src/luascript.cpp:16294
---@field register fun(self: Spell): boolean
---*int LuaScriptInterface::luaSpellName(lua_State* L)
---@source ../src/luascript.cpp:16328
---@field name fun(self: Spell, setName?: string): string|boolean
---*int LuaScriptInterface::luaSpellId(lua_State* L)
---@source ../src/luascript.cpp:16345
---@field id fun(self: Spell, setId?: uint8): uint8|boolean
---*int LuaScriptInterface::luaSpellCooldown(lua_State* L)
---@source ../src/luascript.cpp:16430
---@field cooldown fun(self: Spell, setCooldown?: uint32): uint32|boolean
---*int LuaScriptInterface::luaSpellGroup(lua_State* L)
---@source ../src/luascript.cpp:16362
---@field group fun(self: Spell, setPrimaryGroup?: SpellGroups|string, setSecondaryGroup?: SpellGroups|string): SpellGroups, SpellGroups|boolean
---*int LuaScriptInterface::luaSpellGroupCooldown(lua_State* L)
---@source ../src/luascript.cpp:16447
---@field groupCooldown fun(self:Spell, setPrimaryGroupCd?:uint32, setSecondaryGroupCd?:uint32): uint32, uint32|boolean
---*int LuaScriptInterface::luaSpellLevel(lua_State* L)
---@source ../src/luascript.cpp:16470
---@field level fun(self:Spell, setLevel?:uint32): uint32|boolean
---*int LuaScriptInterface::luaSpellMagicLevel(lua_State* L)
---@source ../src/luascript.cpp:16487
---@field magicLevel fun(self:Spell, setMagicLevel?:uint32):uint32|boolean
---*int LuaScriptInterface::luaSpellMana(lua_State* L)
---@source ../src/luascript.cpp:16504
---@field mana fun(self:Spell, setMana?:uint32):uint32 | boolean
---*int LuaScriptInterface::luaSpellManaPercent(lua_State* L)
---@source ../src/luascript.cpp:16521
---@field manaPercent fun(self:Spell, setManaPercent?:uint32):uint32 | boolean
---*int LuaScriptInterface::luaSpellSoul(lua_State* L)
---@source ../src/luascript.cpp:16538
---@field soul fun(self:Spell, setSoul?:uint32):uint32 | boolean
---*int LuaScriptInterface::luaSpellRange(lua_State* L)
---@source ../src/luascript.cpp:16555
---@field range fun(self:Spell, setRange?:int32):int32 | boolean
---*int LuaScriptInterface::luaSpellPremium(lua_State* L)
---@source ../src/luascript.cpp:16572
---@field isPremium fun(self:Spell, setPremium?:boolean):boolean
---*int LuaScriptInterface::luaSpellEnabled(lua_State* L)
---@source ../src/luascript.cpp:16589
---@field isEnabled fun(self:Spell, setEnabled?:boolean):boolean
---*int LuaScriptInterface::luaSpellNeedTarget(lua_State* L)
---@source ../src/luascript.cpp:16606
---@field needTarget fun(self:Spell, setNeedTarget?:boolean):boolean
---*int LuaScriptInterface::luaSpellNeedWeapon(lua_State* L)
---@source ../src/luascript.cpp:16623
---@field needWeapon fun(self:Spell, setNeedWeapon?:boolean):boolean
---*int LuaScriptInterface::luaSpellNeedLearn(lua_State* L)
---@source ../src/luascript.cpp:16640
---@field needLearn fun(self:Spell, setNeedLearn?:boolean):boolean
---*int LuaScriptInterface::luaSpellSelfTarget(lua_State* L)
---@source ../src/luascript.cpp:16657
---@field isSelfTarget fun(self:Spell, setSelfTarget?:boolean):boolean
---*int LuaScriptInterface::luaSpellBlocking(lua_State* L)
---@source ../src/luascript.cpp:16674
---@field isBlocking fun(self:Spell, setBlockingSolid?:boolean, setBlockingCreature?:boolean):boolean, boolean | boolean 
---*int LuaScriptInterface::luaSpellAggressive(lua_State* L)
---@source ../src/luascript.cpp:16694
---@field isAggressive fun(self:Spell, setIsAggressive?:boolean):boolean
---*int LuaScriptInterface::luaSpellPzLock(lua_State* L)
---@field isPzLock fun(self:Spell, setIsPzLock?:boolean):boolean
---*int LuaScriptInterface::luaSpellVocation(lua_State* L)
---@source ../src/luascript.cpp:16728
---@field vocation fun(self:Spell, setVocations?:string, ...?):table<number, string> | boolean
---*int LuaScriptInterface::luaSpellWords(lua_State* L)\
---@source ../src/luascript.cpp:16758
---@field words fun(self:Spell, setWords?:string, setSeperator?:string):string, string | boolean -- Only for InstantSpells
---*int LuaScriptInterface::luaSpellNeedDirection(lua_State* L)
---@source ../src/luascript.cpp:16789
---@field needDirection fun(self:Spell, setNeedDirection?:boolean):boolean -- Only for InstantSpells
---*int LuaScriptInterface::luaSpellHasParams(lua_State* L)
---@source ../src/luascript.cpp:16813
---@field hasParams fun(self:Spell, setHasParams?:boolean):boolean -- Only for InstantSpells
---*int LuaScriptInterface::luaSpellHasPlayerNameParam(lua_State* L)
---@source ../src/luascript.cpp:16837
---@field hasPlayerNameParam fun(self:Spell, setHasPlayerNameParam?:boolean):boolean -- Only for InstantSpells
---*int LuaScriptInterface::luaSpellNeedCasterTargetOrDirection(lua_State* L)
---@source ../src/luascript.cpp:16861
---@field needCasterTargetOrDirection fun(self:Spell, setNeedCasterTargetOrDirection?:boolean):boolean -- Only for InstantSpells
---*int LuaScriptInterface::luaSpellIsBlockingWalls(lua_State* L)
---@source ../src/luascript.cpp:16885
---@field isBlockingWalls fun(self:Spell, setIsBlockingWalls?:boolean):boolean -- Only for 
---*int LuaScriptInterface::luaSpellRuneLevel(lua_State* L)
---@source ../src/luascript.cpp:16909
---@field runeLevel fun(self:Spell, setRuneLevel?:integer):integer | boolean -- Only for RuneSpells
---*int LuaScriptInterface::luaSpellRuneMagicLevel(lua_State* L)
---@source ../src/luascript.cpp:16934
---@field runeMagicLevel fun(self:Spell, setRuneMagicLevel?:integer):integer | boolean -- Only for RuneSpells
---*int LuaScriptInterface::luaSpellRuneId(lua_State* L)
---@source ../src/luascript.cpp:16959
---@field runeId fun(self:Spell, setRuneId?:integer):integer | boolean -- Only for RuneSpells
---*int LuaScriptInterface::luaSpellCharges(lua_State* L)
---@source ../src/luascript.cpp:16983
---@field charges fun(self:Spell, setRuneCharges?:integer):integer | boolean -- Only for RuneSpells
---*int LuaScriptInterface::luaSpellAllowFarUse(lua_State* L)
---@source ../src/luascript.cpp:17007
---@field allowFarUse fun(self:Spell, setRuneAllowFarUse?:boolean):boolean -- Only for RuneSpells
---*int LuaScriptInterface::luaSpellBlockWalls(lua_State* L)
---@source ../src/luascript.cpp:17031
---@field blockWalls fun(self:Spell, setRuneBlockWalls?:boolean):boolean -- Only for RuneSpells
---*int LuaScriptInterface::luaSpellCheckFloor(lua_State* L)
---@source ../src/luascript.cpp:17055
---@field checkFloor fun(self:Spell, setRuneCheckFloor?:boolean):boolean -- Only for RuneSpells
---@field mlevel integer -- Only from Player.getInstantSpells
---@field manapercent integer -- Only from Player.getInstantSpells
---@field params boolean -- Only from Player.getInstantSpells
---@operator call(string | integer):Spell
---@source ../src/spells.h
Spell = {}

--- The action class is used to register or call actions in the game.
--- <br>
--- Actions are defined in `data/scripts/actions` folder, in revscript lua files.
--- <br>
--- Actions can only be defined during game startup.
--- <br><br>
--- **Note**: The action class used in revscripts combine the have no `get` functions, only `set` functions.
--- This means that after all actions are loaded on game startup, there is no way to check during run time what an action is registered with
--- <br><br>
--- Example usage:
--- ```lua
---local watch = Action()
---
---function watch.onUse(player, item, fromPosition, target, toPosition, isHotkey)
---	   player:sendTextMessage(MESSAGE_INFO_DESCR, "The time is " .. Game.getFormattedWorldTime() .. ".")
---	   return true
---end
---
---watch:id(1728, 1729, 1730, 1731, 2036, 3900, 6092, 7828, 9443, 9444) -- All itemIds that when used, will call this action's onUse function
---watch:register()
---```
---Further definitions of the Action class can be found in `actions.h`.
---@source ../src/actions.h
---@class Action
---*int LuaScriptInterface::luaActionOnUse(lua_State* L)
---@source ../src/luascript.cpp:17094
---@field onUse fun(player:Player, item:Item, fromPos:Position, target?:(Creature|Item), toPos:Position, isHotkey:boolean):boolean
---*int LuaScriptInterface::luaActionRegister(lua_State* L)
---@source ../src/luascript.cpp:17111
---@field register fun(self:Action):boolean
---*int LuaScriptInterface::luaActionItemId(lua_State* L)
---@source ../src/luascript.cpp:17130
---@field id fun(self:Action, itemId:uint16, ...?):boolean
---*int LuaScriptInterface::luaActionActionId(lua_State* L)
---@source ../src/luascript.cpp:17150
---@field aid fun(self:Action, actionId:uint16, ...?):boolean
---*int LuaScriptInterface::luaActionUniqueId(lua_State* L)
---@source ../src/luascript.cpp:17170
---@field uid fun(self:Action, uniqueId:uint16, ...?):boolean
---*int LuaScriptInterface::luaActionAllowFarUse(lua_State* L)
---@source ../src/luascript.cpp:17190
---@field allowFarUse fun(self:Action, allowFarUse:boolean):boolean
---*int LuaScriptInterface::luaActionBlockWalls(lua_State* L)
---@source ../src/luascript.cpp:17203
---@field blockWalls fun(self:Action, blockWalls:boolean):boolean
---*int LuaScriptInterface::luaActionCheckFloor(lua_State* L)
---@source ../src/luascript.cpp:17216
---@field checkFloor fun(self:Action, checkFloor:boolean):boolean
---@operator call():Action
Action = {}

--- Talkactions are used to define something that will happen when a player says a specific set of words in the game.
--- <br>
--- Talkactions are defined in `data/scripts/talkactions` folder, in revscript lua files.
--- <br>
--- Talkactions can only be defined during game startup.
--- <br><br>
--- Example usage:
--- ```lua
--- local talk = TalkAction("/pos", "!pos") -- Registering the words for the talkaction
--- 
--- function talk.onSay(player, words, param)
--- 	local position = player:getPosition()
--- 	player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Your current position is: " .. position.x .. ", " .. position.y .. ", " .. position.z .. ".")
--- 	return false -- Return false is used here so the command is not sent to the chat
--- end
--- 
--- talk:separator(" ")
--- talk:register()
--- ```
---@class TalkAction
---*int LuaScriptInterface::luaTalkactionOnSay(lua_State* L)
---@source ../src/luascript.cpp:17248
---@field onSay fun(player:Player, words:string, param:string, type:integer):boolean
---*int LuaScriptInterface::luaTalkactionRegister(lua_State* L)
---@source ../src/luascript.cpp:17264
---@field register fun(self:TalkAction):boolean
---*int LuaScriptInterface::luaTalkactionSeparator(lua_State* L)
---@source ../src/luascript.cpp:17280
---@field separator fun(self:TalkAction, separator?:string):boolean
---*int LuaScriptInterface::luaTalkactionAccess(lua_State* L)
---@source ../src/luascript.cpp:17293
---@field access fun(self:TalkAction, access?:boolean):boolean
---*int LuaScriptInterface::luaTalkactionAccountType(lua_State* L)
---@source ../src/luascript.cpp:17306
---@field accountType fun(self:TalkAction, accountType?:AccountTypes):boolean
---@overload fun(...:string):TalkAction
TalkAction = {}

--- A creature even is used to define a function that will be called when a specific event happens to a creature.
--- <br>
--- Creature events are defined in `data/scripts/creatureevents` folder, in revscript lua files.
--- <br>
--- Creature events can only be defined during game startup.
--- <br><br>
--- Some examples of creature events are:
--- <ol>
--- <li>"login" - Called when a player logs in</li>
--- <li>"logout" - Called when a player logs out</li>
--- <li>"reconnect" - Called when a player reconnects</li>
--- <li>"onThink" - Called every #ms for the registered creature</li>
--- <li>"prepareDeath" - Called when a creature is about to die</li>
--- </ol>
--- <br>
--- Example usage:
--- ```lua
--- local event = CreatureEvent("loginWelcomeMessage")
--- 
--- function event.onLogin(player)
--- 	player:sendTextMessage(MESSAGE_INFO_DESCR, "Welcome to the game!")
--- 	return true -- Return true to allow the player to log in, false to prevent it
--- end
--- 
--- event:register()
--- ```
---@class CreatureEvent
---*int LuaScriptInterface::luaCreatureEventType(lua_State* L)
---@source ../src/luascript.cpp:17336
---@field type fun(self:CreatureEvent, callback:string):boolean
---*int LuaScriptInterface::luaCreatureEventRegister(lua_State* L)
---@source ../src/luascript.cpp:17382
---@field register fun(self:CreatureEvent):boolean
---*int LuaScriptInterface::luaCreatureEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17398
---@field onLogin fun(player:Player):boolean
---*int LuaScriptInterface::luaCreatureEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17398
---@field onLogout fun(player:Player):boolean
---*int LuaScriptInterface::luaCreatureEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17398
---@field onReconnect fun(player:Player)
---*int LuaScriptInterface::luaCreatureEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17398
---@field onThink fun(creature:Creature, interval:integer):boolean
---*int LuaScriptInterface::luaCreatureEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17398
---@field onPrepareDeath fun(creature:Creature, killer:Creature):boolean
---*int LuaScriptInterface::luaCreatureEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17398
---@field onDeath fun(creature:Creature, corpse:Item, killer:Creature, mostDamageKiller:Creature, lastHitUnjustified:boolean, mostDamageUnjustified:boolean):boolean
---*int LuaScriptInterface::luaCreatureEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17398
---@field onAdvance fun(player:Player, skill:integer, oldLevel:integer, newLevel:integer):boolean
---*int LuaScriptInterface::luaCreatureEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17398
---@field onKill fun(player:Player, target:Creature):boolean
---*int LuaScriptInterface::luaCreatureEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17398
---*int LuaScriptInterface::luaCreatureEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17398
---@field onTextEdit fun(player:Player, item:Item, text:string, windowTextId:integer):boolean
---*int LuaScriptInterface::luaCreatureEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17398
---@field onHealthChange fun(creature:Creature, attacker:Creature, primaryDamage:integer, primaryType:integer, secondaryDamage:integer, secondaryType:integer, origin:integer):integer, integer, integer, integer
---*int LuaScriptInterface::luaCreatureEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17398
---*int LuaScriptInterface::luaCreatureEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17398
---@field onManaChange fun(creature:Creature, attacker:Creature, primaryDamage:integer, primaryType:integer, secondaryDamage:integer, secondaryType:integer, origin:integer):integer, integer, integer, integer
---*int LuaScriptInterface::luaCreatureEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17398
---@field onModalWindow fun(player:Player, modalWindowId:integer, buttonId:integer, choiceId:integer):nil
---*int LuaScriptInterface::luaCreatureEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17398
---@field onExtendedOpcode fun(player:Player, opcode:integer, buffer:string):boolean
---@operator call(string):CreatureEvent
CreatureEvent = {}

--- MoveEvents are used to defined something that will happen when a creature moves themself, or an item in the game.
--- <br>
--- MoveEvents are defined in `data/scripts/moveevents` folder, in revscript lua files.
--- <br>
--- Some examples of move events are:
--- <ol>
--- <li>"onStepIn" - Called when a player steps on a tile</li>
--- <li>"onStepOut" - Called when a player steps out of a tile</li>
--- <li>"onEquip" - Called when a player equips an item</li>
--- <li>"onDeEquip" - Called when a player de-equips an item</li>
--- </ol>
--- <br>
--- Example usage:
--- ```lua
--- local event = MoveEvent("onStepIn")
--- 
--- function event.onStepIn(creature, item, position, fromPosition)
---  	if creature:isPlayer() then
---  		creature:sendTextMessage(MESSAGE_INFO_DESCR, "You stepped on a tile!")
---  	end
---  	return true
--- end
--- 
--- event:register()
--- ```
--- Further definitions of the MoveEvent class can be found in `movement.h`.
---@class MoveEvent
---*int LuaScriptInterface::luaMoveEventType(lua_State* L)
---@source ../src/luascript.cpp:17430
---@field type fun(self:MoveEvent, callback:string):boolean
---*int LuaScriptInterface::luaMoveEventRegister(lua_State* L)
---@source ../src/luascript.cpp:17446
---@field register fun(self:MoveEvent):boolean
---*int LuaScriptInterface::luaMoveEventSlot(lua_State* L)
---@source ../src/luascript.cpp:17509
---@field slot fun(self:MoveEvent, slot:string):boolean
---*int LuaScriptInterface::luaMoveEventLevel(lua_State* L)
---@source ../src/luascript.cpp:17553
---@field level fun(self:MoveEvent, level:uint32):boolean
---*int LuaScriptInterface::luaMoveEventMagLevel(lua_State* L)
---@source ../src/luascript.cpp:17567
---@field magicLevel fun(self:MoveEvent, magicLevel:uint32):boolean
---*int LuaScriptInterface::luaMoveEventPremium(lua_State* L)
---@source ../src/luascript.cpp:17581
---@field premium fun(self:MoveEvent, premium:boolean):boolean
---*int LuaScriptInterface::luaMoveEventVocation(lua_State* L)
---@source ../src/luascript.cpp:17595
---@field vocation fun(self:MoveEvent, vocation:string, showInDescription?:boolean, lastVocation?:boolean):boolean
---*int LuaScriptInterface::luaMoveEventTileItem(lua_State* L)
---@source ../src/luascript.cpp:17635
---@field tileItem fun(self:MoveEvent, tileItem:boolean):boolean
---*int LuaScriptInterface::luaMoveEventItemId(lua_State* L)
---@source ../src/luascript.cpp:17648
---@field id fun(self:MoveEvent, itemId:uint32, ...?):boolean
---*int LuaScriptInterface::luaMoveEventActionId(lua_State* L)
---@source ../src/luascript.cpp:17668
---@field aid fun(self:MoveEvent, actionId:uint32, ...?):boolean
---*int LuaScriptInterface::luaMoveEventUniqueId(lua_State* L)
---@source ../src/luascript.cpp:17688
---@field uid fun(self:MoveEvent, uniqueId:uint32, ...?):boolean
---*int LuaScriptInterface::luaMoveEventPosition(lua_State* L)
---@source ../src/luascript.cpp:17708
---@field position fun(self:MoveEvent, position:Position, ...?):boolean
---*int LuaScriptInterface::luaMoveEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17493
---@field onStepIn fun(creature:Creature, item:Item, position:Position, fromPosition:Position):boolean
---*int LuaScriptInterface::luaMoveEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17493
---@field onStepOut fun(creature:Creature, item:Item, position:Position, fromPosition:Position):boolean
---*int LuaScriptInterface::luaMoveEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17493
---@field onEquip fun(player:Player, item:Item, slot:integer, isCheck:boolean):boolean
---*int LuaScriptInterface::luaMoveEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17493
---@field onDeEquip fun(player:Player, item:Item, slot:integer, isCheck:boolean):boolean
---*int LuaScriptInterface::luaMoveEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17493
---@field onAddItem fun(moveItem:Item, tileItem:Item, pos:Position):boolean
---@operator call():MoveEvent
MoveEvent = {}

--- GlobalEvents are used to define something when the gamestate is at certain points.
--- <br>
--- GlobalEvents are defined in `data/scripts/globalevents` folder, in revscript lua files.
--- <br><br>
--- Globalevent types are:
--- <ol>
--- <li>`onStartup` - Called when the game starts</li>
--- <li>`onShutdown` - Called when the game shuts down</li>
--- <li>`timer` - Called every #ms for the server</li>
--- <li>`save` - Called when the server saves</li>
--- <li>`record` - Called when the max online player count reaches a new record</li>
--- </ol>
--- <br>
--- Example usage:
--- ```lua
--- local onStartupEvent = GlobalEvent("onStartup")
--- 
--- function onStartupEvent.onStartup()
---     print("The server has started!")
---     return true
--- end
--- 
--- onStartupEvent:register()
--- ```
--- Further definitions of the GlobalEvent class can be found in `globalevent.h`.
---@class GlobalEvent
---*int LuaScriptInterface::luaGlobalEventType(lua_State* L)
---@source ../src/luascript.cpp:17746
---@field type fun(self:GlobalEvent, callback:string):boolean
---*int LuaScriptInterface::luaGlobalEventRegister(lua_State* L)
---@source ../src/luascript.cpp:17775
---@field register fun(self:GlobalEvent):boolean
---*int LuaScriptInterface::luaGlobalEventTime(lua_State* L)
---@source ../src/luascript.cpp:17815
---@field time fun(self:GlobalEvent, time:string):boolean -- "hh:mm:ss"
---*int LuaScriptInterface::luaGlobalEventInterval(lua_State* L)
---@source ../src/luascript.cpp:17875
---@field interval fun(self:GlobalEvent, interval:integer):boolean
---*int LuaScriptInterface::luaGlobalEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17799
---@field onStartup fun():boolean
---*int LuaScriptInterface::luaGlobalEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17799
---@field onShutdown fun():boolean
---*int LuaScriptInterface::luaGlobalEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17799
---@field onRecord fun(current:integer, old:integer):boolean
---*int LuaScriptInterface::luaGlobalEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17799
---@field onThink fun(interval:integer):boolean
---*int LuaScriptInterface::luaGlobalEventOnCallback(lua_State* L)
---@source ../src/luascript.cpp:17799
---@field onTime fun(interval:integer):boolean
---@operator call(string):GlobalEvent
GlobalEvent = {}

--- The weapon class is used to define a scripted weapon in the game.
--- <br>
--- Weapons are defined in `data/scripts/weapons` folder, in revscript lua files.
--- <br>
--- Weapons registered in this way can have level/skill requirements to use the weapon, have a mana/health cost, define the damage, and other properties.
--- <br><br>
--- Example usage:
--- ```lua
--- local area = createCombatArea({
--- 	{1, 1, 1},
--- 	{1, 3, 1},
--- 	{1, 1, 1}
--- })
--- 
--- local combat = Combat()
--- combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
--- combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_EXPLOSIONAREA)
--- combat:setParameter(COMBAT_PARAM_BLOCKARMOR, true)
--- combat:setFormula(COMBAT_FORMULA_SKILL, 0, 0, 1, 0)
--- combat:setArea(area)
--- 
--- local burstarrow = Weapon(WEAPON_AMMO)
--- 
--- burstarrow.onUseWeapon = function(player, variant)
--- 	if player:getSkull() == SKULL_BLACK then
--- 		return false
--- 	end
--- 
--- 	return combat:execute(player, variant)
--- end
--- 
--- burstarrow:id(2546)
--- burstarrow:attack(27)
--- burstarrow:shootType(CONST_ANI_BURSTARROW)
--- burstarrow:ammoType("arrow")
--- burstarrow:maxHitChance(100)
--- burstarrow:register()
--- ```
--- Further definitions of the Weapon class can be found in `weapons.h`.
---@class Weapon
---*int LuaScriptInterface::luaCreateWeapon(lua_State* L)
---@source ../src/luascript.cpp:17890
---@field create fun(weaponType: WeaponType): Weapon
---*int LuaScriptInterface::luaWeaponAction(lua_State* L)
---@source ../src/luascript.cpp:17936
---@field action fun(self:Weapon, callback:string):boolean
---*int LuaScriptInterface::luaWeaponRegister(lua_State* L)
---@source ../src/luascript.cpp:17960
---@field register fun(self:Weapon):boolean
---*int LuaScriptInterface::luaWeaponOnUseWeapon(lua_State* L)
---@source ../src/luascript.cpp:17998
---@field onUseWeapon fun(player:Player, variant:Variant):boolean
---*int LuaScriptInterface::luaWeaponUnproperly(lua_State* L)
---@source ../src/luascript.cpp:18014
---@field wieldedUnproperly fun(self:Weapon, bool:boolean):boolean
---*int LuaScriptInterface::luaWeaponLevel(lua_State* L)
---@source ../src/luascript.cpp:18027
---@field level fun(self:Weapon, lvl:uint32):boolean
---*int LuaScriptInterface::luaWeaponMagicLevel(lua_State* L)
---@source ../src/luascript.cpp:18041
---@field magicLevel fun(self:Weapon, lvl:uint32):boolean
---*int LuaScriptInterface::luaWeaponMana(lua_State* L)
---@source ../src/luascript.cpp:18055
---@field mana fun(self:Weapon, mana:uint32):boolean
---*int LuaScriptInterface::luaWeaponManaPercent(lua_State* L)
---@source ../src/luascript.cpp:18068
---@field manaPercent fun(self:Weapon, percent:uint32):boolean
---*int LuaScriptInterface::luaWeaponHealth(lua_State* L)
---@source ../src/luascript.cpp:18081
---@field health fun(self:Weapon, health:int32):boolean
---*int LuaScriptInterface::luaWeaponHealthPercent(lua_State* L)
---@source ../src/luascript.cpp:18094
---@field healthPercent fun(self:Weapon, percent:uint32):boolean
---*int LuaScriptInterface::luaWeaponSoul(lua_State* L)
---@source ../src/luascript.cpp:18107
---@field soul fun(self:Weapon, soul:uint32):boolean
---*int LuaScriptInterface::luaWeaponBreakChance(lua_State* L)
---@source ../src/luascript.cpp:18120
---@field breakChance fun(self:Weapon, percent:uint32):boolean
---*int LuaScriptInterface::luaWeaponWandDamage(lua_State* L)
---@source ../src/luascript.cpp:18133
---@field damage fun(self:Weapon, min?:uint32, max?:uint32):boolean -- Note: Only used for wands
---*int LuaScriptInterface::luaWeaponElement(lua_State* L)
---@source ../src/luascript.cpp:18151
---@field element fun(self:Weapon, combatType:CombatTypes|string):boolean
---*int LuaScriptInterface::luaWeaponPremium(lua_State* L)
---@source ../src/luascript.cpp:18184
---@field isPremium fun(self:Weapon, bool:boolean):boolean
---*int LuaScriptInterface::luaWeaponVocation(lua_State* L)
---@source ../src/luascript.cpp:18198
---@field vocation fun(self:Weapon, vocName:string, showInDescription?:boolean, lastVoc?:boolean):boolean
---*int LuaScriptInterface::luaWeaponId(lua_State* L)
---@source ../src/luascript.cpp:18233
---@field id fun(self:Weapon, id:uint32):boolean
---*int LuaScriptInterface::luaWeaponAttack(lua_State* L)
---@source ../src/luascript.cpp:18246
---@field attack fun(self:Weapon, atk:int32):boolean
---*int LuaScriptInterface::luaWeaponDefense(lua_State* L)
---@source ../src/luascript.cpp:18261
---@field defense fun(self:Weapon, defense:int32, extraDefense?:int32):boolean
---*int LuaScriptInterface::luaWeaponRange(lua_State* L)
---@source ../src/luascript.cpp:18279
---@field range fun(self:Weapon, range:uint8):boolean
---*int LuaScriptInterface::luaWeaponCharges(lua_State* L)
---@source ../src/luascript.cpp:18294
---@field charges fun(self:Weapon, charges:uint32, showCharges?:boolean):boolean
---*int LuaScriptInterface::luaWeaponDuration(lua_State* L)
---@source ../src/luascript.cpp:18312
---@field duration fun(self:Weapon, duration:uint32|table<{min: uint32, max: uint32}>, showDuration?:boolean):boolean
---*int LuaScriptInterface::luaWeaponDecayTo(lua_State* L)
---@source ../src/luascript.cpp:18336
---@field decayTo fun(self:Weapon, itemid?:uint16):boolean
---*int LuaScriptInterface::luaWeaponTransformEquipTo(lua_State* L)
---@source ../src/luascript.cpp:18353
---@field transformEquipTo fun(self:Weapon, itemid:uint16):boolean
---*int LuaScriptInterface::luaWeaponTransformDeEquipTo(lua_State* L)
---@source ../src/luascript.cpp:18368
---@field transformDeEquipTo fun(self:Weapon, itemid:uint16):boolean
---*int LuaScriptInterface::luaWeaponShootType(lua_State* L)
---@source ../src/luascript.cpp:18383
---@field shootType fun(self:Weapon, type:ShootTypes):boolean
---*int LuaScriptInterface::luaWeaponSlotType(lua_State* L)
---@source ../src/luascript.cpp:18396
---@field slotType fun(self:Weapon, slot:string):boolean
---*int LuaScriptInterface::luaWeaponAmmoType(lua_State* L)
---@source ../src/luascript.cpp:18419
---@field ammoType fun(self:Weapon, type:string):boolean
---*int LuaScriptInterface::luaWeaponHitChance(lua_State* L)
---@source ../src/luascript.cpp:18444
---@field hitChance fun(self:Weapon, chance:int8):boolean
---*int LuaScriptInterface::luaWeaponMaxHitChance(lua_State* L)
---@source ../src/luascript.cpp:18459
---@field maxHitChance fun(self:Weapon, max:int32):boolean
---*int LuaScriptInterface::luaWeaponExtraElement(lua_State* L)
---@field extraElement fun(self:Weapon, atk:uint16, combatType:CombatTypes|string):boolean
---@operator call(integer):Weapon
Weapon = {}

--- Event/EventCallbacks are used to define scripts to be called when specific events happen in the game. 
--- <br>
--- EventCallbacks lib is in `data/scripts/lib/event_callbacks.lua`.
--- <br>
--- Default event callbacks are defined in `data/scripts/events/` folders.
--- <br><br>
--- For example usages, refer to `data/scripts/` folders.
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

--- hasEvent checks if a specific event is registered for a creature.
--- <br>
--- It is used only in revscripts to check if an event is registered for a specific creature, then it will initiate the callback.
--- <br>
--- Unless registering new event types, this should not have to be changed or interacted with.
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

--- The "Thing" class is the base class for all objects in the game, including items and creatures.
--- <br>
--- Instead of interacting with this class directly, you should use the specific classes for items and creatures (player/monster/npc).
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

--- XMLDocument class is used to utilize C++ XML functions in Lua.
--- <br>
--- Specifically, XMLDocument is used to get XMLNodes to be used by the XMLNode class.
---@class XMLDocument
---*int LuaScriptInterface::luaCreateXmlDocument(lua_State* L)
---@source ../src/luascript.cpp:18512
---@field create fun(): XMLDocument
---@field __gc fun(self: XMLDocument)
---@field delete fun(self: XMLDocument)
---*int LuaScriptInterface::luaXmlDocumentChild(lua_State* L)
---@source ../src/luascript.cpp:18543
---@field child fun(self: XMLDocument, childName: string): XMLNode
XMLDocument = {}

--- XMLNodes are used to represent a node in an XML document.
--- <br>
--- It utilizies the C++ XML functions to get the data from the XML document.
---@class XMLNode
---@field create fun(): XMLNode
---@field __gc fun(self: XMLNode)
---@field delete fun(self: XMLNode)
---*int LuaScriptInterface::luaXmlNodeAttribute(lua_State* L)
---@source ../src/luascript.cpp:18575
---@field attribute fun(self: XMLNode, attributeName: string): string
---*int LuaScriptInterface::luaXmlNodeName(lua_State* L)
---@source ../src/luascript.cpp:18599
---@field name fun(self: XMLNode): string
---*int LuaScriptInterface::luaXmlNodeFirstChild(lua_State* L)
---@source ../src/luascript.cpp:18612
---@field firstChild fun(self: XMLNode): XMLNode
---*int LuaScriptInterface::luaXmlNodeNextSibling(lua_State* L)
---@source ../src/luascript.cpp:18633
---@field nextSibling fun(self: XMLNode): XMLNode
XMLNode = {}

-- Legacy Functions

--- Adds an item to a player's inventory.
---@deprecated -- use player:addItem() instead
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
---@source ../src/game.h:63
ITEM_STACK_SIZE = 100

---@alias ShootTypes
---| 0 # CONST_ANI_NONE
---| 1 # CONST_ANI_SPEAR
---| 2 # CONST_ANI_BOLT
---| 3 # CONST_ANI_ARROW
---| 4 # CONST_ANI_FIRE
---| 5 # CONST_ANI_ENERGY
---| 6 # CONST_ANI_POISONARROW
---| 7 # CONST_ANI_BURSTARROW
---| 8 # CONST_ANI_THROWINGSTAR
---| 9 # CONST_ANI_THROWINGKNIFE
---| 10 # CONST_ANI_SMALLSTONE
---| 11 # CONST_ANI_DEATH
---| 12 # CONST_ANI_LARGEROCK
---| 13 # CONST_ANI_SNOWBALL
---| 14 # CONST_ANI_POWERBOLT
---| 15 # CONST_ANI_POISON
---| 16 # CONST_ANI_INFERNALBOLT
---| 17 # CONST_ANI_HUNTINGSPEAR
---| 18 # CONST_ANI_ENCHANTEDSPEAR
---| 19 # CONST_ANI_REDSTAR
---| 20 # CONST_ANI_GREENSTAR
---| 21 # CONST_ANI_ROYALSPEAR
---| 22 # CONST_ANI_SNIPERARROW
---| 23 # CONST_ANI_ONYXARROW
---| 24 # CONST_ANI_PIERCINGBOLT
---| 25 # CONST_ANI_WHIRLWINDSWORD
---| 26 # CONST_ANI_WHIRLWINDAXE
---| 27 # CONST_ANI_WHIRLWINDCLUB
---| 28 # CONST_ANI_ETHEREALSPEAR
---| 29 # CONST_ANI_ICE
---| 30 # CONST_ANI_EARTH
---| 31 # CONST_ANI_HOLY
---| 32 # CONST_ANI_SUDDENDEATH
---| 33 # CONST_ANI_FLASHARROW
---| 34 # CONST_ANI_FLAMMINGARROW
---| 35 # CONST_ANI_SHIVERARROW
---| 36 # CONST_ANI_ENERGYBALL
---| 37 # CONST_ANI_SMALLICE
---| 38 # CONST_ANI_SMALLHOLY
---| 39 # CONST_ANI_SMALLEARTH
---| 40 # CONST_ANI_EARTHARROW
---| 41 # CONST_ANI_EXPLOSION
---| 42 # CONST_ANI_CAKE
---| 44 # CONST_ANI_TARSALARROW
---| 45 # CONST_ANI_VORTEXBOLT
---| 48 # CONST_ANI_PRISMATICBOLT
---| 49 # CONST_ANI_CRYSTALLINEARROW
---| 50 # CONST_ANI_DRILLBOLT
---| 51 # CONST_ANI_ENVENOMEDARROW
---| 53 # CONST_ANI_GLOOTHSPEAR
---| 54 # CONST_ANI_SIMPLEARROW
---| 56 # CONST_ANI_LEAFSTAR
---| 57 # CONST_ANI_DIAMONDARROW
---| 58 # CONST_ANI_SPECTRALBOLT
---| 59 # CONST_ANI_ROYALSTAR
---| 254 # CONST_ANI_WEAPONTYPE

---@source ../src/const.h:178
CONST_ANI_NONE = 0
---@source ../src/const.h:178
CONST_ANI_SPEAR = 1
---@source ../src/const.h:178
CONST_ANI_BOLT = 2
---@source ../src/const.h:178
CONST_ANI_ARROW = 3
---@source ../src/const.h:178
CONST_ANI_FIRE = 4
---@source ../src/const.h:178
CONST_ANI_ENERGY = 5
---@source ../src/const.h:178
CONST_ANI_POISONARROW = 6
---@source ../src/const.h:178
CONST_ANI_BURSTARROW = 7
---@source ../src/const.h:178
CONST_ANI_THROWINGSTAR = 8
---@source ../src/const.h:178
CONST_ANI_THROWINGKNIFE = 9
---@source ../src/const.h:178
CONST_ANI_SMALLSTONE = 10
---@source ../src/const.h:178
CONST_ANI_DEATH = 11
---@source ../src/const.h:178
CONST_ANI_LARGEROCK = 12
---@source ../src/const.h:178
CONST_ANI_SNOWBALL = 13
---@source ../src/const.h:178
CONST_ANI_POWERBOLT = 14
---@source ../src/const.h:178
CONST_ANI_POISON = 15
---@source ../src/const.h:178
CONST_ANI_INFERNALBOLT = 16
---@source ../src/const.h:178
CONST_ANI_HUNTINGSPEAR = 17
---@source ../src/const.h:178
CONST_ANI_ENCHANTEDSPEAR = 18
---@source ../src/const.h:178
CONST_ANI_REDSTAR = 19
---@source ../src/const.h:178
CONST_ANI_GREENSTAR = 20
---@source ../src/const.h:178
CONST_ANI_ROYALSPEAR = 21
---@source ../src/const.h:178
CONST_ANI_SNIPERARROW = 22
---@source ../src/const.h:178
CONST_ANI_ONYXARROW = 23
---@source ../src/const.h:178
CONST_ANI_PIERCINGBOLT = 24
---@source ../src/const.h:178
CONST_ANI_WHIRLWINDSWORD = 25
---@source ../src/const.h:178
CONST_ANI_WHIRLWINDAXE = 26
---@source ../src/const.h:178
CONST_ANI_WHIRLWINDCLUB = 27
---@source ../src/const.h:178
CONST_ANI_ETHEREALSPEAR = 28
---@source ../src/const.h:178
CONST_ANI_ICE = 29
---@source ../src/const.h:178
CONST_ANI_EARTH = 30
---@source ../src/const.h:178
CONST_ANI_HOLY = 31
---@source ../src/const.h:178
CONST_ANI_SUDDENDEATH = 32
---@source ../src/const.h:178
CONST_ANI_FLASHARROW = 33
---@source ../src/const.h:178
CONST_ANI_FLAMMINGARROW = 34
---@source ../src/const.h:178
CONST_ANI_SHIVERARROW = 35
---@source ../src/const.h:178
CONST_ANI_ENERGYBALL = 36
---@source ../src/const.h:178
CONST_ANI_SMALLICE = 37
---@source ../src/const.h:178
CONST_ANI_SMALLHOLY = 38
---@source ../src/const.h:178
CONST_ANI_SMALLEARTH = 39
---@source ../src/const.h:178
CONST_ANI_EARTHARROW = 40
---@source ../src/const.h:178
CONST_ANI_EXPLOSION = 41
---@source ../src/const.h:178
CONST_ANI_CAKE = 42
---@source ../src/const.h:178
CONST_ANI_TARSALARROW = 44
---@source ../src/const.h:178
CONST_ANI_VORTEXBOLT = 45
---@source ../src/const.h:178
CONST_ANI_PRISMATICBOLT = 48
---@source ../src/const.h:178
CONST_ANI_CRYSTALLINEARROW = 49
---@source ../src/const.h:178
CONST_ANI_DRILLBOLT = 50
---@source ../src/const.h:178
CONST_ANI_ENVENOMEDARROW = 51
---@source ../src/const.h:178
CONST_ANI_GLOOTHSPEAR = 53
---@source ../src/const.h:178
CONST_ANI_SIMPLEARROW = 54
---@source ../src/const.h:178
CONST_ANI_LEAFSTAR = 56
---@source ../src/const.h:178
CONST_ANI_DIAMONDARROW = 57
---@source ../src/const.h:178
CONST_ANI_SPECTRALBOLT = 58
---@source ../src/const.h:178
CONST_ANI_ROYALSTAR = 59
---@source ../src/const.h:178
CONST_ANI_WEAPONTYPE = 254

---@alias MagicEffectConsts
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

---@alias ItemProperties
---| 0 # CONST_PROP_BLOCKSOLID
---| 1 # CONST_PROP_HASHEIGHT
---| 2 # CONST_PROP_BLOCKPROJECTILE
---| 3 # CONST_PROP_BLOCKPATH
---| 4 # CONST_PROP_ISVERTICAL
---| 5 # CONST_PROP_ISHORIZONTAL
---| 6 # CONST_PROP_MOVEABLE
---| 7 # CONST_PROP_IMMOVABLEBLOCKSOLID
---| 8 # CONST_PROP_IMMOVABLEBLOCKPATH
---| 9 # CONST_PROP_IMMOVABLENOFIELDBLOCKPATH
---| 10 # CONST_PROP_NOFIELDBLOCKPATH
---| 11 # CONST_PROP_SUPPORTHANGABLE

---@source ../src/item.h:22
CONST_PROP_BLOCKSOLID = 0
---@source ../src/item.h:22
CONST_PROP_HASHEIGHT = 1
---@source ../src/item.h:22
CONST_PROP_BLOCKPROJECTILE = 2
---@source ../src/item.h:22
CONST_PROP_BLOCKPATH = 3
---@source ../src/item.h:22
CONST_PROP_ISVERTICAL = 4
---@source ../src/item.h:22
CONST_PROP_ISHORIZONTAL = 5
---@source ../src/item.h:22
CONST_PROP_MOVEABLE = 6
---@source ../src/item.h:22
CONST_PROP_IMMOVABLEBLOCKSOLID = 7
---@source ../src/item.h:22
CONST_PROP_IMMOVABLEBLOCKPATH = 8
---@source ../src/item.h:22
CONST_PROP_IMMOVABLENOFIELDBLOCKPATH = 9
---@source ../src/item.h:22
CONST_PROP_NOFIELDBLOCKPATH = 10
---@source ../src/item.h:22
CONST_PROP_SUPPORTHANGABLE = 11


---@alias PlayerEquipmentSlots
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

---@alias ZoneTypes
---| 0 # ZONE_PROTECTION
---| 1 # ZONE_NOPVP
---| 2 # ZONE_PVP
---| 3 # ZONE_NOLOGOUT
---| 4 # ZONE_NORMAL

---@source ../src/tile.h:56
ZONE_PROTECTION = 0
---@source ../src/tile.h:56
ZONE_NOPVP = 1
---@source ../src/tile.h:56
ZONE_PVP = 2
---@source ../src/tile.h:56
ZONE_NOLOGOUT = 3
---@source ../src/tile.h:56
ZONE_NORMAL = 4

---@alias TileFlags
---| 0 # TILESTATE_NONE
---| 1 # TILESTATE_FLOORCHANGE_DOWN
---| 2 # TILESTATE_FLOORCHANGE_NORTH
---| 4 # TILESTATE_FLOORCHANGE_SOUTH
---| 8 # TILESTATE_FLOORCHANGE_EAST
---| 16 # TILESTATE_FLOORCHANGE_WEST
---| 32 # TILESTATE_FLOORCHANGE_SOUTH_ALT
---| 64 # TILESTATE_FLOORCHANGE_EAST_ALT
---| 128 # TILESTATE_PROTECTIONZONE
---| 256 # TILESTATE_NOPVPZONE
---| 512 # TILESTATE_NOLOGOUT
---| 1024 # TILESTATE_PVPZONE
---| 2048 # TILESTATE_TELEPORT
---| 4096 # TILESTATE_MAGICFIELD
---| 8192 # TILESTATE_MAILBOX
---| 16384 # TILESTATE_TRASHHOLDER
---| 32768 # TILESTATE_BED
---| 65536 # TILESTATE_DEPOT
---| 131072 # TILESTATE_BLOCKSOLID
---| 262144 # TILESTATE_BLOCKPATH
---| 524288 # TILESTATE_IMMOVABLEBLOCKSOLID
---| 1048576 # TILESTATE_IMMOVABLEBLOCKPATH
---| 2097152 # TILESTATE_IMMOVABLENOFIELDBLOCKPATH
---| 4194304 # TILESTATE_NOFIELDBLOCKPATH
---| 8388608 # TILESTATE_SUPPORTS_HANGABLE

---@source ../src/tile.h:22
TILESTATE_NONE = 0
---@source ../src/tile.h:22
TILESTATE_FLOORCHANGE_DOWN = 1 * 2 ^ 0
---@source ../src/tile.h:22
TILESTATE_FLOORCHANGE_NORTH = 1 * 2 ^ 1
---@source ../src/tile.h:22
TILESTATE_FLOORCHANGE_SOUTH = 1 * 2 ^ 2
---@source ../src/tile.h:22
TILESTATE_FLOORCHANGE_EAST = 1 * 2 ^ 3
---@source ../src/tile.h:22
TILESTATE_FLOORCHANGE_WEST = 1 * 2 ^ 4
---@source ../src/tile.h:22
TILESTATE_FLOORCHANGE_SOUTH_ALT = 1 * 2 ^ 5
---@source ../src/tile.h:22
TILESTATE_FLOORCHANGE_EAST_ALT = 1 * 2 ^ 6
---@source ../src/tile.h:22
TILESTATE_PROTECTIONZONE = 1 * 2 ^ 7
---@source ../src/tile.h:22
TILESTATE_NOPVPZONE = 1 * 2 ^ 8
---@source ../src/tile.h:22
TILESTATE_NOLOGOUT = 1 * 2 ^ 9
---@source ../src/tile.h:22
TILESTATE_PVPZONE = 1 * 2 ^ 10
---@source ../src/tile.h:22
TILESTATE_TELEPORT = 1 * 2 ^ 11
---@source ../src/tile.h:22
TILESTATE_MAGICFIELD = 1 * 2 ^ 12
---@source ../src/tile.h:22
TILESTATE_MAILBOX = 1 * 2 ^ 13
---@source ../src/tile.h:22
TILESTATE_TRASHHOLDER = 1 * 2 ^ 14
---@source ../src/tile.h:22
TILESTATE_BED = 1 * 2 ^ 15
---@source ../src/tile.h:22
TILESTATE_DEPOT = 1 * 2 ^ 16
---@source ../src/tile.h:22
TILESTATE_BLOCKSOLID = 1 * 2 ^ 17
---@source ../src/tile.h:22
TILESTATE_BLOCKPATH = 1 * 2 ^ 18
---@source ../src/tile.h:22
TILESTATE_IMMOVABLEBLOCKSOLID = 1 * 2 ^ 19
---@source ../src/tile.h:22
TILESTATE_IMMOVABLEBLOCKPATH = 1 * 2 ^ 20
---@source ../src/tile.h:22
TILESTATE_IMMOVABLENOFIELDBLOCKPATH = 1 * 2 ^ 21
---@source ../src/tile.h:22
TILESTATE_NOFIELDBLOCKPATH = 1 * 2 ^ 22
---@source ../src/tile.h:22
TILESTATE_SUPPORTS_HANGABLE = 1 * 2 ^ 23

---@source ../src/tile.h:22
TILESTATE_FLOORCHANGE =
	TILESTATE_FLOORCHANGE_DOWN or TILESTATE_FLOORCHANGE_NORTH or
		TILESTATE_FLOORCHANGE_SOUTH or TILESTATE_FLOORCHANGE_EAST or
		TILESTATE_FLOORCHANGE_WEST or TILESTATE_FLOORCHANGE_SOUTH_ALT or
		TILESTATE_FLOORCHANGE_EAST_ALT

SPELL_UNDEFINED = 0
SPELL_INSTANT = 1
SPELL_RUNE = 2

---@alias CombatTypeParams
---| 0 # COMBAT_PARAM_TYPE
---| 1 # COMBAT_PARAM_EFFECT
---| 2 # COMBAT_PARAM_DISTANCEEFFECT
---| 3 # COMBAT_PARAM_BLOCKSHIELD
---| 4 # COMBAT_PARAM_BLOCKARMOR
---| 5 # COMBAT_PARAM_TARGETCASTERORTOPMOST
---| 6 # COMBAT_PARAM_CREATEITEM
---| 7 # COMBAT_PARAM_AGGRESSIVE
---| 8 # COMBAT_PARAM_DISPEL
---| 9 # COMBAT_PARAM_USECHARGES

---@source ../src/enums.h:211
COMBAT_PARAM_TYPE = 0
---@source ../src/enums.h:211
COMBAT_PARAM_EFFECT = 1
---@source ../src/enums.h:211
COMBAT_PARAM_DISTANCEEFFECT = 2
---@source ../src/enums.h:211
COMBAT_PARAM_BLOCKSHIELD = 3
---@source ../src/enums.h:211
COMBAT_PARAM_BLOCKARMOR = 4
---@source ../src/enums.h:211
COMBAT_PARAM_TARGETCASTERORTOPMOST = 5
---@source ../src/enums.h:211
COMBAT_PARAM_CREATEITEM = 6
---@source ../src/enums.h:211
COMBAT_PARAM_AGGRESSIVE = 7
---@source ../src/enums.h:211
COMBAT_PARAM_DISPEL = 8
---@source ../src/enums.h:211
COMBAT_PARAM_USECHARGES = 9


---@alias CombatTypes
---| 0 # COMBAT_NONE
---| 1 # COMBAT_PHYSICALDAMAGE
---| 2 # COMBAT_ENERGYDAMAGE
---| 4 # COMBAT_EARTHDAMAGE
---| 8 # COMBAT_FIREDAMAGE
---| 16 # COMBAT_UNDEFINEDDAMAGE
---| 32 # COMBAT_LIFEDRAIN
---| 64 # COMBAT_MANADRAIN
---| 128 # COMBAT_HEALING
---| 256 # COMBAT_DROWNDAMAGE
---| 512 # COMBAT_ICEDAMAGE
---| 1024 # COMBAT_HOLYDAMAGE
---| 2048 # COMBAT_DEATHDAMAGE

---@source ../src/enums.h:191
COMBAT_NONE = 0
---@source ../src/enums.h:191
COMBAT_PHYSICALDAMAGE = 1 * 2 ^ 0
---@source ../src/enums.h:191
COMBAT_ENERGYDAMAGE = 1 * 2 ^ 1
---@source ../src/enums.h:191
COMBAT_EARTHDAMAGE = 1 * 2 ^ 2
---@source ../src/enums.h:191
COMBAT_FIREDAMAGE = 1 * 2 ^ 3
---@source ../src/enums.h:191
COMBAT_UNDEFINEDDAMAGE = 1 * 2 ^ 4
---@source ../src/enums.h:191
COMBAT_LIFEDRAIN = 1 * 2 ^ 5
---@source ../src/enums.h:191
COMBAT_MANADRAIN = 1 * 2 ^ 6
---@source ../src/enums.h:191
COMBAT_HEALING = 1 * 2 ^ 7
---@source ../src/enums.h:191
COMBAT_DROWNDAMAGE = 1 * 2 ^ 8
---@source ../src/enums.h:191
COMBAT_ICEDAMAGE = 1 * 2 ^ 9
---@source ../src/enums.h:191
COMBAT_HOLYDAMAGE = 1 * 2 ^ 10
---@source ../src/enums.h:191
COMBAT_DEATHDAMAGE = 1 * 2 ^ 11

COMBAT_COUNT = 12

---@alias CombatFormulas
---| 0 # COMBAT_FORMULA_UNDEFINED
---| 1 # COMBAT_FORMULA_LEVELMAGIC
---| 2 # COMBAT_FORMULA_SKILL
---| 3 # COMBAT_FORMULA_DAMAGE

---@source ../src/enums.h:342
COMBAT_FORMULA_UNDEFINED = 0
---@source ../src/enums.h:342
COMBAT_FORMULA_LEVELMAGIC = 1
---@source ../src/enums.h:342
COMBAT_FORMULA_SKILL = 2
---@source ../src/enums.h:342
COMBAT_FORMULA_DAMAGE = 3

---@alias AccountTypes
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

---@alias CallbackParams
---| 0 # CALLBACK_PARAM_LEVELMAGICVALUE
---| 1 # CALLBACK_PARAM_SKILLVALUE
---| 2 # CALLBACK_PARAM_TARGETTILE
---| 3 # CALLBACK_PARAM_TARGETCREATURE

---@source ../src/enums.h:225
CALLBACK_PARAM_LEVELMAGICVALUE = 0
---@source ../src/enums.h:225
CALLBACK_PARAM_SKILLVALUE = 1
---@source ../src/enums.h:225
CALLBACK_PARAM_TARGETTILE = 2
---@source ../src/enums.h:225
CALLBACK_PARAM_TARGETCREATURE = 3

---@enum ExperienceRateType
ExperienceRateType = {BASE = 1, LOW_LEVEL = 2, BONUS = 3, STAMINA = 4}

GUEST_LIST = 0x100
SUBOWNER_LIST = 0x101

---@alias PodiumShowTypes
---| 0 # PODIUM_SHOW_PLATFORM
---| 1 # PODIUM_SHOW_OUTFIT
---| 2 # PODIUM_SHOW_MOUNT

---@source ../src/const.h:657
PODIUM_SHOW_PLATFORM = 0 -- show the platform below the outfit
---@source ../src/const.h:657
PODIUM_SHOW_OUTFIT = 1   -- show outfit
---@source ../src/const.h:657
PODIUM_SHOW_MOUNT = 2     -- show mount

---@alias Directions
---| 0 # DIRECTION_NORTH
---| 1 # DIRECTION_EAST
---| 2 # DIRECTION_SOUTH
---| 3 # DIRECTION_WEST
---| 4 # DIRECTION_SOUTHWEST
---| 5 # DIRECTION_SOUTHEAST
---| 6 # DIRECTION_NORTHWEST
---| 7 # DIRECTION_NORTHEAST
---| 7 # DIRECTION_LAST
---| 8 # DIRECTION_NONE

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

---@alias Skulls
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

---@alias MessageTypes
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

---@alias CylinderFlag
---| 1 # FLAG_NOLIMIT
---| 2 # FLAG_IGNOREBLOCKITEM
---| 4 # FLAG_IGNOREBLOCKCREATURE
---| 8 # FLAG_CHILDISOWNER
---| 16 # FLAG_PATHFINDING
---| 32 # FLAG_IGNOREFIELDDAMAGE
---| 64 # FLAG_IGNORENOTMOVEABLE
---| 128 # FLAG_IGNOREAUTOSTACK

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
---@alias ReturnValues
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

---@alias ConditionType
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

---@source ../src/enums.h:350
CONDITION_NONE = 0
---@source ../src/enums.h:350
CONDITION_POISON = 1 * 2 ^ 0
---@source ../src/enums.h:350
CONDITION_FIRE = 1 * 2 ^ 1
---@source ../src/enums.h:350
CONDITION_ENERGY = 1 * 2 ^ 2
---@source ../src/enums.h:350
CONDITION_BLEEDING = 1 * 2 ^ 3
---@source ../src/enums.h:350
CONDITION_HASTE = 1 * 2 ^ 4
---@source ../src/enums.h:350
CONDITION_PARALYZE = 1 * 2 ^ 5
---@source ../src/enums.h:350
CONDITION_OUTFIT = 1 * 2 ^ 6
---@source ../src/enums.h:350
CONDITION_INVISIBLE = 1 * 2 ^ 7
---@source ../src/enums.h:350
CONDITION_LIGHT = 1 * 2 ^ 8
---@source ../src/enums.h:350
CONDITION_MANASHIELD = 1 * 2 ^ 9
---@source ../src/enums.h:350
CONDITION_INFIGHT = 1 * 2 ^ 10
---@source ../src/enums.h:350
CONDITION_DRUNK = 1 * 2 ^ 11
---@source ../src/enums.h:350
CONDITION_EXHAUST_WEAPON = 1 * 2 ^ 12 -- unused
---@source ../src/enums.h:350
CONDITION_REGENERATION = 1 * 2 ^ 13
---@source ../src/enums.h:350
CONDITION_SOUL = 1 * 2 ^ 14
---@source ../src/enums.h:350
CONDITION_DROWN = 1 * 2 ^ 15
---@source ../src/enums.h:350
CONDITION_MUTED = 1 * 2 ^ 16
---@source ../src/enums.h:350
CONDITION_CHANNELMUTEDTICKS = 1 * 2 ^ 17
---@source ../src/enums.h:350
CONDITION_YELLTICKS = 1 * 2 ^ 18
---@source ../src/enums.h:350
CONDITION_ATTRIBUTES = 1 * 2 ^ 19
---@source ../src/enums.h:350
CONDITION_FREEZING = 1 * 2 ^ 20
---@source ../src/enums.h:350
CONDITION_DAZZLED = 1 * 2 ^ 21
---@source ../src/enums.h:350
CONDITION_CURSED = 1 * 2 ^ 22
---@source ../src/enums.h:350
CONDITION_EXHAUST_COMBAT = 1 * 2 ^ 23 -- unused
---@source ../src/enums.h:350
CONDITION_EXHAUST_HEAL = 1 * 2 ^ 24   -- unused
---@source ../src/enums.h:350
CONDITION_PACIFIED = 1 * 2 ^ 25
---@source ../src/enums.h:350
CONDITION_SPELLCOOLDOWN = 1 * 2 ^ 26
---@source ../src/enums.h:350
CONDITION_SPELLGROUPCOOLDOWN = 1 * 2 ^ 27
---@source ../src/enums.h:350
CONDITION_ROOT = 1 * 2 ^ 28
---@source ../src/enums.h:350
CONDITION_MANASHIELD_BREAKABLE = 1 * 2 ^ 29

---@alias ConditionParam
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

---@source ../src/enums.h:233
CONDITION_PARAM_OWNER = 1
---@source ../src/enums.h:233
CONDITION_PARAM_TICKS = 2
---@source ../src/enums.h:233
-- CONDITION_PARAM_OUTFIT = 3
---@source ../src/enums.h:233
CONDITION_PARAM_HEALTHGAIN = 4
---@source ../src/enums.h:233
CONDITION_PARAM_HEALTHTICKS = 5
---@source ../src/enums.h:233
CONDITION_PARAM_MANAGAIN = 6
---@source ../src/enums.h:233
CONDITION_PARAM_MANATICKS = 7
---@source ../src/enums.h:233
CONDITION_PARAM_DELAYED = 8
---@source ../src/enums.h:233
CONDITION_PARAM_SPEED = 9
---@source ../src/enums.h:233
CONDITION_PARAM_LIGHT_LEVEL = 10
---@source ../src/enums.h:233
CONDITION_PARAM_LIGHT_COLOR = 11
---@source ../src/enums.h:233
CONDITION_PARAM_SOULGAIN = 12
---@source ../src/enums.h:233
CONDITION_PARAM_SOULTICKS = 13
---@source ../src/enums.h:233
CONDITION_PARAM_MINVALUE = 14
---@source ../src/enums.h:233
CONDITION_PARAM_MAXVALUE = 15
---@source ../src/enums.h:233
CONDITION_PARAM_STARTVALUE = 16
---@source ../src/enums.h:233
CONDITION_PARAM_TICKINTERVAL = 17
---@source ../src/enums.h:233
CONDITION_PARAM_FORCEUPDATE = 18
---@source ../src/enums.h:233
CONDITION_PARAM_SKILL_MELEE = 19
---@source ../src/enums.h:233
CONDITION_PARAM_SKILL_FIST = 20
---@source ../src/enums.h:233
CONDITION_PARAM_SKILL_CLUB = 21
---@source ../src/enums.h:233
CONDITION_PARAM_SKILL_SWORD = 22
---@source ../src/enums.h:233
CONDITION_PARAM_SKILL_AXE = 23
---@source ../src/enums.h:233
CONDITION_PARAM_SKILL_DISTANCE = 24
---@source ../src/enums.h:233
CONDITION_PARAM_SKILL_SHIELD = 25
---@source ../src/enums.h:233
CONDITION_PARAM_SKILL_FISHING = 26
---@source ../src/enums.h:233
CONDITION_PARAM_STAT_MAXHITPOINTS = 27
---@source ../src/enums.h:233
CONDITION_PARAM_STAT_MAXMANAPOINTS = 28
---@source ../src/enums.h:233
-- CONDITION_PARAM_STAT_SOULPOINTS = 29
---@source ../src/enums.h:233
CONDITION_PARAM_STAT_MAGICPOINTS = 30
---@source ../src/enums.h:233
CONDITION_PARAM_STAT_MAXHITPOINTSPERCENT = 31
---@source ../src/enums.h:233
CONDITION_PARAM_STAT_MAXMANAPOINTSPERCENT = 32
---@source ../src/enums.h:233
-- CONDITION_PARAM_STAT_SOULPOINTSPERCENT = 33
CONDITION_PARAM_STAT_MAGICPOINTSPERCENT = 34
---@source ../src/enums.h:233
CONDITION_PARAM_PERIODICDAMAGE = 35
---@source ../src/enums.h:233
CONDITION_PARAM_SKILL_MELEEPERCENT = 36
---@source ../src/enums.h:233
CONDITION_PARAM_SKILL_FISTPERCENT = 37
---@source ../src/enums.h:233
CONDITION_PARAM_SKILL_CLUBPERCENT = 38
---@source ../src/enums.h:233
CONDITION_PARAM_SKILL_SWORDPERCENT = 39
---@source ../src/enums.h:233
CONDITION_PARAM_SKILL_AXEPERCENT = 40
---@source ../src/enums.h:233
CONDITION_PARAM_SKILL_DISTANCEPERCENT = 41
---@source ../src/enums.h:233
CONDITION_PARAM_SKILL_SHIELDPERCENT = 42
---@source ../src/enums.h:233
CONDITION_PARAM_SKILL_FISHINGPERCENT = 43
---@source ../src/enums.h:233
CONDITION_PARAM_BUFF_SPELL = 44
---@source ../src/enums.h:233
CONDITION_PARAM_SUBID = 45
---@source ../src/enums.h:233
CONDITION_PARAM_FIELD = 46
---@source ../src/enums.h:233
CONDITION_PARAM_DISABLE_DEFENSE = 47
---@source ../src/enums.h:233
CONDITION_PARAM_SPECIALSKILL_CRITICALHITCHANCE = 48
---@source ../src/enums.h:233
CONDITION_PARAM_SPECIALSKILL_CRITICALHITAMOUNT = 49
---@source ../src/enums.h:233
CONDITION_PARAM_SPECIALSKILL_LIFELEECHCHANCE = 50
---@source ../src/enums.h:233
CONDITION_PARAM_SPECIALSKILL_LIFELEECHAMOUNT = 51
---@source ../src/enums.h:233
CONDITION_PARAM_SPECIALSKILL_MANALEECHCHANCE = 52
---@source ../src/enums.h:233
CONDITION_PARAM_SPECIALSKILL_MANALEECHAMOUNT = 53
---@source ../src/enums.h:233
CONDITION_PARAM_AGGRESSIVE = 54
---@source ../src/enums.h:233
CONDITION_PARAM_DRUNKENNESS = 55
---@source ../src/enums.h:233
CONDITION_PARAM_MANASHIELD_BREAKABLE = 56


---@alias Icons
---| 0 # ICON_NONE
---| 1 # ICON_POISON
---| 2 # ICON_BURN
---| 4 # ICON_ENERGY
---| 8 # ICON_DRUNK
---| 16 # ICON_MANASHIELD
---| 32 # ICON_PARALYZE
---| 64 # ICON_HASTE
---| 128 # ICON_SWORDS
---| 256 # ICON_DROWNING
---| 512 # ICON_FREEZING
---| 1024 # ICON_DAZZLED
---| 2048 # ICON_CURSED
---| 4096 # ICON_PARTY_BUFF
---| 8192 # ICON_REDSWORDS
---| 16384 # ICON_PIGEON
---| 32768 # ICON_BLEEDING
---| 65536 # ICON_LESSERHEX
---| 131072 # ICON_INTENSEHEX
---| 262144 # ICON_GREATERHEX
---| 524288 # ICON_ROOT
---| 1048576 # ICON_FEAR
---| 2097152 # ICON_GOSHNAR1
---| 4194304 # ICON_GOSHNAR2
---| 8388608 # ICON_GOSHNAR3
---| 16777216 # ICON_GOSHNAR4
---| 33554432 # ICON_GOSHNAR5
---| 67108864 # ICON_MANASHIELD_BREAKABLE

ICON_POISON = 1
ICON_BURN = 2
ICON_ENERGY = 4
ICON_DRUNK = 8
ICON_MANASHIELD = 16
ICON_PARALYZE = 32
ICON_HASTE = 64
ICON_SWORDS = 128
ICON_DROWNING = 256
ICON_FREEZING = 512
ICON_DAZZLED = 1024
ICON_CURSED = 2048
ICON_PARTY_BUFF = 4096
ICON_REDSWORDS = 8192
ICON_PIGEON = 16384
ICON_BLEEDING = 32768
ICON_LESSERHEX = 65536
ICON_INTENSEHEX = 131072
ICON_GREATERHEX = 262144
ICON_ROOT = 524288
ICON_FEAR = 1048576
ICON_GOSHNAR1 = 2097152
ICON_GOSHNAR2 = 4194304
ICON_GOSHNAR3 = 8388608
ICON_GOSHNAR4 = 16777216
ICON_GOSHNAR5 = 33554432
ICON_MANASHIELD_BREAKABLE = 67108864

---@alias WeaponType
---| 0 # WEAPON_NONE
---| 1 # WEAPON_SWORD
---| 2 # WEAPON_CLUB
---| 3 # WEAPON_AXE
---| 4 # WEAPON_SHIELD
---| 5 # WEAPON_DISTANCE
---| 6 # WEAPON_WAND
---| 7 # WEAPON_AMMO
---| 8 # WEAPON_QUIVER

---@source ../src/const.h:441
WEAPON_NONE = 0
---@source ../src/const.h:441
WEAPON_SWORD = 1
---@source ../src/const.h:441
WEAPON_CLUB = 2
---@source ../src/const.h:441
WEAPON_AXE = 3
---@source ../src/const.h:441
WEAPON_SHIELD = 4
---@source ../src/const.h:441
WEAPON_DISTANCE = 5
---@source ../src/const.h:441
WEAPON_WAND = 6
---@source ../src/const.h:441
WEAPON_AMMO = 7
---@source ../src/const.h:441
WEAPON_QUIVER = 8

---@alias SlotPositions
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

---@source ../src/items.h:14
SLOTP_WHEREEVER = 0xFFFFFFFF
---@source ../src/items.h:14
SLOTP_HEAD = 1 * 2 ^ 0
---@source ../src/items.h:14
SLOTP_NECKLACE = 1 * 2 ^ 1
---@source ../src/items.h:14
SLOTP_BACKPACK = 1 * 2 ^ 2
---@source ../src/items.h:14
SLOTP_ARMOR = 1 * 2 ^ 3
---@source ../src/items.h:14
SLOTP_RIGHT = 1 * 2 ^ 4
---@source ../src/items.h:14
SLOTP_LEFT = 1 * 2 ^ 5
---@source ../src/items.h:14
SLOTP_LEGS = 1 * 2 ^ 6
---@source ../src/items.h:14
SLOTP_FEET = 1 * 2 ^ 7
---@source ../src/items.h:14
SLOTP_RING = 1 * 2 ^ 8
---@source ../src/items.h:14
SLOTP_AMMO = 1 * 2 ^ 9
---@source ../src/items.h:14
SLOTP_DEPOT = 1 * 2 ^ 10
---@source ../src/items.h:14
SLOTP_TWO_HAND = 1 * 2 ^ 11
---@source ../src/items.h:14
SLOTP_HAND = SLOTP_LEFT or SLOTP_RIGHT

---@alias DamageOrigin
---| 0 # ORIGIN_NONE
---| 1 # ORIGIN_CONDITION
---| 2 # ORIGIN_SPELL
---| 3 # ORIGIN_MELEE
---| 4 # ORIGIN_RANGED
---| 5 # ORIGIN_WAND
---| 6 # ORIGIN_REFLECT

---@source ../src/enums.h:629
ORIGIN_NONE = 0
---@source ../src/enums.h:629
ORIGIN_CONDITION = 1
---@source ../src/enums.h:629
ORIGIN_SPELL = 2
---@source ../src/enums.h:629
ORIGIN_MELEE = 3
---@source ../src/enums.h:629
ORIGIN_RANGED = 4
---@source ../src/enums.h:629
ORIGIN_WAND = 5
---@source ../src/enums.h:629
ORIGIN_REFLECT = 6

---@alias PlayerSexes
---| 0 # PLAYERSEX_FEMALE
---| 1 # PLAYERSEX_MALE

---@source ../src/enums.h:402
PLAYERSEX_FEMALE = 0
---@source ../src/enums.h:402
PLAYERSEX_MALE = 1

---@alias ConditionIds
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

---@source ../src/enums.h:386
CONDITIONID_DEFAULT = -1
---@source ../src/enums.h:386
CONDITIONID_COMBAT = 0
---@source ../src/enums.h:386
CONDITIONID_HEAD = 1
---@source ../src/enums.h:386
CONDITIONID_NECKLACE = 2
---@source ../src/enums.h:386
CONDITIONID_BACKPACK = 3
---@source ../src/enums.h:386
CONDITIONID_ARMOR = 4
---@source ../src/enums.h:386
CONDITIONID_RIGHT = 5
---@source ../src/enums.h:386
CONDITIONID_LEFT = 6
---@source ../src/enums.h:386
CONDITIONID_LEGS = 7
---@source ../src/enums.h:386
CONDITIONID_FEET = 8
---@source ../src/enums.h:386
CONDITIONID_RING = 9
---@source ../src/enums.h:386
CONDITIONID_AMMO = 10

---@alias MapMarks
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

---@source ../src/enums.h:502
MAPMARK_TICK = 0
---@source ../src/enums.h:502
MAPMARK_QUESTION = 1
---@source ../src/enums.h:502
MAPMARK_EXCLAMATION = 2
---@source ../src/enums.h:502
MAPMARK_STAR = 3
---@source ../src/enums.h:502
MAPMARK_CROSS = 4
---@source ../src/enums.h:502
MAPMARK_TEMPLE = 5
---@source ../src/enums.h:502
MAPMARK_KISS = 6
---@source ../src/enums.h:502
MAPMARK_SHOVEL = 7
---@source ../src/enums.h:502
MAPMARK_SWORD = 8
---@source ../src/enums.h:502
MAPMARK_FLAG = 9
---@source ../src/enums.h:502
MAPMARK_LOCK = 10
---@source ../src/enums.h:502
MAPMARK_BAG = 11
---@source ../src/enums.h:502
MAPMARK_SKULL = 12
---@source ../src/enums.h:502
MAPMARK_DOLLAR = 13
---@source ../src/enums.h:502
MAPMARK_REDNORTH = 14
---@source ../src/enums.h:502
MAPMARK_REDSOUTH = 15
---@source ../src/enums.h:502
MAPMARK_REDEAST = 16
---@source ../src/enums.h:502
MAPMARK_REDWEST = 17
---@source ../src/enums.h:502
MAPMARK_GREENNORTH = 18
---@source ../src/enums.h:502
MAPMARK_GREENSOUTH = 19

---@alias ReloadTypes
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

---@alias PlayerFlags
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

---@source ../src/const.h:613
PlayerFlag_CannotUseCombat = 1 * 2 ^ 0
---@source ../src/const.h:613
PlayerFlag_CannotAttackPlayer = 1 * 2 ^ 1
---@source ../src/const.h:613
PlayerFlag_CannotAttackMonster = 1 * 2 ^ 2
---@source ../src/const.h:613
PlayerFlag_CannotBeAttacked = 1 * 2 ^ 3
---@source ../src/const.h:613
PlayerFlag_CanConvinceAll = 1 * 2 ^ 4
---@source ../src/const.h:613
PlayerFlag_CanSummonAll = 1 * 2 ^ 5
---@source ../src/const.h:613
PlayerFlag_CanIllusionAll = 1 * 2 ^ 6
---@source ../src/const.h:613
PlayerFlag_CanSenseInvisibility = 1 * 2 ^ 7
---@source ../src/const.h:613
PlayerFlag_IgnoredByMonsters = 1 * 2 ^ 8
---@source ../src/const.h:613
PlayerFlag_NotGainInFight = 1 * 2 ^ 9
---@source ../src/const.h:613
PlayerFlag_HasInfiniteMana = 1 * 2 ^ 10
---@source ../src/const.h:613
PlayerFlag_HasInfiniteSoul = 1 * 2 ^ 11
---@source ../src/const.h:613
PlayerFlag_HasNoExhaustion = 1 * 2 ^ 12
---@source ../src/const.h:613
PlayerFlag_CannotUseSpells = 1 * 2 ^ 13
---@source ../src/const.h:613
PlayerFlag_CannotPickupItem = 1 * 2 ^ 14
---@source ../src/const.h:613
PlayerFlag_CanAlwaysLogin = 1 * 2 ^ 15
---@source ../src/const.h:613
PlayerFlag_CanBroadcast = 1 * 2 ^ 16
---@source ../src/const.h:613
PlayerFlag_CanEditHouses = 1 * 2 ^ 17
---@source ../src/const.h:613
PlayerFlag_CannotBeBanned = 1 * 2 ^ 18
---@source ../src/const.h:613
PlayerFlag_CannotBePushed = 1 * 2 ^ 19
---@source ../src/const.h:613
PlayerFlag_HasInfiniteCapacity = 1 * 2 ^ 20
---@source ../src/const.h:613
PlayerFlag_CanPushAllCreatures = 1 * 2 ^ 21
---@source ../src/const.h:613
PlayerFlag_CanTalkRedPrivate = 1 * 2 ^ 22
---@source ../src/const.h:613
PlayerFlag_CanTalkRedChannel = 1 * 2 ^ 23
---@source ../src/const.h:613
PlayerFlag_TalkOrangeHelpChannel = 1 * 2 ^ 24
---@source ../src/const.h:613
PlayerFlag_NotGainExperience = 1 * 2 ^ 25
---@source ../src/const.h:613
PlayerFlag_NotGainMana = 1 * 2 ^ 26
---@source ../src/const.h:613
PlayerFlag_NotGainHealth = 1 * 2 ^ 27
---@source ../src/const.h:613
PlayerFlag_NotGainSkill = 1 * 2 ^ 28
---@source ../src/const.h:613
PlayerFlag_SetMaxSpeed = 1 * 2 ^ 29
---@source ../src/const.h:613
PlayerFlag_SpecialVIP = 1 * 2 ^ 30
---@source ../src/const.h:613
PlayerFlag_NotGenerateLoot = 1 * 2 ^ 31
---@source ../src/const.h:613
-- PlayerFlag with exponent 32 existed but was deprecated (feature dropped from client).
---@source ../src/const.h:613
PlayerFlag_IgnoreProtectionZone = 1 * 2 ^ 33
---@source ../src/const.h:613
PlayerFlag_IgnoreSpellCheck = 1 * 2 ^ 34
---@source ../src/const.h:613
PlayerFlag_IgnoreWeaponCheck = 1 * 2 ^ 35
---@source ../src/const.h:613
PlayerFlag_CannotBeMuted = 1 * 2 ^ 36
---@source ../src/const.h:613
PlayerFlag_IsAlwaysPremium = 1 * 2 ^ 37
---@source ../src/const.h:613
PlayerFlag_IgnoreYellCheck = 1 * 2 ^ 38
---@source ../src/const.h:613
PlayerFlag_IgnoreSendPrivateCheck = 1 * 2 ^ 39

---@alias ItemAttributes
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
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_ACTIONID = 1 * 2 ^ 0
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_UNIQUEID = 1 * 2 ^ 1
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_DESCRIPTION = 1 * 2 ^ 2
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_TEXT = 1 * 2 ^ 3
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_DATE = 1 * 2 ^ 4
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_WRITER = 1 * 2 ^ 5
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_NAME = 1 * 2 ^ 6
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_ARTICLE = 1 * 2 ^ 7
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_PLURALNAME = 1 * 2 ^ 8
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_WEIGHT = 1 * 2 ^ 9
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_ATTACK = 1 * 2 ^ 10
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_DEFENSE = 1 * 2 ^ 11
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_EXTRADEFENSE = 1 * 2 ^ 12
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_ARMOR = 1 * 2 ^ 13
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_HITCHANCE = 1 * 2 ^ 14
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_SHOOTRANGE = 1 * 2 ^ 15
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_OWNER = 1 * 2 ^ 16
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_DURATION = 1 * 2 ^ 17
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_DECAYSTATE = 1 * 2 ^ 18
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_CORPSEOWNER = 1 * 2 ^ 19
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_CHARGES = 1 * 2 ^ 20
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_FLUIDTYPE = 1 * 2 ^ 21
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_DOORID = 1 * 2 ^ 22
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_DECAYTO = 1 * 2 ^ 23
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_WRAPID = 1 * 2 ^ 24
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_STOREITEM = 1 * 2 ^ 25
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_ATTACK_SPEED = 1 * 2 ^ 26
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_OPENCONTAINER = 1 * 2 ^ 27
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_DURATION_MIN = ITEM_ATTRIBUTE_DURATION
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_DURATION_MAX = 1 * 2 ^ 27
---@source ../src/enums.h:48
ITEM_ATTRIBUTE_CUSTOM = 1 * 2 ^ 31

---@alias SpecialItems
---| 460 = 'ITEM_BROWSEFIELD'
---| 26054 = 'ITEM_DECORATION_KIT'
---| 1487 = 'ITEM_FIREFIELD_PVP_FULL'
---| 1488 = 'ITEM_FIREFIELD_PVP_MEDIUM'
---| 1489 = 'ITEM_FIREFIELD_PVP_SMALL'
---| 1492 = 'ITEM_FIREFIELD_PERSISTENT_FULL'
---| 1493 = 'ITEM_FIREFIELD_PERSISTENT_MEDIUM'
---| 1494 = 'ITEM_FIREFIELD_PERSISTENT_SMALL'
---| 1500 = 'ITEM_FIREFIELD_NOPVP'
---| 1501 = 'ITEM_FIREFIELD_NOPVP_MEDIUM'
---| 1490 = 'ITEM_POISONFIELD_PVP'
---| 1496 = 'ITEM_POISONFIELD_PERSISTENT'
---| 1503 = 'ITEM_POISONFIELD_NOPVP'
---| 1491 = 'ITEM_ENERGYFIELD_PVP'
---| 1495 = 'ITEM_ENERGYFIELD_PERSISTENT'
---| 1504 = 'ITEM_ENERGYFIELD_NOPVP'
---| 1497 = 'ITEM_MAGICWALL'
---| 1498 = 'ITEM_MAGICWALL_PERSISTENT'
---| 11098 = 'ITEM_MAGICWALL_SAFE'
---| 20669 = 'ITEM_MAGICWALL_NOPVP'
---| 1499 = 'ITEM_WILDGROWTH'
---| 2721 = 'ITEM_WILDGROWTH_PERSISTENT'
---| 11099 = 'ITEM_WILDGROWTH_SAFE'
---| 20670 = 'ITEM_WILDGROWTH_NOPVP'
---| 1987 = 'ITEM_BAG'
---| 23782 = 'ITEM_SHOPPING_BAG'
---| 2148 = 'ITEM_GOLD_COIN'
---| 2152 = 'ITEM_PLATINUM_COIN'
---| 2160 = 'ITEM_CRYSTAL_COIN'
---| 24774 = 'ITEM_STORE_COIN'
---| 2594 = 'ITEM_DEPOT'
---| 2589 = 'ITEM_LOCKER'
---| 14404 = 'ITEM_INBOX'
---| 14405 = 'ITEM_MARKET'
---| 26052 = 'ITEM_STORE_INBOX'
---| 25453 = 'ITEM_DEPOT_BOX_I'
---| 25454 = 'ITEM_DEPOT_BOX_II'
---| 25455 = 'ITEM_DEPOT_BOX_III'
---| 25456 = 'ITEM_DEPOT_BOX_IV'
---| 25457 = 'ITEM_DEPOT_BOX_V'
---| 25458 = 'ITEM_DEPOT_BOX_VI'
---| 25459 = 'ITEM_DEPOT_BOX_VII'
---| 25460 = 'ITEM_DEPOT_BOX_VIII'
---| 25461 = 'ITEM_DEPOT_BOX_IX'
---| 25462 = 'ITEM_DEPOT_BOX_X'
---| 25463 = 'ITEM_DEPOT_BOX_XI'
---| 25464 = 'ITEM_DEPOT_BOX_XII'
---| 25465 = 'ITEM_DEPOT_BOX_XIII'
---| 25466 = 'ITEM_DEPOT_BOX_XIV'
---| 25467 = 'ITEM_DEPOT_BOX_XV'
---| 25468 = 'ITEM_DEPOT_BOX_XVI'
---| 25469 = 'ITEM_DEPOT_BOX_XVII'
---| 34571 = 'ITEM_DEPOT_BOX_XVIII'
---| 44714 = 'ITEM_DEPOT_BOX_XIX'
---| 44715 = 'ITEM_DEPOT_BOX_XX'
---| 3058 = 'ITEM_MALE_CORPSE'
---| 3065 = 'ITEM_FEMALE_CORPSE'
---| 2016 = 'ITEM_FULLSPLASH'
---| 2019 = 'ITEM_SMALLSPLASH'
---| 2595 = 'ITEM_PARCEL'
---| 2597 = 'ITEM_LETTER'
---| 2598 = 'ITEM_LETTER_STAMPED'
---| 2599 = 'ITEM_LABEL'
---| 2173 = 'ITEM_AMULETOFLOSS'
---| 1968 = 'ITEM_DOCUMENT_RO'

---@source ../src/const.h:520
ITEM_BROWSEFIELD = 460 -- for internal use
---@source ../src/const.h:520
ITEM_DECORATION_KIT = 26054
---@source ../src/const.h:520
ITEM_FIREFIELD_PVP_FULL = 1487
---@source ../src/const.h:520
ITEM_FIREFIELD_PVP_MEDIUM = 1488
---@source ../src/const.h:520
ITEM_FIREFIELD_PVP_SMALL = 1489
---@source ../src/const.h:520
ITEM_FIREFIELD_PERSISTENT_FULL = 1492
---@source ../src/const.h:520
ITEM_FIREFIELD_PERSISTENT_MEDIUM = 1493
---@source ../src/const.h:520
ITEM_FIREFIELD_PERSISTENT_SMALL = 1494
---@source ../src/const.h:520
ITEM_FIREFIELD_NOPVP = 1500
---@source ../src/const.h:520
ITEM_FIREFIELD_NOPVP_MEDIUM = 1501
---@source ../src/const.h:520
ITEM_POISONFIELD_PVP = 1490
---@source ../src/const.h:520
ITEM_POISONFIELD_PERSISTENT = 1496
---@source ../src/const.h:520
ITEM_POISONFIELD_NOPVP = 1503
---@source ../src/const.h:520
ITEM_ENERGYFIELD_PVP = 1491
---@source ../src/const.h:520
ITEM_ENERGYFIELD_PERSISTENT = 1495
---@source ../src/const.h:520
ITEM_ENERGYFIELD_NOPVP = 1504
---@source ../src/const.h:520
ITEM_MAGICWALL = 1497
---@source ../src/const.h:520
ITEM_MAGICWALL_PERSISTENT = 1498
---@source ../src/const.h:520
ITEM_MAGICWALL_SAFE = 11098
---@source ../src/const.h:520
ITEM_MAGICWALL_NOPVP = 20669
---@source ../src/const.h:520
ITEM_WILDGROWTH = 1499
---@source ../src/const.h:520
ITEM_WILDGROWTH_PERSISTENT = 2721
---@source ../src/const.h:520
ITEM_WILDGROWTH_SAFE = 11099
---@source ../src/const.h:520
ITEM_WILDGROWTH_NOPVP = 20670
---@source ../src/const.h:520
ITEM_BAG = 1987
---@source ../src/const.h:520
ITEM_SHOPPING_BAG = 23782
---@source ../src/const.h:520
ITEM_GOLD_COIN = 2148
---@source ../src/const.h:520
ITEM_PLATINUM_COIN = 2152
---@source ../src/const.h:520
ITEM_CRYSTAL_COIN = 2160
---@source ../src/const.h:520
ITEM_STORE_COIN = 24774
---@source ../src/const.h:520
ITEM_DEPOT = 2594
---@source ../src/const.h:520
ITEM_LOCKER = 2589
---@source ../src/const.h:520
ITEM_INBOX = 14404
---@source ../src/const.h:520
ITEM_MARKET = 14405
---@source ../src/const.h:520
ITEM_STORE_INBOX = 26052
---@source ../src/const.h:520
ITEM_DEPOT_BOX_I = 25453
---@source ../src/const.h:520
ITEM_DEPOT_BOX_II = 25454
---@source ../src/const.h:520
ITEM_DEPOT_BOX_III = 25455
---@source ../src/const.h:520
ITEM_DEPOT_BOX_IV = 25456
---@source ../src/const.h:520
ITEM_DEPOT_BOX_V = 25457
---@source ../src/const.h:520
ITEM_DEPOT_BOX_VI = 25458
---@source ../src/const.h:520
ITEM_DEPOT_BOX_VII = 25459
---@source ../src/const.h:520
ITEM_DEPOT_BOX_VIII = 25460
---@source ../src/const.h:520
ITEM_DEPOT_BOX_IX = 25461
---@source ../src/const.h:520
ITEM_DEPOT_BOX_X = 25462
---@source ../src/const.h:520
ITEM_DEPOT_BOX_XI = 25463
---@source ../src/const.h:520
ITEM_DEPOT_BOX_XII = 25464
---@source ../src/const.h:520
ITEM_DEPOT_BOX_XIII = 25465
---@source ../src/const.h:520
ITEM_DEPOT_BOX_XIV = 25466
---@source ../src/const.h:520
ITEM_DEPOT_BOX_XV = 25467
---@source ../src/const.h:520
ITEM_DEPOT_BOX_XVI = 25468
---@source ../src/const.h:520
ITEM_DEPOT_BOX_XVII = 25469
---@source ../src/const.h:520
ITEM_DEPOT_BOX_XVIII = 34571
---@source ../src/const.h:520
ITEM_DEPOT_BOX_XIX = 44714
---@source ../src/const.h:520
ITEM_DEPOT_BOX_XX = 44715
---@source ../src/const.h:520
ITEM_MALE_CORPSE = 3058
---@source ../src/const.h:520
ITEM_FEMALE_CORPSE = 3065
---@source ../src/const.h:520
ITEM_FULLSPLASH = 2016
---@source ../src/const.h:520
ITEM_SMALLSPLASH = 2019
---@source ../src/const.h:520
ITEM_PARCEL = 2595
---@source ../src/const.h:520
ITEM_LETTER = 2597
---@source ../src/const.h:520
ITEM_LETTER_STAMPED = 2598
---@source ../src/const.h:520
ITEM_LABEL = 2599
---@source ../src/const.h:520
ITEM_AMULETOFLOSS = 2173
---@source ../src/const.h:520
ITEM_DOCUMENT_RO = 1968

---@alias OperatingSystem
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

---@source ../src/enums.h:133
CLIENTOS_NONE = 0
---@source ../src/enums.h:133
CLIENTOS_LINUX = 1
---@source ../src/enums.h:133
CLIENTOS_WINDOWS = 2
---@source ../src/enums.h:133
CLIENTOS_FLASH = 3
---@source ../src/enums.h:133
CLIENTOS_QT_LINUX = 4
---@source ../src/enums.h:133
CLIENTOS_QT_WINDOWS = 5
---@source ../src/enums.h:133
CLIENTOS_QT_MAC = 6
---@source ../src/enums.h:133
CLIENTOS_QT_LINUX2 = 7
---@source ../src/enums.h:133
CLIENTOS_OTCLIENT_LINUX = 10
---@source ../src/enums.h:133
CLIENTOS_OTCLIENT_WINDOWS = 11
---@source ../src/enums.h:133
CLIENTOS_OTCLIENT_MAC = 12

---@alias SkillTypes
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
---| 8 # SKILL_LAST\

---@source ../src/enums.h:301
SKILL_FIST = 0
---@source ../src/enums.h:301
SKILL_CLUB = 1
---@source ../src/enums.h:301
SKILL_SWORD = 2
---@source ../src/enums.h:301
SKILL_AXE = 3
---@source ../src/enums.h:301
SKILL_DISTANCE = 4
---@source ../src/enums.h:301
SKILL_SHIELD = 5
---@source ../src/enums.h:301
SKILL_FISHING = 6
---@source ../src/enums.h:301
SKILL_MAGLEVEL = 7
---@source ../src/enums.h:301
SKILL_LEVEL = 8
---@source ../src/enums.h:301
SKILL_FIRST = SKILL_FIST
---@source ../src/enums.h:301
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

---@source ../src/items.h:32
---@alias ItemTypes
---| 0 # ITEM_TYPE_NONE
---| 1 # ITEM_TYPE_DEPOT
---| 2 # ITEM_TYPE_MAILBOX
---| 3 # ITEM_TYPE_TRASHHOLDER
---| 4 # ITEM_TYPE_CONTAINER
---| 5 # ITEM_TYPE_DOOR
---| 6 # ITEM_TYPE_MAGICFIELD
---| 7 # ITEM_TYPE_TELEPORT
---| 8 # ITEM_TYPE_BED
---| 9 # ITEM_TYPE_KEY
---| 10 # ITEM_TYPE_RUNE
---| 11 # ITEM_TYPE_PODIUM
---| 12 # ITEM_TYPE_LAST

---@source ../src/items.h:32
ITEM_TYPE_NONE = 0
---@source ../src/items.h:32
ITEM_TYPE_DEPOT = 1
---@source ../src/items.h:32
ITEM_TYPE_MAILBOX = 2
---@source ../src/items.h:32
ITEM_TYPE_TRASHHOLDER = 3
---@source ../src/items.h:32
ITEM_TYPE_CONTAINER = 4
---@source ../src/items.h:32
ITEM_TYPE_DOOR = 5
---@source ../src/items.h:32
ITEM_TYPE_MAGICFIELD = 6
---@source ../src/items.h:32
ITEM_TYPE_TELEPORT = 7
---@source ../src/items.h:32
ITEM_TYPE_BED = 8
---@source ../src/items.h:32
ITEM_TYPE_KEY = 9
---@source ../src/items.h:32
ITEM_TYPE_RUNE = 10
---@source ../src/items.h:32
ITEM_TYPE_PODIUM = 11
---@source ../src/items.h:32
ITEM_TYPE_LAST = 12

---@alias ItemGroups
---| 0 # ITEM_GROUP_NONE
---| 1 # ITEM_GROUP_GROUND
---| 2 # ITEM_GROUP_CONTAINER
---| 3 # ITEM_GROUP_WEAPON -- deprecated
---| 4 # ITEM_GROUP_AMMUNITION -- deprecated
---| 5 # ITEM_GROUP_ARMOR -- deprecated
---| 6 # ITEM_GROUP_CHARGES
---| 7 # ITEM_GROUP_TELEPORT -- deprecated
---| 8 # ITEM_GROUP_MAGICFIELD -- deprecated
---| 9 # ITEM_GROUP_WRITEABLE -- deprecated
---| 10 # ITEM_GROUP_KEY -- deprecated
---| 11 # ITEM_GROUP_SPLASH
---| 12 # ITEM_GROUP_FLUID
---| 13 # ITEM_GROUP_DOOR -- deprecated
---| 14 # ITEM_GROUP_DEPRECATED
---| 15 # ITEM_GROUP_PODIUM
---| 16 # ITEM_GROUP_LAST

---@source ../src/itemloader.h:9
ITEM_GROUP_NONE = 0
---@source ../src/itemloader.h:9
ITEM_GROUP_GROUND = 1
---@source ../src/itemloader.h:9
ITEM_GROUP_CONTAINER = 2
---@source ../src/itemloader.h:9
ITEM_GROUP_WEAPON = 3     -- deprecated
---@source ../src/itemloader.h:9
ITEM_GROUP_AMMUNITION = 4 -- deprecated
---@source ../src/itemloader.h:9
ITEM_GROUP_ARMOR = 5      -- deprecated
---@source ../src/itemloader.h:9
ITEM_GROUP_CHARGES = 6
---@source ../src/itemloader.h:9
ITEM_GROUP_TELEPORT = 7   -- deprecated
---@source ../src/itemloader.h:9
ITEM_GROUP_MAGICFIELD = 8 -- deprecated
---@source ../src/itemloader.h:9
ITEM_GROUP_WRITEABLE = 9  -- deprecated
---@source ../src/itemloader.h:9
ITEM_GROUP_KEY = 10       -- deprecated
---@source ../src/itemloader.h:9
ITEM_GROUP_SPLASH = 11
---@source ../src/itemloader.h:9
ITEM_GROUP_FLUID = 12
---@source ../src/itemloader.h:9
ITEM_GROUP_DOOR = 13      -- deprecated
---@source ../src/itemloader.h:9
ITEM_GROUP_DEPRECATED = 14
---@source ../src/itemloader.h:9
ITEM_GROUP_PODIUM = 15
---@source ../src/itemloader.h:9
ITEM_GROUP_LAST = 16

---@alias AmmoTypes
---| 0 # AMMO_NONE
---| 1 # AMMO_BOLT
---| 2 # AMMO_ARROW
---| 3 # AMMO_SPEAR
---| 4 # AMMO_THROWINGSTAR
---| 5 # AMMO_THROWINGKNIFE
---| 6 # AMMO_STONE
---| 7 # AMMO_SNOWBALL

---@source ../src/const.h:454
AMMO_NONE = 0
---@source ../src/const.h:454
AMMO_BOLT = 1
---@source ../src/const.h:454
AMMO_ARROW = 2
---@source ../src/const.h:454
AMMO_SPEAR = 3
---@source ../src/const.h:454
AMMO_THROWINGSTAR = 4
---@source ../src/const.h:454
AMMO_THROWINGKNIFE = 5
---@source ../src/const.h:454
AMMO_STONE = 6
---@source ../src/const.h:454
AMMO_SNOWBALL = 7

---@alias WieldInfo
---| 0 # WIELDINFO_NONE
---| 1 # WIELDINFO_LEVEL
---| 2 # WIELDINFO_MAGLV
---| 4 # WIELDINFO_VOCREQ
---| 8 # WIELDINFO_PREMIUM

---@source ../src/const.h:474
WIELDINFO_NONE = 0 * 2 ^ 0
---@source ../src/const.h:474
WIELDINFO_LEVEL = 1 * 2 ^ 0
---@source ../src/const.h:474
WIELDINFO_MAGLV = 1 * 2 ^ 1
---@source ../src/const.h:474
WIELDINFO_VOCREQ = 1 * 2 ^ 2
---@source ../src/const.h:474
WIELDINFO_PREMIUM = 1 * 2 ^ 3

---@alias SpeakClasses
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

-- Cannot be called in LUA
-- VARIANT_NUMBER = 0
-- VARIANT_POSITION = 1
-- VARIANT_TARGETPOSITION = 2
-- VARIANT_STRING = 3

---@alias SpecialSkills
---| 0 # SPECIALSKILL_CRITICALHITCHANCE
---| 1 # SPECIALSKILL_CRITICALHITAMOUNT
---| 2 # SPECIALSKILL_LIFELEECHCHANCE
---| 3 # SPECIALSKILL_LIFELEECHAMOUNT
---| 4 # SPECIALSKILL_MANALEECHCHANCE
---| 5 # SPECIALSKILL_MANALEECHAMOUNT
---| 0 # SPECIALSKILL_FIRST
---| 5 # SPECIALSKILL_LAST

---@source ../src/enums.h:329
SPECIALSKILL_CRITICALHITCHANCE = 0
---@source ../src/enums.h:329
SPECIALSKILL_CRITICALHITAMOUNT = 1
---@source ../src/enums.h:329
SPECIALSKILL_LIFELEECHCHANCE = 2
---@source ../src/enums.h:329
SPECIALSKILL_LIFELEECHAMOUNT = 3
---@source ../src/enums.h:329
SPECIALSKILL_MANALEECHCHANCE = 4
---@source ../src/enums.h:329
SPECIALSKILL_MANALEECHAMOUNT = 5
---@source ../src/enums.h:329
SPECIALSKILL_FIRST = SPECIALSKILL_CRITICALHITCHANCE
---@source ../src/enums.h:329
SPECIALSKILL_LAST = SPECIALSKILL_MANALEECHAMOUNT

---@alias SpeechBubbles
---| 0 # SPEECHBUBBLE_NONE
---| 1 # SPEECHBUBBLE_NORMAL
---| 2 # SPEECHBUBBLE_TRADE
---| 3 # SPEECHBUBBLE_QUEST
---| 4 # SPEECHBUBBLE_COMPASS
---| 5 # SPEECHBUBBLE_NORMAL2
---| 6 # SPEECHBUBBLE_NORMAL3
---| 7 # SPEECHBUBBLE_HIRELING
---| 7 # SPEECHBUBBLE_LAST

---@source ../src/enums.h:488
SPEECHBUBBLE_NONE = 0
---@source ../src/enums.h:488
SPEECHBUBBLE_NORMAL = 1
---@source ../src/enums.h:488
SPEECHBUBBLE_TRADE = 2
---@source ../src/enums.h:488
SPEECHBUBBLE_QUEST = 3
---@source ../src/enums.h:488
SPEECHBUBBLE_COMPASS = 4
---@source ../src/enums.h:488
SPEECHBUBBLE_NORMAL2 = 5
---@source ../src/enums.h:488
SPEECHBUBBLE_NORMAL3 = 6
---@source ../src/enums.h:488
SPEECHBUBBLE_HIRELING = 7
---@source ../src/enums.h:488
SPEECHBUBBLE_LAST = SPEECHBUBBLE_HIRELING

---@alias PlayerStats
---| 0 # STAT_MAXHITPOINTS
---| 1 # STAT_MAXMANAPOINTS
---| 2 # STAT_SOULPOINTS -- unused
---| 3 # STAT_MAGICPOINTS
---| 0 # STAT_FIRST
---| 3 # STAT_LAST

---@source ../src/enums.h:318
STAT_MAXHITPOINTS = 0
---@source ../src/enums.h:318
STAT_MAXMANAPOINTS = 1
---@source ../src/enums.h:318
STAT_SOULPOINTS = 2 -- unused
---@source ../src/enums.h:318
STAT_MAGICPOINTS = 3
---@source ../src/enums.h:318
STAT_FIRST = STAT_MAXHITPOINTS
---@source ../src/enums.h:318
STAT_LAST = STAT_MAGICPOINTS

-- Not accessible in LUA
-- MONSTERS_EVENT_NONE = 0
-- MONSTERS_EVENT_THINK = 1
-- MONSTERS_EVENT_APPEAR = 2
-- MONSTERS_EVENT_DISAPPEAR = 3
-- MONSTERS_EVENT_MOVE = 4
-- MONSTERS_EVENT_SAY = 5


---@alias CreatureTypes
---| 0 # CREATURETYPE_PLAYER
---| 1 # CREATURETYPE_MONSTER
---| 2 # CREATURETYPE_NPC
---| 3 # CREATURETYPE_SUMMON_OWN
---| 4 # CREATURETYPE_SUMMON_OTHERS
---| 5 # CREATURETYPE_HIDDEN

---@source ../src/enums.h:123
CREATURETYPE_PLAYER = 0
---@source ../src/enums.h:123
CREATURETYPE_MONSTER = 1
---@source ../src/enums.h:123
CREATURETYPE_NPC = 2
---@source ../src/enums.h:123
CREATURETYPE_SUMMON_OWN = 3
---@source ../src/enums.h:123
CREATURETYPE_SUMMON_OTHERS = 4
---@source ../src/enums.h:123
CREATURETYPE_HIDDEN = 5

MAX_LOOTCHANCE = 100000

---@alias FluidColours
---| 0 # FLUID_EMPTY
---| 1 # FLUID_BLUE
---| 2 # FLUID_RED
---| 3 # FLUID_BROWN
---| 4 # FLUID_GREEN
---| 5 # FLUID_YELLOW
---| 6 # FLUID_WHITE
---| 7 # FLUID_PURPLE
---| 8 # FLUID_BLACK

---@source ../src/const.h:311
FLUID_EMPTY = 0
---@source ../src/const.h:311
FLUID_BLUE = 1
---@source ../src/const.h:311
FLUID_RED = 2
---@source ../src/const.h:311
FLUID_BROWN = 3
---@source ../src/const.h:311
FLUID_GREEN = 4
---@source ../src/const.h:311
FLUID_YELLOW = 5
---@source ../src/const.h:311
FLUID_WHITE = 6
---@source ../src/const.h:311
FLUID_PURPLE = 7
---@source ../src/const.h:311
FLUID_BLACK = 8

---@alias FluidTypes
---| 0 # FLUID_NONE
---| 1 # FLUID_WATER
---| 2 # FLUID_BLOOD
---| 3 # FLUID_BEER
---| 4 # FLUID_SLIME
---| 5 # FLUID_LEMONADE
---| 6 # FLUID_MILK
---| 7 # FLUID_MANA
---| 8 # FLUID_INK
---| 9 # FLUID_LIFE
---| 10 # FLUID_OIL
---| 11 # FLUID_URINE
---| 12 # FLUID_COCONUTMILK
---| 13 # FLUID_WINE
---| 14 # FLUID_MUD
---| 15 # FLUID_FRUITJUICE
---| 16 # FLUID_LAVA
---| 17 # FLUID_RUM
---| 18 # FLUID_SWAMP
---| 19 # FLUID_TEA
---| 20 # FLUID_MEAD

---@source ../src/const.h:324
FLUID_NONE = FLUID_EMPTY
---@source ../src/const.h:324
FLUID_WATER = FLUID_BLUE
---@source ../src/const.h:324
FLUID_BLOOD = FLUID_RED
---@source ../src/const.h:324
---@---@source ../src/const.h:324
FLUID_BEER = FLUID_BROWN
---@source ../src/const.h:324
FLUID_SLIME = FLUID_GREEN
---@source ../src/const.h:324
FLUID_LEMONADE = FLUID_YELLOW
---@source ../src/const.h:324
FLUID_MILK = FLUID_WHITE
---@source ../src/const.h:324
FLUID_MANA = FLUID_PURPLE
---@source ../src/const.h:324
FLUID_INK = FLUID_BLACK
---@source ../src/const.h:324
FLUID_LIFE = FLUID_RED + 8
---@source ../src/const.h:324
FLUID_OIL = FLUID_BROWN + 8
---@source ../src/const.h:324
FLUID_URINE = FLUID_YELLOW + 8
---@source ../src/const.h:324
FLUID_COCONUTMILK = FLUID_WHITE + 8
---@source ../src/const.h:324
FLUID_WINE = FLUID_PURPLE + 8
---@source ../src/const.h:324
FLUID_MUD = FLUID_BROWN + 16
---@source ../src/const.h:324
FLUID_FRUITJUICE = FLUID_YELLOW + 16
---@source ../src/const.h:324
FLUID_LAVA = FLUID_RED + 24
---@source ../src/const.h:324
FLUID_RUM = FLUID_BROWN + 24
---@source ../src/const.h:324
FLUID_SWAMP = FLUID_GREEN + 24
---@source ../src/const.h:324
FLUID_TEA = FLUID_BROWN + 32
---@source ../src/const.h:324
FLUID_MEAD = FLUID_BROWN + 40

---@alias RaceTypes
---| 0 # RACE_NONE
---| 1 # RACE_VENOM
---| 2 # RACE_BLOOD
---| 3 # RACE_UNDEAD
---| 4 # RACE_FIRE
---| 5 # RACE_ENERGY
---| 6 # RACE_INK

---@source ../src/const.h:180
RACE_NONE = 0
---@source ../src/const.h:180
RACE_VENOM = 1
---@source ../src/const.h:180
RACE_BLOOD = 2
---@source ../src/const.h:180
RACE_UNDEAD = 3
---@source ../src/const.h:180
RACE_FIRE = 4
---@source ../src/const.h:180
RACE_ENERGY = 5
---@source ../src/const.h:180
RACE_INK = 6

---@alias GameStates
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

---@alias ReportTypes
---| 0 # REPORT_TYPE_NAME
---| 1 # REPORT_TYPE_STATEMENT
---| 2 # REPORT_TYPE_BOT

---@source ../src/enums.h:7
REPORT_TYPE_NAME = 0
---@source ../src/enums.h:7
REPORT_TYPE_STATEMENT = 1
---@source ../src/enums.h:7
REPORT_TYPE_BOT = 2

---@alias TextColors
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

---@alias SpellGroups
---| 0 # SPELLGROUP_NONE
---| 1 # SPELLGROUP_ATTACK
---| 2 # SPELLGROUP_HEALING
---| 3 # SPELLGROUP_SUPPORT
---| 4 # SPELLGROUP_SPECIAL
---| 6 # SPELLGROUP_CRIPPLING
---| 7 # SPELLGROUP_FOCUS
---| 8 # SPELLGROUP_ULTIMATESTRIKES

---@source ../src/enums.h:150
SPELLGROUP_NONE = 0
---@source ../src/enums.h:150
SPELLGROUP_ATTACK = 1
---@source ../src/enums.h:150
SPELLGROUP_HEALING = 2
---@source ../src/enums.h:150
SPELLGROUP_SUPPORT = 3
---@source ../src/enums.h:150
SPELLGROUP_SPECIAL = 4
---@source ../src/enums.h:150
-- SPELLGROUP_CONJURE = 5
---@source ../src/enums.h:150
SPELLGROUP_CRIPPLING = 6
---@source ../src/enums.h:150
SPELLGROUP_FOCUS = 7
---@source ../src/enums.h:150
SPELLGROUP_ULTIMATESTRIKES = 8

---@alias WorldTypes
---| 1 # WORLD_TYPE_NO_PVP
---| 2 # WORLD_TYPE_PVP
---| 3 # WORLD_TYPE_PVP_ENFORCED

---@source ../src/game.h:29
WORLD_TYPE_NO_PVP = 1
---@source ../src/game.h:30
WORLD_TYPE_PVP = 2
---@source ../src/game.h:31
WORLD_TYPE_PVP_ENFORCED = 3

---@alias FightModes
---| 1 # FIGHTMODE_ATTACK
---| 2 # FIGHTMODE_BALANCE
---| 3 # FIGHTMODE_DEFENSE

---@source ../src/player.h:35
FIGHTMODE_ATTACK = 1
---@source ../src/player.h:36
FIGHTMODE_BALANCE = 2
---@source ../src/player.h:37
FIGHTMODE_DEFENSE = 3

---@alias SquareColours
---| 0 # SQ_COLOR_BLACK

---@source ../src/const.h:387
SQ_COLOR_BLACK = 0

---@alias ResourceTypes
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

---@alias TargetSearchType
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

---@alias MonsterIcons
---| 1 # MONSTER_ICON_VULNERABLE
---| 2 # MONSTER_ICON_WEAKENED
---| 3 # MONSTER_ICON_MELEE
---| 4 # MONSTER_ICON_INFLUENCED
---| 5 # MONSTER_ICON_FIENDISH
---| 1 # MONSTER_ICON_FIRST
---| 5 # MONSTER_ICON_LAST

---@source ../src/const.h:688
MONSTER_ICON_VULNERABLE = 1
---@source ../src/const.h:688
MONSTER_ICON_WEAKENED = 2
---@source ../src/const.h:688
MONSTER_ICON_MELEE = 3
---@source ../src/const.h:688
MONSTER_ICON_INFLUENCED = 4
---@source ../src/const.h:688
MONSTER_ICON_FIENDISH = 5

---@source ../src/const.h:688
MONSTER_ICON_FIRST = MONSTER_ICON_VULNERABLE
---@source ../src/const.h:688
MONSTER_ICON_LAST =  MONSTER_ICON_FIENDISH

---@alias CreatureIcons
---| 1 # CREATURE_ICON_CROSS_WHITE
---| 2 # CREATURE_ICON_CROSS_WHITE_RED
---| 3 # CREATURE_ICON_ORB_RED
---| 4 # CREATURE_ICON_ORB_GREEN
---| 5 # CREATURE_ICON_ORB_RED_GREEN
---| 6 # CREATURE_ICON_GEM_GREEN
---| 7 # CREATURE_ICON_GEM_YELLOW
---| 8 # CREATURE_ICON_GEM_BLUE
---| 9 # CREATURE_ICON_GEM_PURPLE
---| 10 # CREATURE_ICON_GEM_RED
---| 11 # CREATURE_ICON_PIGEON
---| 12 # CREATURE_ICON_ENERGY
---| 13 # CREATURE_ICON_POISON
---| 14 # CREATURE_ICON_WATER
---| 15 # CREATURE_ICON_FIRE
---| 16 # CREATURE_ICON_ICE
---| 17 # CREATURE_ICON_ARROW_UP
---| 18 # CREATURE_ICON_ARROW_DOWN
---| 19 # CREATURE_ICON_WARNING
---| 20 # CREATURE_ICON_QUESTION
---| 21 # CREATURE_ICON_CROSS_RED

---@source ../src/const.h:698
CREATURE_ICON_CROSS_WHITE = 1
---@source ../src/const.h:698
CREATURE_ICON_CROSS_WHITE_RED = 2
---@source ../src/const.h:698
CREATURE_ICON_ORB_RED = 3
---@source ../src/const.h:698
CREATURE_ICON_ORB_GREEN = 4
---@source ../src/const.h:698
CREATURE_ICON_ORB_RED_GREEN = 5
---@source ../src/const.h:698
CREATURE_ICON_GEM_GREEN = 6
---@source ../src/const.h:698
CREATURE_ICON_GEM_YELLOW = 7
---@source ../src/const.h:698
CREATURE_ICON_GEM_BLUE = 8
---@source ../src/const.h:698
CREATURE_ICON_GEM_PURPLE = 9
---@source ../src/const.h:698
CREATURE_ICON_GEM_RED = 10
---@source ../src/const.h:698
CREATURE_ICON_PIGEON = 11
---@source ../src/const.h:698
CREATURE_ICON_ENERGY = 12
---@source ../src/const.h:698
CREATURE_ICON_POISON = 13
---@source ../src/const.h:698
CREATURE_ICON_WATER = 14
---@source ../src/const.h:698
CREATURE_ICON_FIRE = 15
---@source ../src/const.h:698
CREATURE_ICON_ICE = 16
---@source ../src/const.h:698
CREATURE_ICON_ARROW_UP = 17
---@source ../src/const.h:698
CREATURE_ICON_ARROW_DOWN = 18
---@source ../src/const.h:698
CREATURE_ICON_WARNING = 19
---@source ../src/const.h:698
CREATURE_ICON_QUESTION = 20
---@source ../src/const.h:698
CREATURE_ICON_CROSS_RED = 21

---@source ../src/const.h:698
CREATURE_ICON_FIRST = CREATURE_ICON_CROSS_WHITE
---@source ../src/const.h:698
CREATURE_ICON_LAST = CREATURE_ICON_CROSS_RED

--These aliases are at the bottom so when you "ctrl+click" an alias that uses them, the alias definition will pop up first

---@alias uint8 number
---@alias uint16 number
---@alias uint32 number
---@alias uint64 number
---@alias int8 number
---@alias int16 number
---@alias int32 number 
---@alias int64 number 
---@alias double number 