#include <getopt.h>
#include <limits.h>

#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include <pugixml.hpp>
#include "../src/pugicast.h"

void usage(const char* program)
{
	std::cout << "usage: " << program
		<< " -x path/to/data/raids -o path/to/data/globalevents\n";
}

constexpr const char* INDENT = "    ";

struct RaidEvent
{
	explicit RaidEvent(uint32_t delay) : delay{delay} {}
	virtual ~RaidEvent() = default;

	virtual void to_script(std::ostringstream& ss) const { ss << '\n'; };

	uint32_t delay;
};

struct AnnounceEvent : public RaidEvent
{
	AnnounceEvent(uint32_t delay, std::string msg, std::string type) :
			RaidEvent{delay}, message{std::move(msg)}, type{std::move(type)} {}

	static AnnounceEvent* from(uint32_t delay, const pugi::xml_node& eventNode)
	{
		pugi::xml_attribute message = eventNode.attribute("message");
		if (not message) {
			throw std::runtime_error("message tag missing for announce event.");
		}

		std::string messageType = [&](const pugi::xml_attribute& type) {
			if (not type) {
				std::cout << "[Notice] Raid: type tag missing for announce"
				             "event. Using default: MESSAGE_TYPE_ADVANCE\n";
				return "MESSAGE_EVENT_ADVANCE";
			}

			std::string tmpStrValue = type.as_string();
			if (tmpStrValue == "warning") {
				return "MESSAGE_STATUS_WARNING";
			} else if (tmpStrValue == "event") {
				return "MESSAGE_EVENT_ADVANCE";
			} else if (tmpStrValue == "default") {
				return "MESSAGE_EVENT_DEFAULT";
			} else if (tmpStrValue == "description") {
				return "MESSAGE_INFO_DESCR";
			} else if (tmpStrValue == "smallstatus") {
				return "MESSAGE_STATUS_SMALL";
			} else if (tmpStrValue == "blueconsole") {
				return "MESSAGE_STATUS_CONSOLE_BLUE";
			} else if (tmpStrValue == "redconsole") {
				return "MESSAGE_STATUS_CONSOLE_RED";
			}
			std::cout << "[Notice] Raid: Unknown type tag for announce"
				         "event. Using default: MESSAGE_TYPE_ADVANCE\n";
			return "MESSAGE_EVENT_ADVANCE";
		}(eventNode.attribute("type"));

		return new AnnounceEvent{delay, message.as_string(), messageType};
	}

	void to_script(std::ostringstream& ss) const override
	{
		ss << INDENT << "Game.broadcastMessage(\"" << message << "\", " << type
		   << ")\n";
	}

	std::string message, type;
};

struct Position
{
	int32_t x;
	int32_t y;
	int16_t z;
};

template<class T>
T require_attr(const pugi::xml_node& node, const std::string& tag)
{
	if (pugi::xml_attribute attr = node.attribute(tag.c_str())) {
		return pugi::cast<T>(attr.value());
	}
	throw std::runtime_error(tag + " tag missing.");
}

struct SingleSpawnEvent : public RaidEvent
{
	SingleSpawnEvent(uint32_t delay, std::string name, Position pos) :
			RaidEvent{delay}, name{std::move(name)}, pos{std::move(pos)} {}

	static SingleSpawnEvent* from(uint32_t delay, const pugi::xml_node& eventNode)
	{
		pugi::xml_attribute monsterName = eventNode.attribute("name");
		if (not monsterName) {
			throw std::runtime_error("name tag missing for singlespawn event.");
		}

		Position pos{
			require_attr<int32_t>(eventNode, "x"),
			require_attr<int32_t>(eventNode, "y"),
			require_attr<int16_t>(eventNode, "z"),
		};

		return new SingleSpawnEvent{delay, monsterName.as_string(), std::move(pos)};
	}

