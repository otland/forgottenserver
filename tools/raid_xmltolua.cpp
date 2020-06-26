#include <getopt.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <pugixml.hpp>
#include "../src/pugicast.h"

namespace {

namespace fs = boost::filesystem;
namespace po = boost::program_options;

enum LogLevel {
	WARNING = 0,
	INFO = 1,
	DEBUG = 2,
};

struct ParseEventError: public std::runtime_error {
	template<class ...Args>
	ParseEventError(Args&& ...args): std::runtime_error{std::forward<Args>(args)...} {};
};

struct InvalidEventType: public ParseEventError
{
	InvalidEventType(const std::string& type): ParseEventError{build_message(type)} {}

	static std::string build_message(const std::string& type)
	{
		std::ostringstream ss;
		ss << "Invalid event type: " << type << ".";
		return ss.str();
	}
};

struct MissingTag: public ParseEventError
{
	MissingTag(const std::string& tag): ParseEventError{build_message(tag)} {}

	static std::string build_message(const std::string& tag)
	{
		std::ostringstream ss;
		ss << "Missing tag: " << tag << ".";
		return ss.str();
	}
};

constexpr const char* INDENT = "    ";

struct RaidEvent
{
	explicit RaidEvent(uint32_t delay) : delay{delay} {}
	virtual ~RaidEvent() = default;

	virtual void to_script(std::ostringstream& ss) const { ss << '\n'; };

	uint32_t delay;
};

static const std::unordered_map<std::string, std::string> messageTypes{
	{"warning", "MESSAGE_STATUS_WARNING"},
	{"event", "MESSAGE_EVENT_ADVANCE"},
	{"default", "MESSAGE_EVENT_DEFAULT"},
	{"description", "MESSAGE_INFO_DESCR"},
	{"smallstatus", "MESSAGE_STATUS_SMALL"},
	{"blueconsole", "MESSAGE_STATUS_CONSOLE_BLUE"},
	{"redconsole", "MESSAGE_STATUS_CONSOLE_RED"},
};
static const std::string defaultMessageType{"MESSAGE_EVENT_ADVANCE"};

struct AnnounceEvent : public RaidEvent
{
	AnnounceEvent(uint32_t delay, std::string msg, std::string type) :
			RaidEvent{delay}, message{std::move(msg)}, type{std::move(type)} {}

