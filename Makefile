# Makefile to compile under Debian GNU/Linux
TFS = forgottenserver

VPATH = src

INCLUDEDIRS = -I"." -I"/usr/include/libxml2" -I"/usr/include/luajit-2.0" -I"/usr/include/mysql"

FLAGS = -D__NO_HOMEDIR_CONF__ -D__LUAJIT__ -DBOOST_DISABLE_ASSERTS -DNDEBUG

CXXFLAGS = -march=native $(INCLUDEDIRS) $(FLAGS) -Werror -Wall -O3 -ggdb3 -pthread -std=c++11
CXX = ccache g++

LIBS = -lxml2 -lluajit-5.1 -lgmp -lboost_thread -lboost_system -lboost_regex -lmysqlclient /usr/lib/libtcmalloc_minimal.so.4

LDFLAGS = $(LIBS)

CXXSOURCES = actions.cpp admin.cpp ban.cpp baseevents.cpp beds.cpp \
	creature.cpp creatureevent.cpp chat.cpp combat.cpp commands.cpp \
	condition.cpp configmanager.cpp connection.cpp container.cpp \
	cylinder.cpp database.cpp databasemanager.cpp databasemysql.cpp \
	depotchest.cpp depotlocker.cpp fileloader.cpp game.cpp globalevent.cpp \
	guild.cpp house.cpp housetile.cpp ioguild.cpp iologindata.cpp \
	iomap.cpp iomapserialize.cpp iomarket.cpp inbox.cpp item.cpp items.cpp \
	logger.cpp luascript.cpp mailbox.cpp map.cpp md5.cpp modalwindow.cpp \
	monster.cpp monsters.cpp mounts.cpp movement.cpp networkmessage.cpp \
	npc.cpp otserv.cpp outfit.cpp outputmessage.cpp party.cpp player.cpp \
	position.cpp protocol.cpp protocolgame.cpp protocollogin.cpp \
	protocolold.cpp quests.cpp raids.cpp rsa.cpp scheduler.cpp \
	scriptmanager.cpp server.cpp sha1.cpp spawn.cpp spells.cpp status.cpp \
	talkaction.cpp tasks.cpp teleport.cpp thing.cpp tile.cpp tools.cpp \
	trashholder.cpp vocation.cpp waitlist.cpp weapons.cpp wildcardtree.cpp

OBJDIR = obj
CXXOBJECTS = $(CXXSOURCES:%.cpp=$(OBJDIR)/%.o)

all: $(TFS)

clean:
	$(RM) $(CXXOBJECTS) $(TFS)

$(TFS): $(CXXOBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(CXXOBJECTS) $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<