	void to_script(std::ostringstream& ss) const override
	{
		ss << INDENT << "Game.createMonster(\"" << name << "\", Position("
			<< pos.x << ", " << pos.y << ", " << pos.z << ")\n";
	}

	std::string name;
	Position pos;
};

struct Spawn
{
	std::string name;
	uint32_t minAmount, maxAmount;
};

Spawn parseSpawnNode(const pugi::xml_node& monsterNode)
{
	pugi::xml_attribute name = monsterNode.attribute("name");
	if (not name) {
		throw std::runtime_error("name tag missing for monster node.");
	}

	uint32_t minAmount = 0, maxAmount = 0;
	if (pugi::xml_attribute attr = monsterNode.attribute("minamount")) {
		minAmount = pugi::cast<uint32_t>(attr.value());
	}
	if (pugi::xml_attribute attr = monsterNode.attribute("maxamount")) {
		maxAmount = pugi::cast<uint32_t>(attr.value());
	}

	if (maxAmount == 0 and minAmount == 0) {
		minAmount = maxAmount = require_attr<uint32_t>(monsterNode, "amount");
	}

	return {name.as_string(), minAmount, maxAmount};
}

std::vector<Spawn> parseSpawnList(const pugi::xml_node& monsterNode)
{
	std::vector<Spawn> spawnList;

	for (auto&& monsterNode: monsterNode.children()) {
		try {
			auto&& spawn = parseSpawnNode(monsterNode);
			spawnList.emplace_back(std::move(spawn));
		} catch (const std::runtime_error& err) {
			std::cout << "[Error - parseSpawn] " << err.what() << std::endl;
		}
	}

	return spawnList;
}

struct AreaSpawnEvent : public RaidEvent
{
	AreaSpawnEvent(uint32_t delay, Position from, Position to, uint32_t radius, std::vector<Spawn> spawnList) :
			RaidEvent{delay}, from_{std::move(from)}, to{std::move(to)}, radius{radius}, spawnList{std::move(spawnList)} {}

	static AreaSpawnEvent* from(uint32_t delay, const pugi::xml_node& eventNode, uint16_t radius)
	{
		if (radius == 0) {
			Position from{
				require_attr<int32_t>(eventNode, "fromx"),
				require_attr<int32_t>(eventNode, "fromy"),
				require_attr<int16_t>(eventNode, "fromz"),
			};

			Position to{
				require_attr<int32_t>(eventNode, "tox"),
				require_attr<int32_t>(eventNode, "toy"),
				require_attr<int16_t>(eventNode, "toz"),
			};

			return new AreaSpawnEvent{delay, from, to, 0, parseSpawnList(eventNode)};
		} else {
			Position center{
				require_attr<int32_t>(eventNode, "centerx"),
				require_attr<int32_t>(eventNode, "centery"),
				require_attr<int16_t>(eventNode, "centerz"),
			};

			return new AreaSpawnEvent{delay, {}, center, radius, parseSpawnList(eventNode)};
		}
	}

	void to_script(std::ostringstream& ss) const override
	{
		if (radius == 0) {
			ss << INDENT << "local from, to, z = {x=" << from_.x << ", y="
				<< from_.y << "}, {x=" << to.x << ", y=" << to.y << "}, "
				<< to.z << '\n';
			for (auto&& spawn: spawnList) {
				if (spawn.minAmount == spawn.maxAmount) {
					ss << INDENT << "for _ = 1, " << spawn.minAmount
					   << " do\n";
				} else {
					ss << INDENT << "for _ = 1, math.random("
					   << spawn.minAmount << ", " << spawn.maxAmount
					   << ") do\n";
				}
				ss << INDENT << INDENT
				   << "local x, y = "
				      "math.random(from.x, to.x), "
				      "math.random(from.y, to.y)\n"
				   << INDENT << INDENT << "Game.createMonster(\""
				   << spawn.name << "\", Position(x, y, z))\n"
				   << INDENT << "end\n";
			}
		} else {
			ss << INDENT << "local center, radius, z = {x=" << to.x	<< ", y="
				<< to.y << "}, " << radius << ", " << to.z << '\n';
			for (auto&& spawn: spawnList) {
				if (spawn.minAmount == spawn.maxAmount) {
					ss << INDENT << "for _ = 1, " << spawn.minAmount
					   << " do\n";
				} else {
					ss << INDENT << "for _ = 1, math.random("
					   << spawn.minAmount << ", " << spawn.maxAmount
					   << ") do\n";
				}
				ss << INDENT << INDENT
				   << "local x, y = "
				      "math.random(center.x - radius, center.x + radius), "
				      "math.random(center.y - radius, center.y + radius)\n"
				   << INDENT << INDENT << "Game.createMonster(\""
				   << spawn.name << "\", Position(x, y, z))\n"
				   << INDENT << "end\n";
			}
		}
	}

