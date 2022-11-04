project('tfs', 'cpp', license: ['gpl-2.0'], default_options: ['cpp_std=c++17'])

fmt_dep = dependency('fmt', version: '>=7.1.0', fallback: ['fmt', 'fmt_dep'])
pugixml_dep = dependency('pugixml', fallback: ['pugixml', 'pugixml_dep'])
thread_dep = dependency('threads')

boost_dep = dependency('boost',
	modules: ['date_time', 'iostreams', 'system'],
	version: '>=1.66.0',
)

# Debian installs libcrypto++, Arch Linux installs libcryptopp
cryptopp_dep = dependency('libcrypto++', required: false)
if not cryptopp_dep.found()
  cryptopp_dep = dependency('libcryptopp')
endif

# Debian installs mysqlclient, Alpine installs libmariadb, Arch Linux installs mariadb
mysql_dep = dependency('mysqlclient', required: false)
if not mysql_dep.found()
	mysql_dep = dependency('libmariadb', required: false)
	if not mysql_dep.found()
		mysql_dep = dependency('mariadb')
	endif
endif

luajit_opt = get_option('luajit')
if luajit_opt.disabled()
	lua_dep = dependency('lua', fallback: ['lua', 'lua_dep'], version: '>=5.1.0')
else
	lua_dep = dependency('luajit', required: luajit_opt.enabled())
	if not lua_dep.found()
		lua_dep = dependency('lua', fallback: ['lua', 'lua_dep'], version: '>=5.1.0')
	endif
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
	cpp_pch: ['src/otpch.h', 'src/otpch.cpp'],
	dependencies: [boost_dep, cryptopp_dep, fmt_dep, lua_dep, mysql_dep, pugixml_dep, thread_dep],
)