	static std::unique_ptr<RaidEvent> from(uint32_t delay, const pugi::xml_node& eventNode)
	{
		pugi::xml_attribute message = eventNode.attribute("message");
		if (not message) {
			throw MissingTag{"message"};
		}

		std::string messageType = [](const pugi::xml_attribute& type) {
			if (not type) {
				std::cout << "[Notice] Raid: type tag missing for announce "
				             "event. Using default: " << defaultMessageType <<
							 "\n";
				return defaultMessageType;
			}

			try {
				return messageTypes.at(type.as_string());
			} catch (const std::out_of_range&) {
				std::cout << "[Notice] Raid: Unknown type tag for announce "
				             "event. Using default: " << defaultMessageType <<
							 "\n";
				return defaultMessageType;
			}
		}(eventNode.attribute("type"));

		return std::make_unique<AnnounceEvent>(delay, message.as_string(), messageType);
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
T require(const pugi::xml_node& node, const std::string& tag)
{
	if (pugi::xml_attribute attr = node.attribute(tag.c_str())) {
		return pugi::cast<T>(attr.value());
	}
	throw MissingTag{tag};
}

struct SingleSpawnEvent : public RaidEvent
{
	SingleSpawnEvent(uint32_t delay, std::string name, Position pos) :
			RaidEvent{delay}, name{std::move(name)}, pos{std::move(pos)} {}

	static std::unique_ptr<RaidEvent> from(uint32_t delay, const pugi::xml_node& eventNode)
	{
		pugi::xml_attribute monsterName = eventNode.attribute("name");
		if (not monsterName) {
			throw MissingTag{"name"};
		}

		Position pos{
			require<int32_t>(eventNode, "x"),
			require<int32_t>(eventNode, "y"),
			require<int16_t>(eventNode, "z"),
		};

		return std::make_unique<SingleSpawnEvent>(delay, monsterName.as_string(), std::move(pos));
	}

	void to_script(std::ostringstream& ss) const override
	{
		ss << INDENT << "Game.createMonster(\"" << name << "\", Position("
			<< pos.x << ", " << pos.y << ", " << pos.z << "))\n";
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
		throw MissingTag{"name"};
	}

	uint32_t minAmount = 0, maxAmount = 0;
	if (pugi::xml_attribute attr = monsterNode.attribute("minamount")) {
		minAmount = pugi::cast<uint32_t>(attr.value());
	}
	if (pugi::xml_attribute attr = monsterNode.attribute("maxamount")) {
		maxAmount = pugi::cast<uint32_t>(attr.value());
	}

	if (maxAmount == 0 and minAmount == 0) {
		minAmount = maxAmount = require<uint32_t>(monsterNode, "amount");
	}

	return {name.as_string(), minAmount, maxAmount};
}

std::vector<Spawn> parseSpawnList(const pugi::xml_node& monsterNode)
{
	std::vector<Spawn> spawnList;

	for (auto&& node: monsterNode.children()) {
		try {
			auto&& spawn = parseSpawnNode(node);
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

	static std::unique_ptr<RaidEvent> from(uint32_t delay, const pugi::xml_node& eventNode, uint16_t radius)
	{
		if (radius == 0) {
			Position from{
				require<int32_t>(eventNode, "fromx"),
				require<int32_t>(eventNode, "fromy"),
				require<int16_t>(eventNode, "fromz"),
			};

			Position to{
				require<int32_t>(eventNode, "tox"),
				require<int32_t>(eventNode, "toy"),
				require<int16_t>(eventNode, "toz"),
			};

			return std::make_unique<AreaSpawnEvent>(delay, from, to, 0, parseSpawnList(eventNode));
		} else {
			Position center{
				require<int32_t>(eventNode, "centerx"),
				require<int32_t>(eventNode, "centery"),
				require<int16_t>(eventNode, "centerz"),
			};

			return std::make_unique<AreaSpawnEvent>(delay, center, center, radius, parseSpawnList(eventNode));
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
	ScriptEvent(uint32_t delay) : RaidEvent{delay} {}
};

struct RaidInfo
{
	std::string name;
	fs::path path;
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

	fs::path stem() const { return info.path.stem(); }

	std::vector<std::unique_ptr<RaidEvent>> eventList;
	RaidInfo info;
};

std::unique_ptr<RaidEvent> parseEvent(const std::string& eventType, const pugi::xml_node& eventNode)
{
	pugi::xml_attribute delayAttr = eventNode.attribute("delay");
	if (not delayAttr) {
		throw MissingTag{"delay"};
	}

	uint32_t delay = pugi::cast<uint32_t>(delayAttr.value());
	if (eventType == "announce") {
		return AnnounceEvent::from(delay, eventNode);
	} else if (eventType == "singlespawn") {
		return SingleSpawnEvent::from(delay, eventNode);
	} else if (eventType == "areaspawn") {
		uint16_t radius = 0;
		if (auto radiusAttr = eventNode.attribute("radius")) {
			radius = pugi::cast<uint16_t>(radiusAttr.value());
		}
		return AreaSpawnEvent::from(delay, eventNode, radius);
	} else if (eventType == "script") {
		return std::make_unique<ScriptEvent>(delay);
	}
	throw InvalidEventType{eventType};
}

Raid loadRaid(RaidInfo info, const fs::path& raids_path)
{
	fs::path xml_path = raids_path / info.path.filename();

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
		} catch (const ParseEventError& err) {
			std::cout << "[Error - loadRaid] " << info.path << ": " << err.what() << std::endl;
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

std::vector<Raid> loadRaids(const fs::path& raids_path, bool, bool)
{
	fs::path xml_path = raids_path / "raids.xml";
	/* std::cout << xml_path << std::endl; */

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

void storeRaids(const fs::path& globalevents_path, const std::vector<Raid>& raidList, bool dryrun, bool)
{
	for (auto&& raid: raidList) {
		fs::path script_path = globalevents_path / "scripts" / raid.stem();
		script_path += ".lua";

		std::cout << raid.info.path << " -> " << script_path << std::endl;
		if (dryrun) {
			continue;
		}

		std::ofstream output{script_path.native()};
		if (not output.is_open()) {
			std::cout << "[Error - storeRaids] Couldn't open file: " << script_path << std::endl;
			continue;
		}

		output << raid.to_script() << std::endl;
	}
}

}

int main(int argc, char** argv)
{
	std::ios::sync_with_stdio(false);
	fs::path globalevents, raids;
	bool dryrun, verbose;

	po::options_description desc{"Usage"};
	desc.add_options()
		("help,h", "print help message")
		("dry-run,d", po::bool_switch(&dryrun), "do not write files")
		("globalevents,g", po::value(&globalevents)->required(), "path to globalevents folder (required)")
		("raids,r", po::value(&raids)->required(), "path to raids folder (required)")
		("verbose,v", po::bool_switch(&verbose), "print debug messages");

	po::variables_map vm;
	try {
		po::store(po::parse_command_line(argc, argv, desc), vm);

		if (vm.count("help")) {
			std::cout << desc << std::endl;
			return 0;
		}

		po::notify(vm);
	} catch (const po::error& e) {
		std::cerr << "[Error] " << e.what() << std::endl << desc << std::endl;
		return 1;
	}

	if (globalevents.empty() and raids.empty()) {
		std::cout << desc << std::endl;
		return 1;
	}

	if (globalevents.empty()) {
		raids = raids.remove_trailing_separator();
		globalevents = raids.parent_path() / "globalevents";
	} else if (raids.empty()) {
		globalevents = globalevents.remove_trailing_separator();
		raids = globalevents.parent_path() / "raids";
	}

	std::cout << "Output path: " << globalevents << '\n';
	std::cout << "XML path: " << raids << '\n';

	storeRaids(globalevents, loadRaids(raids, dryrun, verbose), dryrun, verbose);
}