	Position from_, to;
	uint32_t radius;
	std::vector<Spawn> spawnList;
};

struct ScriptEvent : public RaidEvent
{
	ScriptEvent(uint32_t delay) :
			RaidEvent{delay} {}
};

struct RaidInfo
{
	std::string name, file;
	uint32_t interval, margin;
	bool repeat;
};

struct Raid
{
	Raid(std::vector<std::unique_ptr<RaidEvent>> eventList, RaidInfo info) :
			eventList{std::move(eventList)}, info{std::move(info)} {}

	std::string to_script() const
	{
		std::ostringstream prelude, callback;

		uint32_t counter = 0u;
		for (auto&& event: eventList) {
			prelude << "local function event" << counter << "()\n";
			event->to_script(prelude);
			prelude << "end\n\n";
			callback << INDENT << "addEvent(event" << counter << ", " << event->delay << ")\n";

			++counter;
		}

		prelude << "function onTime(interval)\n" << callback.str() << INDENT
			<< "return true\nend";

		return prelude.str();
	}

	std::vector<std::unique_ptr<RaidEvent>> eventList;
	RaidInfo info;
};

std::unique_ptr<RaidEvent> parseEvent(const std::string& eventType, const pugi::xml_node& eventNode)
{
	pugi::xml_attribute delayAttr = eventNode.attribute("delay");
	if (not delayAttr) {
		throw std::runtime_error("delay tag missing.");
	}

	uint32_t delay = pugi::cast<uint32_t>(delayAttr.value());
	if (eventType == "announce") {
		return std::unique_ptr<RaidEvent>{AnnounceEvent::from(delay, eventNode)};
	} else if (eventType == "singlespawn") {
		return std::unique_ptr<RaidEvent>{SingleSpawnEvent::from(delay, eventNode)};
	} else if (eventType == "areaspawn") {
		uint16_t radius = 0;
		if (auto radiusAttr = eventNode.attribute("radius")) {
			radius = pugi::cast<uint16_t>(radiusAttr.value());
		}
		return std::unique_ptr<RaidEvent>{AreaSpawnEvent::from(delay, eventNode, radius)};
	} else if (eventType == "script") {
		return std::unique_ptr<RaidEvent>{new ScriptEvent{delay}};
	}
	throw std::runtime_error("invalid event type " + eventType);
}

Raid loadRaid(RaidInfo info, const std::string& raids_path)
{
	std::string xml_path = raids_path + "/" + info.file;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xml_path.c_str());
	if (not result) {
		throw std::runtime_error(result.description());
	}

	std::vector<std::unique_ptr<RaidEvent>> eventList;
	for (auto&& eventNode: doc.child("raid").children()) {
		try {
			auto&& event = parseEvent(eventNode.name(), eventNode);
			eventList.emplace_back(std::move(event));
		} catch (const std::runtime_error& err) {
			std::cout << "[Error - loadRaid] " << info.file << ": " << err.what() << std::endl;
		}
	}

	std::sort(eventList.begin(), eventList.end(), [](
			const std::unique_ptr<RaidEvent>& lhs,
			const std::unique_ptr<RaidEvent>& rhs
	) {
		return lhs->delay < rhs->delay;
	});

	return {std::move(eventList), std::move(info)};
}

