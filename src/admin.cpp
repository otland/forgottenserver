/**
 * The Forgotten Server - a server application for the MMORPG Tibia
 * Copyright (C) 2013  Mark Samman <mark.samman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "otpch.h"

#include "admin.h"
#include "game.h"
#include "connection.h"
#include "outputmessage.h"
#include "networkmessage.h"
#include "configmanager.h"
#include "house.h"
#include "iologindata.h"
#include "ban.h"
#include "tools.h"
#include "rsa.h"

#include "ext/pugixml.hpp"
#include "pugicast.h"

#include "logger.h"

static void addLogLine(ProtocolAdmin* conn, eLogType type, int level, const std::string& message);

extern Game g_game;
extern ConfigManager g_config;
extern Ban g_bans;

AdminProtocolConfig* g_adminConfig = nullptr;

#ifdef __ENABLE_SERVER_DIAGNOSTIC__
uint32_t ProtocolAdmin::protocolAdminCount = 0;
#endif

ProtocolAdmin::ProtocolAdmin(Connection_ptr connection) :
	Protocol(connection)
{
	m_state = NO_CONNECTED;
	m_loginTries = 0;
	m_lastCommand = 0;
	m_startTime = time(nullptr);
#ifdef __ENABLE_SERVER_DIAGNOSTIC__
	protocolAdminCount++;
#endif
}

ProtocolAdmin::~ProtocolAdmin()
{
#ifdef __ENABLE_SERVER_DIAGNOSTIC__
	protocolAdminCount--;
#endif
}

void ProtocolAdmin::onRecvFirstMessage(NetworkMessage& msg)
{
	//is the remote admin protocol enabled?
	if (!g_adminConfig->isEnabled()) {
		getConnection()->closeConnection();
		return;
	}

	m_state = NO_CONNECTED;

	//is allowed this ip?
	if (!g_adminConfig->allowIP(getIP())) {
		addLogLine(this, LOGTYPE_EVENT, 1, "ip not allowed");
		getConnection()->closeConnection();
		return;
	}

	//max connections limit
	if (!g_adminConfig->addConnection()) {
		addLogLine(this, LOGTYPE_EVENT, 1, "cannot add new connection");
		getConnection()->closeConnection();
		return;
	}

	addLogLine(this, LOGTYPE_EVENT, 1, "sending HELLO");
	//send hello
	OutputMessage_ptr output = OutputMessagePool::getInstance()->getOutputMessage(this, false);
	if (output) {
		output->AddByte(AP_MSG_HELLO);
		output->AddU32(1); //version
		output->AddString("OTADMIN");
		output->AddU16(g_adminConfig->getProtocolPolicy()); //security policy
		output->AddU32(g_adminConfig->getProtocolOptions()); //protocol options(encryption, ...)
		OutputMessagePool::getInstance()->send(output);
	}

	m_lastCommand = time(nullptr);
	m_state = ENCRYPTION_NO_SET;
}

void ProtocolAdmin::deleteProtocolTask()
{
	addLogLine(nullptr, LOGTYPE_EVENT, 1, "end connection");
	g_adminConfig->removeConnection();
	Protocol::deleteProtocolTask();
}

void ProtocolAdmin::parsePacket(NetworkMessage& msg)
{
	if (g_game.getGameState() == GAME_STATE_SHUTDOWN) {
		getConnection()->closeConnection();
		return;
	}

	uint8_t recvbyte = msg.GetByte();

	OutputMessagePool* outputPool = OutputMessagePool::getInstance();

	OutputMessage_ptr output = outputPool->getOutputMessage(this, false);
	if (!output) {
		return;
	}

	switch (m_state) {
		case ENCRYPTION_NO_SET: {
			if (g_adminConfig->requireEncryption()) {
				if ((time(nullptr) - m_startTime) > 30000) {
					getConnection()->closeConnection();
					addLogLine(this, LOGTYPE_WARNING, 1, "encryption timeout");
					return;
				}

				if (recvbyte != AP_MSG_ENCRYPTION && recvbyte != AP_MSG_KEY_EXCHANGE) {
					output->AddByte(AP_MSG_ERROR);
					output->AddString("encryption needed");
					outputPool->send(output);
					getConnection()->closeConnection();
					addLogLine(this, LOGTYPE_WARNING, 1, "wrong command while ENCRYPTION_NO_SET");
					return;
				}

				break;
			} else {
				m_state = NO_LOGGED_IN;
			}
		}

		case NO_LOGGED_IN: {
			if (g_adminConfig->requireLogin()) {
				if ((time(nullptr) - m_startTime) > 30000) {
					//login timeout
					getConnection()->closeConnection();
					addLogLine(this, LOGTYPE_WARNING, 1, "login timeout");
					return;
				}

				if (m_loginTries > 3) {
					output->AddByte(AP_MSG_ERROR);
					output->AddString("too many login tries");
					outputPool->send(output);
					getConnection()->closeConnection();
					addLogLine(this, LOGTYPE_WARNING, 1, "too many login tries");
					return;
				}

				if (recvbyte != AP_MSG_LOGIN) {
					output->AddByte(AP_MSG_ERROR);
					output->AddString("you are not logged in");
					outputPool->send(output);
					getConnection()->closeConnection();
					addLogLine(this, LOGTYPE_WARNING, 1, "wrong command while NO_LOGGED_IN");
					return;
				}

				break;
			} else {
				m_state = LOGGED_IN;
			}
		}

		case LOGGED_IN: {
			//can execute commands
			break;
		}

		default: {
			getConnection()->closeConnection();
			return;
		}
	}

	m_lastCommand = time(nullptr);

	switch (recvbyte) {
		case AP_MSG_LOGIN: {
			if (m_state == NO_LOGGED_IN && g_adminConfig->requireLogin()) {
				std::string password = msg.GetString();
				if (g_adminConfig->passwordMatch(password)) {
					m_state = LOGGED_IN;
					output->AddByte(AP_MSG_LOGIN_OK);
					addLogLine(this, LOGTYPE_EVENT, 1, "login ok");
				} else {
					m_loginTries++;
					output->AddByte(AP_MSG_LOGIN_FAILED);
					output->AddString("wrong password");
					addLogLine(this, LOGTYPE_WARNING, 1, "login failed.(" + password + ")");
				}
			} else {
				output->AddByte(AP_MSG_LOGIN_FAILED);
				output->AddString("can not login");
				addLogLine(this, LOGTYPE_WARNING, 1, "wrong state at login");
			}

			break;
		}

		case AP_MSG_ENCRYPTION: {
			if (m_state == ENCRYPTION_NO_SET && g_adminConfig->requireEncryption()) {
				uint8_t keyType = msg.GetByte();

				if (keyType == ENCRYPTION_RSA1024XTEA) {
					RSA* rsa = g_adminConfig->getRSAKey(ENCRYPTION_RSA1024XTEA);

					if (!rsa) {
						output->AddByte(AP_MSG_ENCRYPTION_FAILED);
						addLogLine(this, LOGTYPE_WARNING, 1, "no valid server key type");
						break;
					}

					if (RSA_decrypt(rsa, msg)) {
						m_state = NO_LOGGED_IN;
						uint32_t k[4];
						k[0] = msg.GetU32();
						k[1] = msg.GetU32();
						k[2] = msg.GetU32();
						k[3] = msg.GetU32();

						//use for in/out the new key we have
						enableXTEAEncryption();
						setXTEAKey(k);

						output->AddByte(AP_MSG_ENCRYPTION_OK);
						addLogLine(this, LOGTYPE_EVENT, 1, "encryption ok");
					} else {
						output->AddByte(AP_MSG_ENCRYPTION_FAILED);
						output->AddString("wrong encrypted packet");
						addLogLine(this, LOGTYPE_WARNING, 1, "wrong encrypted packet");
					}
				} else {
					output->AddByte(AP_MSG_ENCRYPTION_FAILED);
					output->AddString("no valid key type");
					addLogLine(this, LOGTYPE_WARNING, 1, "no valid client key type");
				}
			} else {
				output->AddByte(AP_MSG_ENCRYPTION_FAILED);
				output->AddString("can not set encryption");
				addLogLine(this, LOGTYPE_EVENT, 1, "can not set encryption");
			}

			break;
		}

		case AP_MSG_KEY_EXCHANGE: {
			if (m_state == ENCRYPTION_NO_SET && g_adminConfig->requireEncryption()) {
				uint8_t keyType = msg.GetByte();

				if (keyType == ENCRYPTION_RSA1024XTEA) {
					RSA* rsa = g_adminConfig->getRSAKey(ENCRYPTION_RSA1024XTEA);

					if (!rsa) {
						output->AddByte(AP_MSG_KEY_EXCHANGE_FAILED);
						addLogLine(this, LOGTYPE_WARNING, 1, "no valid server key type");
						break;
					}

					output->AddByte(AP_MSG_KEY_EXCHANGE_OK);
					output->AddByte(ENCRYPTION_RSA1024XTEA);
					char RSAPublicKey[128];
					rsa->getPublicKey(RSAPublicKey);
					output->AddBytes(RSAPublicKey, 128);
				} else {
					output->AddByte(AP_MSG_KEY_EXCHANGE_FAILED);
					addLogLine(this, LOGTYPE_WARNING, 1, "no valid client key type");
				}
			} else {
				output->AddByte(AP_MSG_KEY_EXCHANGE_FAILED);
				output->AddString("can not get public key");
				addLogLine(this, LOGTYPE_WARNING, 1, "can not get public key");
			}

			break;
		}

		case AP_MSG_COMMAND: {
			if (m_state != LOGGED_IN) {
				addLogLine(this, LOGTYPE_ERROR, 1, "recvbyte == AP_MSG_COMMAND && m_state != LOGGED_IN !!!");
				// We should never reach this point
				break;
			}

			uint8_t command = msg.GetByte();
			switch (command) {
				case CMD_BROADCAST: {
					const std::string message = msg.GetString();
					addLogLine(this, LOGTYPE_EVENT, 1, "broadcast: " + message);
					g_dispatcher.addTask(createTask(boost::bind(&Game::broadcastMessage, &g_game, message, MSG_STATUS_WARNING)));
					output->AddByte(AP_MSG_COMMAND_OK);
					break;
				}

				case CMD_CLOSE_SERVER: {
					g_dispatcher.addTask(createTask(boost::bind(&ProtocolAdmin::adminCommandCloseServer, this)));
					break;
				}

				case CMD_PAY_HOUSES: {
					g_dispatcher.addTask(createTask(boost::bind(&ProtocolAdmin::adminCommandPayHouses, this)));
					break;
				}

				case CMD_OPEN_SERVER: {
					g_dispatcher.addTask(createTask(boost::bind(&ProtocolAdmin::adminCommandOpenServer, this)));
					break;
				}

				case CMD_SHUTDOWN_SERVER: {
					g_dispatcher.addTask(createTask(boost::bind(&ProtocolAdmin::adminCommandShutdownServer, this)));
					getConnection()->closeConnection();
					return;
				}

				case CMD_KICK: {
					const std::string name = msg.GetString();
					g_dispatcher.addTask(createTask(boost::bind(&ProtocolAdmin::adminCommandKickPlayer, this, name)));
					break;
				}

				case CMD_SETOWNER: {
					const std::string param = msg.GetString();
					g_dispatcher.addTask(createTask(boost::bind(&ProtocolAdmin::adminCommandSetOwner, this, param)));
					break;
				}

				default: {
					output->AddByte(AP_MSG_COMMAND_FAILED);
					output->AddString("not known server command");
					addLogLine(this, LOGTYPE_WARNING, 1, "not known server command");
					break;
				}
			}

			break;
		}

		case AP_MSG_PING: {
			output->AddByte(AP_MSG_PING_OK);
			break;
		}

		default: {
			output->AddByte(AP_MSG_ERROR);
			output->AddString("not known command byte");
			addLogLine(this, LOGTYPE_WARNING, 1, "not known command byte");
			break;
		}
	}

	if (output->getMessageLength() > 0) {
		outputPool->send(output);
	}
}

void ProtocolAdmin::adminCommandCloseServer()
{
	g_game.setGameState(GAME_STATE_CLOSED);

	addLogLine(this, LOGTYPE_EVENT, 1, "close server ok");

	OutputMessage_ptr output = OutputMessagePool::getInstance()->getOutputMessage(this, false);
	if (output) {
		output->AddByte(AP_MSG_COMMAND_OK);
		OutputMessagePool::getInstance()->send(output);
	}
}

void ProtocolAdmin::adminCommandPayHouses()
{
	OutputMessage_ptr output = OutputMessagePool::getInstance()->getOutputMessage(this, false);
	if (output) {
		if (Houses::getInstance().payHouses()) {
			addLogLine(this, LOGTYPE_EVENT, 1, "pay houses ok");
			output->AddByte(AP_MSG_COMMAND_OK);
		} else {
			addLogLine(this, LOGTYPE_WARNING, 1, "pay houses fail");
			output->AddByte(AP_MSG_COMMAND_FAILED);
			output->AddString(" ");
		}

		OutputMessagePool::getInstance()->send(output);
	}
}

void ProtocolAdmin::adminCommandOpenServer()
{
	g_game.setGameState(GAME_STATE_NORMAL);

	addLogLine(this, LOGTYPE_EVENT, 1, "open server ok");

	OutputMessage_ptr output = OutputMessagePool::getInstance()->getOutputMessage(this, false);
	if (output) {
		output->AddByte(AP_MSG_COMMAND_OK);
		OutputMessagePool::getInstance()->send(output);
	}
}

void ProtocolAdmin::adminCommandShutdownServer()
{
	g_game.setGameState(GAME_STATE_SHUTDOWN);

	addLogLine(this, LOGTYPE_EVENT, 1, "starting server shutdown");

	OutputMessage_ptr output = OutputMessagePool::getInstance()->getOutputMessage(this, false);
	if (output) {
		output->AddByte(AP_MSG_COMMAND_OK);
		OutputMessagePool::getInstance()->send(output);
	}
}

void ProtocolAdmin::adminCommandKickPlayer(const std::string& name)
{
	OutputMessage_ptr output = OutputMessagePool::getInstance()->getOutputMessage(this, false);
	if (output) {
		Player* player = g_game.getPlayerByName(name);
		if (player) {
			player->kickPlayer(false);
			addLogLine(this, LOGTYPE_EVENT, 1, "kicked player " + name);

			output->AddByte(AP_MSG_COMMAND_OK);
		} else {
			addLogLine(this, LOGTYPE_WARNING, 1, "Could not kick player (not online): " + name);

			output->AddByte(AP_MSG_COMMAND_FAILED);
			output->AddString("player is not online");
		}

		OutputMessagePool::getInstance()->send(output);
	}
}

void ProtocolAdmin::adminCommandSetOwner(const std::string& param)
{
	OutputMessage_ptr output = OutputMessagePool::getInstance()->getOutputMessage(this, false);
	if (output) {
		Tokenizer tokens(param, boost::char_separator<char>(","));

		auto it = tokens.begin();
		std::string _house = parseNextParam(it, tokens.end());
		if (House* house = Houses::getInstance().getHouse(atoi(_house.c_str()))) {
			uint32_t _guid;

			std::string name = parseNextParam(it, tokens.end());
			if (IOLoginData::getInstance()->getGuidByName(_guid, name)) {
				house->setHouseOwner(_guid);
				addLogLine(this, LOGTYPE_EVENT, 1, "set " + name + " as new owner of house with id " + _house);
				output->AddByte(AP_MSG_COMMAND_OK);
			} else {
				addLogLine(this, LOGTYPE_WARNING, 1, "Could not find player with name: " + name);
				output->AddByte(AP_MSG_COMMAND_FAILED);
				output->AddString("such player does not exists");
			}
		} else {
			addLogLine(this, LOGTYPE_WARNING, 1, "Could not find house with id: " + _house);
			output->AddByte(AP_MSG_COMMAND_FAILED);
			output->AddString("such house does not exists");
		}

		OutputMessagePool::getInstance()->send(output);
	}
}

/////////////////////////////////////////////

AdminProtocolConfig::AdminProtocolConfig()
{
	m_enabled = true;
	m_onlyLocalHost = true;
	m_maxConnections = 1;
	m_currrentConnections = 0;
	m_password = "";
	m_key_RSA1024XTEA = nullptr;
	m_requireLogin = true;
	m_requireEncryption = false;
}

AdminProtocolConfig::~AdminProtocolConfig()
{
	delete m_key_RSA1024XTEA;
}

bool AdminProtocolConfig::loadXMLConfig()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/XML/admin.xml");
	if (!result) {
		std::cout << "[Error - AdminProtocolConfig::loadXMLConfig] Failed to load data/XML/admin.xml: " << result.description() << std::endl;
		return false;
	}

	pugi::xml_node otadminNode = doc.child("otadmin");
	if (!otadminNode) {
		return false;
	}

	m_enabled = otadminNode.attribute("enabled").as_bool();
	for (pugi::xml_node node = otadminNode.first_child(); node; node = node.next_sibling()) {
		if (strcasecmp(node.name(), "security") == 0) {
			m_onlyLocalHost = node.attribute("onlylocalhost").as_bool();
			m_maxConnections = pugi::cast<int32_t>(node.attribute("maxconnections").value());
			m_requireLogin = node.attribute("loginrequired").as_bool();

			pugi::xml_attribute loginPasswordAttribute = node.attribute("loginpassword");
			if (loginPasswordAttribute) {
				m_password = loginPasswordAttribute.as_string();
			} else if (m_requireLogin) {
				std::cout << "[AdminProtocolConfig::loadXMLConfig - Security warning] Login required, but using default password." << std::endl;
			}
		} else if (strcasecmp(node.name(), "encryption") == 0) {
			m_requireEncryption = node.attribute("required").as_bool();
			pugi::xml_node encryptionNode = node.child("key");
			if (encryptionNode) {
				std::string encryptionType = encryptionNode.attribute("type").as_string();
				if (asLowerCaseString(encryptionType) != "rsa1024xtea") {
					std::cout << "[AdminProtocolConfig::loadXMLConfig - Warning] " << encryptionType << " is not a valid key type." << std::endl;
					continue;
				}

				pugi::xml_attribute encryptionFile = encryptionNode.attribute("file");
				if (!encryptionFile) {
					std::cout << "[AdminProtocolConfig::loadXMLConfig - Warning] Missing file for RSA1024XTEA key." << std::endl;
					continue;
				}

				m_key_RSA1024XTEA = new RSA();
				if (!m_key_RSA1024XTEA->setKey("data/XML/" + std::string(encryptionFile.as_string()))) {
					delete m_key_RSA1024XTEA;
					m_key_RSA1024XTEA = nullptr;
					std::cout << "[AdminProtocolConfig::loadXMLConfig - Warning] Can not load key from data/XML/" << encryptionFile.as_string() << std::endl;
				}
			}
		}
	}
	return true;
}

bool AdminProtocolConfig::isEnabled() const
{
	return m_enabled;
}

bool AdminProtocolConfig::addConnection()
{
	if (m_currrentConnections >= m_maxConnections) {
		return false;
	}

	m_currrentConnections++;
	return true;
}

void AdminProtocolConfig::removeConnection()
{
	if (m_currrentConnections > 0) {
		m_currrentConnections--;
	}
}

bool AdminProtocolConfig::passwordMatch(const std::string& password)
{
	//prevent empty password login
	if (m_password.empty()) {
		return false;
	}
	return password == m_password;
}

bool AdminProtocolConfig::allowIP(uint32_t ip)
{
	if (m_onlyLocalHost && ip != 0x0100007F) { //127.0.0.1
		addLogLine(nullptr, LOGTYPE_WARNING, 1, std::string("forbidden connection try from ") + convertIPToString(ip));
		return false;
	}
	return true;
}

bool AdminProtocolConfig::requireLogin() const
{
	return m_requireLogin;
}

bool AdminProtocolConfig::requireEncryption() const
{
	return m_requireEncryption;
}

uint16_t AdminProtocolConfig::getProtocolPolicy()
{
	uint16_t policy = 0;
	if (requireLogin()) {
		policy |= REQUIRE_LOGIN;
	}

	if (requireEncryption()) {
		policy |= REQUIRE_ENCRYPTION;
	}
	return policy;
}

uint32_t AdminProtocolConfig::getProtocolOptions()
{
	uint32_t ret = 0;
	if (requireEncryption()) {
		if (m_key_RSA1024XTEA) {
			ret |= ENCRYPTION_RSA1024XTEA;
		}
	}
	return ret;
}

RSA* AdminProtocolConfig::getRSAKey(uint8_t type)
{
	switch (type) {
		case ENCRYPTION_RSA1024XTEA:
			return m_key_RSA1024XTEA;

		default:
			return nullptr;
	}
}

/////////////////////////////////////////////

static void addLogLine(ProtocolAdmin* conn, eLogType type, int level, const std::string& message)
{
	if (!g_config.getBoolean(ConfigManager::ADMIN_LOGS_ENABLED)) {
		return;
	}

	std::string logMsg;
	if (conn) {
		logMsg = "[" + convertIPToString(conn->getIP()) + "] - ";
	}

	logMsg += message;
	LOG_MESSAGE("OTADMIN", type, level, logMsg);
}
