#include "otpch.h"

#include "configmanager.h"
#include "otserv.h"
#include "tools.h"

static bool argumentsHandler(const std::vector<std::string_view>& args)
{
	for (const auto& arg : args) {
		if (arg == "--help") {
			std::clog << "Usage:\n"
			             "\n"
			             "\t--config=$1\t\tAlternate configuration file path.\n"
			             "\t--ip=$1\t\t\tIP address of the server.\n"
			             "\t\t\t\tShould be equal to the global IP.\n"
			             "\t--http-port=$1\tPort for http to listen on.\n"
			             "\t--game-port=$1\tPort for game server to listen on.\n";
			return false;
		} else if (arg == "--version") {
			printServerVersion();
			return false;
		}

		auto tmp = explodeString(arg, "=");

		if (tmp[0] == "--config")
			ConfigManager::setString(ConfigManager::CONFIG_FILE, tmp[1]);
		else if (tmp[0] == "--ip")
			ConfigManager::setString(ConfigManager::IP, tmp[1]);
		else if (tmp[0] == "--http-port")
			ConfigManager::setNumber(ConfigManager::HTTP_PORT, std::stoi(tmp[1].data()));
		else if (tmp[0] == "--game-port")
			ConfigManager::setNumber(ConfigManager::GAME_PORT, std::stoi(tmp[1].data()));
	}

	return true;
}

int main(int argc, const char** argv)
{
	std::vector<std::string_view> args(argv, argv + argc);
	if (!argumentsHandler(args)) {
		return 1;
	}

	startServer();
	return 0;
}
