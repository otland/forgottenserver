project('tfs', 'cpp', license: ['gpl-2.0'], default_options: ['cpp_std=c++17'])

cryptopp = dependency('libcryptopp')
fmt = dependency('fmt', version: '>=7.1.0')
pugixml = dependency('pugixml')

boost = dependency('boost',
	modules: ['date_time', 'filesystem', 'iostreams', 'system'],
	version: '>=1.66.0',
)

mysql = dependency('mysql', required: false)
if not mysql.found()
	mysql = dependency('mariadb')
endif

lua = dependency('luajit', required: false)
if not lua.found()
	lua = dependency('lua')
endif

sources = files([
	'src/actions.cpp',
	'src/ban.cpp',
	'src/baseevents.cpp',
	'src/bed.cpp',
	'src/chat.cpp',
	'src/combat.cpp',
	'src/condition.cpp',
	'src/configmanager.cpp',
	'src/connection.cpp',
	'src/container.cpp',
	'src/creature.cpp',
	'src/creatureevent.cpp',
	'src/cylinder.cpp',
	'src/database.cpp',
	'src/databasemanager.cpp',
	'src/databasetasks.cpp',
	'src/depotchest.cpp',
	'src/depotlocker.cpp',
	'src/events.cpp',
	'src/fileloader.cpp',
	'src/game.cpp',
	'src/globalevent.cpp',
	'src/groups.cpp',
	'src/guild.cpp',
	'src/house.cpp',
	'src/housetile.cpp',
	'src/inbox.cpp',
	'src/iologindata.cpp',
	'src/iomap.cpp',
	'src/iomapserialize.cpp',
	'src/iomarket.cpp',
	'src/item.cpp',
	'src/items.cpp',
	'src/luascript.cpp',
	'src/mailbox.cpp',
	'src/map.cpp',
	'src/monster.cpp',
	'src/monsters.cpp',
	'src/mounts.cpp',
	'src/movement.cpp',
	'src/networkmessage.cpp',
	'src/npc.cpp',
	'src/otserv.cpp',
	'src/outfit.cpp',
	'src/outputmessage.cpp',
	'src/party.cpp',
	'src/player.cpp',
	'src/podium.cpp',
	'src/position.cpp',
	'src/protocol.cpp',
	'src/protocolgame.cpp',
	'src/protocollogin.cpp',
	'src/protocolold.cpp',
	'src/protocolstatus.cpp',
	'src/quests.cpp',
	'src/raids.cpp',
	'src/rsa.cpp',
	'src/scheduler.cpp',
	'src/script.cpp',
	'src/scriptmanager.cpp',
	'src/server.cpp',
	'src/signals.cpp',
	'src/spawn.cpp',
	'src/spells.cpp',
	'src/storeinbox.cpp',
	'src/talkaction.cpp',
	'src/tasks.cpp',
	'src/teleport.cpp',
	'src/thing.cpp',
	'src/tile.cpp',
	'src/tools.cpp',
	'src/trashholder.cpp',
	'src/vocation.cpp',
	'src/weapons.cpp',
	'src/wildcardtree.cpp',
	'src/xtea.cpp',
])

tfs = executable('tfs', sources,
	dependencies: [boost, cryptopp, fmt, lua, mysql, pugixml],
)