std::vector<Raid> loadRaids(const std::string& raids_path)
{
	std::string xml_path = raids_path + "/raids.xml";
	std::cout << xml_path << std::endl;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xml_path.c_str());
	if (not result) {
		std::cout << "[Error - loadRaids] " << result.description() << std::endl;
		return {};
	}

	std::vector<Raid> raidList;
	for (auto&& raidNode: doc.child("raids").children()) {
		pugi::xml_attribute attr;

		std::string name;
		if ((attr = raidNode.attribute("name"))) {
			name = attr.as_string();
		} else {
			std::cout << "[Error - loadRaids] Name tag missing for raid" << std::endl;
			continue;
		}

		std::string file;
		if ((attr = raidNode.attribute("file"))) {
			file = attr.as_string();
		} else {
			std::ostringstream ss;
			ss << "raids/" << name << ".xml";
			file = ss.str();
			std::cout << "[Warning - loadRaids] File tag missing for raid " << name << ". Using default: " << file << std::endl;
		}

		uint32_t interval = pugi::cast<uint32_t>(raidNode.attribute("interval2").value()) * 60;
		if (interval == 0) {
			std::cout << "[Error - loadRaids] interval2 tag missing or zero (would divide by 0) for raid: " << name << std::endl;
			continue;
		}

		uint32_t margin = 0;
		if ((attr = raidNode.attribute("margin"))) {
			margin = pugi::cast<uint32_t>(attr.value()) * 60 * 1000;
		} else {
			std::cout << "[Warning - loadRaids] margin tag missing for raid: " << name << std::endl;
		}

		bool repeat = false;
		if ((attr = raidNode.attribute("repeat"))) {
			repeat = pugi::cast<bool>(attr.as_string());
		}

		try {
			Raid&& raid = loadRaid({name, file, interval, margin, repeat}, raids_path);
			raidList.emplace_back(std::move(raid));
		} catch (const std::runtime_error& err) {
			std::cout << "[Error - loadRaids] Failed to load raid \"" << name << "\": " << err.what() << std::endl;
		}
	}

	return raidList;
}

void storeRaids(const std::string& globalevents_path, const std::vector<Raid>& raidList)
{
	for (auto&& raid: raidList) {
		std::string script_path = raid.info.file;
		script_path.replace(script_path.size() - 4, 4, ".lua");
		std::cout << script_path << std::endl;
		std::cout << raid.to_script() << std::endl;
		std::cout << std::endl;
	}
}

int main(int argc, char** argv)
{
	std::string globalevents_path, raids_path;

	int opt;
	while ((opt = getopt(argc, argv, "hg:r:")) != -1) {
		switch (opt) {
			case '?':
			case 'h':
				usage(argv[0]);
				return 0;

			case 'g':
				globalevents_path = optarg;
				break;

			case 'r':
				raids_path = optarg;
				break;
		}
	}

	if (globalevents_path.empty() and raids_path.empty()) {
		usage(argv[0]);
		return 1;
	}

	if (globalevents_path.empty()) {
		globalevents_path = raids_path + "/../globalevents/";
	} else if (raids_path.empty()) {
		raids_path = globalevents_path + "/../raids/";
	}

	char real_path[PATH_MAX];

	realpath(globalevents_path.c_str(), real_path);
	globalevents_path = real_path;

	realpath(raids_path.c_str(), real_path);
	raids_path = real_path;

	std::cout << "Output path: " << globalevents_path << '\n';
	std::cout << "XML path: " << raids_path << '\n';

	auto raids = loadRaids(raids_path);
	storeRaids(globalevents_path, raids);
}
