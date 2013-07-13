//////////////////////////////////////////////////////////////////////
// The Forgotten Server - a server application for the MMORPG Tibia
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#include "otpch.h"

#include "definitions.h"
#include "npc.h"
#include "game.h"
#include "tools.h"
#include "configmanager.h"
#include "position.h"
#include "spells.h"
#include "player.h"

#include <algorithm>
#include <functional>
#include <string>
#include <sstream>
#include <fstream>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "luascript.h"

extern ConfigManager g_config;
extern Game g_game;
extern Spells* g_spells;

uint32_t Npc::npcAutoID = 0x80000000;
NpcScriptInterface* Npc::m_scriptInterface = NULL;

#ifdef __ENABLE_SERVER_DIAGNOSTIC__
uint32_t Npc::npcCount = 0;
#endif

void Npcs::reload()
{
	const std::map<uint32_t, Npc*>& npcs = g_game.getNpcs();
	for (auto it = npcs.begin(); it != npcs.end(); ++it) {
		it->second->closeAllShopWindows();
	}

	delete Npc::m_scriptInterface;
	Npc::m_scriptInterface = NULL;

	for (auto it = npcs.begin(); it != npcs.end(); ++it) {
		it->second->reload();
	}
}

Npc* Npc::createNpc(const std::string& name)
{
	Npc* npc = new Npc(name);
	if (!npc->load()) {
		delete npc;
		return NULL;
	}
	return npc;
}

Npc::Npc(const std::string& _name) :
	Creature()
{
	m_filename = "data/npc/" + _name + ".xml";
	loaded = false;

	m_npcEventHandler = NULL;
	reset();

#ifdef __ENABLE_SERVER_DIAGNOSTIC__
	npcCount++;
#endif
}

Npc::~Npc()
{
	reset();

#ifdef __ENABLE_SERVER_DIAGNOSTIC__
	npcCount--;
#endif
}

void Npc::addList()
{
	g_game.addNpc(this);
}

void Npc::removeList()
{
	g_game.removeNpc(this);
}

bool Npc::load()
{
	if (isLoaded()) {
		return true;
	}

	reset();

	if (!m_scriptInterface) {
		m_scriptInterface = new NpcScriptInterface();
		m_scriptInterface->loadNpcLib("data/npc/lib/npc.lua");
	}

	loaded = loadFromXml(m_filename);
	return isLoaded();
}

void Npc::reset()
{
	loaded = false;
	walkTicks = 1500;
	floorChange = false;
	attackable = false;
	hasBusyReply = false;
	hasScriptedFocus = false;
	focusCreature = 0;
	isIdle = true;
	hasUsedIdleReply = false;
	talkRadius = 4;
	idleTimeout = 0;
	lastResponseTime = OTSYS_TIME();
	defaultPublic = true;

	delete m_npcEventHandler;
	m_npcEventHandler = NULL;

	for (ResponseList::iterator it = responseList.begin(); it != responseList.end(); ++it) {
		delete *it;
	}

	for (StateList::iterator it = stateList.begin(); it != stateList.end(); ++it) {
		delete *it;
	}

	responseList.clear();
	stateList.clear();
	queueList.clear();
	m_parameters.clear();
	itemListMap.clear();
	responseScriptMap.clear();
	shopPlayerList.clear();
}

void Npc::reload()
{
	reset();
	load();

	//Simulate that the creature is placed on the map again.
	if (m_npcEventHandler) {
		m_npcEventHandler->onCreatureAppear(this);
	}

	if (walkTicks > 0) {
		addEventWalk();
	}
}

bool Npc::loadFromXml(const std::string& filename)
{
	xmlDocPtr doc = xmlParseFile(filename.c_str());

	if (doc) {
		xmlNodePtr root, p;
		root = xmlDocGetRootElement(doc);

		if (xmlStrcmp(root->name, (const xmlChar*)"npc") != 0) {
			std::cerr << "Malformed XML" << std::endl;
			return false;
		}

		int32_t intValue;
		std::string strValue;

		p = root->children;

		std::string scriptfile;
		if (readXMLString(root, "script", strValue)) {
			scriptfile = strValue;
		}

		if (readXMLString(root, "name", strValue)) {
			name = strValue;
		} else {
			name = "";
		}

		if (readXMLInteger(root, "speed", intValue)) {
			baseSpeed = intValue;
		} else {
			baseSpeed = 100;
		}

		if (readXMLInteger(root, "attackable", intValue)) {
			attackable = (intValue != 0);
		}

		if (readXMLInteger(root, "walkinterval", intValue)) {
			walkTicks = intValue;
		}

		if (readXMLInteger(root, "walkradius", intValue)) {
			masterRadius = intValue;
		}

		if (readXMLInteger(root, "floorchange", intValue)) {
			floorChange = (intValue != 0);
		}

		while (p) {
			if (xmlStrcmp(p->name, (const xmlChar*)"health") == 0) {
				if (readXMLInteger(p, "now", intValue)) {
					health = intValue;
				} else {
					health = 100;
				}

				if (readXMLInteger(p, "max", intValue)) {
					healthMax = intValue;
				} else {
					healthMax = 100;
				}
			} else if (xmlStrcmp(p->name, (const xmlChar*)"look") == 0) {
				if (readXMLInteger(p, "type", intValue)) {
					defaultOutfit.lookType = intValue;

					if (readXMLInteger(p, "head", intValue)) {
						defaultOutfit.lookHead = intValue;
					}

					if (readXMLInteger(p, "body", intValue)) {
						defaultOutfit.lookBody = intValue;
					}

					if (readXMLInteger(p, "legs", intValue)) {
						defaultOutfit.lookLegs = intValue;
					}

					if (readXMLInteger(p, "feet", intValue)) {
						defaultOutfit.lookFeet = intValue;
					}

					if (readXMLInteger(p, "addons", intValue)) {
						defaultOutfit.lookAddons = intValue;
					}
				} else if (readXMLInteger(p, "typeex", intValue)) {
					defaultOutfit.lookTypeEx = intValue;
				}

				if (readXMLInteger(p, "mount", intValue)) {
					defaultOutfit.lookMount = intValue;
				}

				currentOutfit = defaultOutfit;
			} else if (xmlStrcmp(p->name, (const xmlChar*)"parameters") == 0) {
				for (xmlNodePtr q = p->children; q != NULL; q = q->next) {
					if (xmlStrcmp(q->name, (const xmlChar*)"parameter") == 0) {
						std::string paramKey;
						std::string paramValue;

						if (!readXMLString(q, "key", paramKey)) {
							continue;
						}

						if (!readXMLString(q, "value", paramValue)) {
							continue;
						}

						m_parameters[paramKey] = paramValue;
					}
				}
			} else if (xmlStrcmp(p->name, (const xmlChar*)"interaction") == 0) {
				if (readXMLInteger(p, "talkradius", intValue)) {
					talkRadius = intValue;
				}

				if (readXMLInteger(p, "idletime", intValue) || readXMLInteger(p, "idletimeout", intValue)) {
					idleTimeout = intValue;
				}

				if (readXMLInteger(p, "defaultpublic", intValue)) {
					defaultPublic = intValue != 0;
				}

				responseList = loadInteraction(p->children);
			}

			p = p->next;
		}

		xmlFreeDoc(doc);

		if (!scriptfile.empty()) {
			m_npcEventHandler = new NpcScript(scriptfile, this);

			if (!m_npcEventHandler->isLoaded()) {
				return false;
			}
		}

		return true;
	}

	return false;
}

StorageCondition Npc::loadStorageCondition(xmlNodePtr node)
{
	StorageCondition cond = { -1, 1, STORAGE_EQUAL};

	std::string strValue;

	readXMLInteger(node, "storageId", cond.id) || readXMLInteger(node, "id", cond.id);

	readXMLInteger(node, "value", cond.value) || readXMLInteger(node, "storageValue", cond.value);

	if (readXMLString(node, "storageComp", strValue) || readXMLString(node, "comparator", strValue)) {
		std::string tmpStr = asLowerCaseString(strValue);

		if (tmpStr == "equal") {
			cond.op = STORAGE_EQUAL;
		} else if (tmpStr == "notequal") {
			cond.op = STORAGE_NOTEQUAL;
		} else if (tmpStr == "greaterorequal") {
			cond.op = STORAGE_GREATEROREQUAL;
		} else if (tmpStr == "greater") {
			cond.op = STORAGE_GREATER;
		} else if (tmpStr == "less") {
			cond.op = STORAGE_LESS;
		} else if (tmpStr == "lessorequal") {
			cond.op = STORAGE_LESSOREQUAL;
		}
	}

	return cond;
}

uint32_t Npc::loadParams(xmlNodePtr node)
{
	uint32_t params = RESPOND_DEFAULT;
	std::string strValue;

	if (readXMLString(node, "param", strValue)) {
		std::vector<std::string> paramList = explodeString(strValue, ";");

		for (std::vector<std::string>::iterator it = paramList.begin(); it != paramList.end(); ++it) {
			std::string tmpStr = asLowerCaseString(*it);

			if (tmpStr == "male") {
				params |= RESPOND_MALE;
			} else if (tmpStr == "female") {
				params |= RESPOND_FEMALE;
			} else if (tmpStr == "pzblock") {
				params |= RESPOND_PZBLOCK;
			} else if (tmpStr == "lowmoney") {
				params |= RESPOND_LOWMONEY;
			} else if (tmpStr == "enoughmoney") {
				params |= RESPOND_ENOUGHMONEY;
			} else if (tmpStr == "noamount") {
				params |= RESPOND_NOAMOUNT;
			} else if (tmpStr == "lowamount") {
				params |= RESPOND_LOWAMOUNT;
			} else if (tmpStr == "enoughamount") {
				params |= RESPOND_ENOUGHAMOUNT;
			} else if (tmpStr == "premium") {
				params |= RESPOND_PREMIUM;
			} else if (tmpStr == "promoted") {
				params |= RESPOND_PROMOTED;
			} else if (tmpStr == "druid") {
				params |= RESPOND_DRUID;
			} else if (tmpStr == "knight") {
				params |= RESPOND_KNIGHT;
			} else if (tmpStr == "paladin") {
				params |= RESPOND_PALADIN;
			} else if (tmpStr == "sorcerer") {
				params |= RESPOND_SORCERER;
			} else if (tmpStr == "lowlevel") {
				params |= RESPOND_LOWLEVEL;
			} else if (tmpStr == "highlevel") {
				params |= RESPOND_HIGHLEVEL;
			} else if (tmpStr == "knowspell") {
				params |= RESPOND_KNOWSPELL;
			} else if (tmpStr == "cannotlearnspell") {
				params |= RESPOND_CANNOTLEARNSPELL;
			} else {
				std::cout << "Warning: [Npc::loadParams] Unknown param " << (*it) << std::endl;
			}
		}
	}

	return params;
}

ResponseList Npc::loadInteraction(xmlNodePtr node)
{
	ResponseList _responseList;
	std::string strValue;
	int32_t intValue;

	while (node) {
		if (xmlStrcmp(node->name, (const xmlChar*)"include") == 0) {
			if (readXMLString(node, "file", strValue)) {
				std::string includeFilename = "data/npc/lib/" + strValue;
				xmlDocPtr doc = xmlParseFile(includeFilename.c_str());

				if (doc) {
					xmlNodePtr root;
					root = xmlDocGetRootElement(doc);

					if (xmlStrcmp(root->name, (const xmlChar*)"interaction") == 0) {
						ResponseList includeResponseList = loadInteraction(root->children);
						_responseList.insert(_responseList.end(), includeResponseList.begin(), includeResponseList.end());
					} else {
						std::cerr << "Malformed XML" << std::endl;
					}

					xmlFreeDoc(doc);
				}
			}
		} else if (xmlStrcmp(node->name, (const xmlChar*)"itemlist") == 0) {
			if (readXMLString(node, "listid", strValue)) {
				ItemListMap::iterator it = itemListMap.find(strValue);

				if (it != itemListMap.end()) {
					//duplicate listid found
					std::cout << "Warning: [Npc::loadInteraction] Duplicate listId found " << strValue << std::endl;
				} else {
					xmlNodePtr tmpNode = node->children;
					std::list<ListItem>& list = itemListMap[strValue];

					while (tmpNode) {
						if (xmlStrcmp(tmpNode->name, (const xmlChar*)"item") == 0) {
							ListItem li;

							if (readXMLInteger(tmpNode, "id", intValue)) {
								li.itemId = intValue;
							} else {
								std::cout << "Warning: [Npc::loadInteraction] Missing list item itemId " << std::endl;
								tmpNode = tmpNode->next;
								continue;
							}

							const ItemType& it = Item::items[li.itemId];

							if (readXMLInteger(tmpNode, "sellprice", intValue)) {
								li.sellPrice = intValue;
							}

							if (readXMLInteger(tmpNode, "buyprice", intValue)) {
								li.buyPrice = intValue;
							}

							if (readXMLString(tmpNode, "keywords", strValue)) {
								li.keywords = strValue;
							} else {
								std::cout << "Warning: [Npc::loadInteraction] Missing list item keywords " << std::endl;
								tmpNode = tmpNode->next;
								continue;
							}

							//optional
							if (readXMLInteger(tmpNode, "subtype", intValue)) {
								li.subType = intValue;
							} else {
								if (it.stackable) {
									li.subType = 1;
								} else if (it.isFluidContainer() || it.isSplash()) {
									li.subType = 0;
								}
							}

							if (readXMLString(tmpNode, "name", strValue)) {
								li.name = strValue;
							}

							if (readXMLString(tmpNode, "pname", strValue)) {
								li.pluralName = strValue;
							}

							list.push_back(li);
						}

						tmpNode = tmpNode->next;
					}
				}
			}
		} else if (xmlStrcmp(node->name, (const xmlChar*)"script") == 0) {
			xmlNodePtr scriptNode = node->children;

			while (scriptNode) {
				if (xmlStrcmp(scriptNode->name, (const xmlChar*)"text") == 0 ||
				        scriptNode->type == XML_CDATA_SECTION_NODE) {
					if (readXMLContentString(scriptNode, strValue)) {
						trim_left(strValue, "\r");
						trim_left(strValue, "\n");
						trim_left(strValue, " ");

						if (m_scriptInterface->reserveScriptEnv()) {
							ScriptEnvironment* scriptEnvironment = m_scriptInterface->getScriptEnv();
							scriptEnvironment->setRealPos(getPosition());
							scriptEnvironment->setNpc(this);
							m_scriptInterface->loadBuffer(strValue, NULL);
							m_scriptInterface->releaseScriptEnv();
						}
					}
				}

				scriptNode = scriptNode->next;
			}
		} else if (xmlStrcmp(node->name, (const xmlChar*)"interact") == 0) {
			NpcResponse::ResponseProperties iprop;
			iprop.publicize = defaultPublic;

			if (readXMLString(node, "keywords", strValue)) {
				iprop.inputList.push_back(asLowerCaseString(strValue));
			}

			iprop.eventType = EVENT_NONE;

			if (readXMLString(node, "event", strValue)) {
				strValue = asLowerCaseString(strValue);

				if (strValue == "onbusy") {
					hasBusyReply = true;
					iprop.eventType = EVENT_BUSY;
				} else if (strValue == "onthink") {
					iprop.eventType = EVENT_THINK;
				} else if (strValue == "onidle") {
					if (readXMLInteger(node, "time", intValue)) {
						iprop.time = intValue;
					} else {
						std::cout << "Warning: [Npc::loadInteraction] Missing time attribute for onidle event" << std::endl;
					}

					if (readXMLInteger(node, "singleevent", intValue)) {
						iprop.singleEvent = (intValue == 1);
					}

					iprop.eventType = EVENT_IDLE;
				} else if (strValue == "onplayerenter") {
					iprop.eventType = EVENT_PLAYER_ENTER;
				} else if (strValue == "onplayermove") {
					iprop.eventType = EVENT_PLAYER_MOVE;
				} else if (strValue == "onplayerleave") {
					iprop.eventType = EVENT_PLAYER_LEAVE;
				} else if (strValue == "onplayershopsell") {
					iprop.eventType = EVENT_PLAYER_SHOPSELL;
				} else if (strValue == "onplayershopbuy") {
					iprop.eventType = EVENT_PLAYER_SHOPBUY;
				} else if (strValue == "onplayershopclose") {
					iprop.eventType = EVENT_PLAYER_SHOPCLOSE;
				} else {
					std::cout << "Warning: [Npc::loadInteraction] Invalid event type -" << strValue << std::endl;
				}
			}

			if (readXMLInteger(node, "topic", intValue)) {
				if (intValue <= 0) {
					std::cout << "Warning: [Npc::loadInteraction] Invalid topic value -" << intValue << std::endl;
				}

				iprop.topic = intValue;
			} else if (readXMLInteger(node, "nottopic", intValue)) {
				iprop.topic = intValue;
				iprop.params |= RESPOND_NOTTOPIC;
			}

			if (readXMLInteger(node, "focus", intValue)) {
				iprop.focusStatus = intValue;
			}

			if (readXMLInteger(node, "haveitem", intValue)) {
				iprop.haveItemId = intValue;
			}

			if (readXMLInteger(node, "donthaveitem", intValue)) {
				iprop.dontHaveItemId = intValue;
			}

			if (readXMLInteger(node, "lowlevel", intValue)) {
				iprop.params |= RESPOND_LOWLEVEL;
				iprop.level = intValue;
			} else if (readXMLInteger(node, "highlevel", intValue)) {
				iprop.params |= RESPOND_HIGHLEVEL;
				iprop.level = intValue;
			}

			uint32_t interactParams = loadParams(node);

			StorageCondition sc = loadStorageCondition(node);

			if (sc.id != -1) {
				iprop.storageConditions.push_back(sc);
			}

			xmlNodePtr tmpNode = node->children;

			while (tmpNode) {
				if (xmlStrcmp(tmpNode->name, (const xmlChar*)"keywords") == 0) {
					//alternative input keywords
					xmlNodePtr altKeyNode = tmpNode->children;

					while (altKeyNode) {
						if (xmlStrcmp(altKeyNode->name, (const xmlChar*)"text") == 0) {
							if (readXMLContentString(altKeyNode, strValue)) {
								iprop.inputList.push_back(asLowerCaseString(strValue));
							}
						}

						altKeyNode = altKeyNode->next;
					}
				} else if (xmlStrcmp(tmpNode->name, (const xmlChar*)"storage") == 0) {
					StorageCondition sc = loadStorageCondition(tmpNode);

					if (sc.id != -1) {
						iprop.storageConditions.push_back(sc);
					}
				} else if (xmlStrcmp(tmpNode->name, (const xmlChar*)"list") == 0) {
					xmlNodePtr listNode = tmpNode->children;

					while (listNode) {
						if (xmlStrcmp(listNode->name, (const xmlChar*)"text") == 0) {
							if (readXMLContentString(listNode, strValue)) {
								ItemListMap::iterator it = itemListMap.find(strValue);

								if (it != itemListMap.end()) {
									iprop.itemList.insert(iprop.itemList.end(), it->second.begin(), it->second.end());
								} else {
									std::cout << "Warning: [Npc::loadInteraction] Could not find a list id called " << strValue << std::endl;
								}
							}
						}

						listNode = listNode->next;
					}
				}

				tmpNode = tmpNode->next;
			}

			tmpNode = node->children;

			while (tmpNode) {
				if (xmlStrcmp(tmpNode->name, (const xmlChar*)"response") == 0) {
					//copy the properties from the interaction
					NpcResponse::ResponseProperties prop = iprop;

					prop.params = interactParams | loadParams(tmpNode);
					ScriptVars scriptVars;

					if (readXMLString(tmpNode, "knowspell", strValue)) {
						prop.knowSpell = strValue;
					}

					if (readXMLString(tmpNode, "text", strValue)) {
						prop.responseType = RESPONSE_DEFAULT;
						replaceString(strValue, "&lt;", "<");
						replaceString(strValue, "&gt;", ">");
						replaceString(strValue, "&amp;", "&");
						replaceString(strValue, "&apos;", "\\");
						replaceString(strValue, "&quot;", "\"");
						prop.output = strValue;
					} else if (readXMLString(tmpNode, "function", strValue)) {
						prop.responseType = RESPONSE_SCRIPT;
						prop.output = strValue;
					}

					if (readXMLInteger(tmpNode, "public", intValue)) {
						prop.publicize = (intValue == 1);
					}

					if (readXMLInteger(tmpNode, "lowhealth", intValue)) {
						prop.health = intValue;
					}

					if (readXMLString(tmpNode, "condition", strValue)) {
						if (strValue == "poison") {
							prop.condition = CONDITION_POISON;
						} else if (strValue == "fire") {
							prop.condition = CONDITION_FIRE;
						} else if (strValue == "energy") {
							prop.condition = CONDITION_ENERGY;
						} else if (strValue == "haste") {
							prop.condition = CONDITION_HASTE;
						} else if (strValue == "paralyze") {
							prop.condition = CONDITION_PARALYZE;
						} else if (strValue == "outfit") {
							prop.condition = CONDITION_OUTFIT;
						} else if (strValue == "invisible") {
							prop.condition = CONDITION_INVISIBLE;
						} else if (strValue == "light") {
							prop.condition = CONDITION_LIGHT;
						} else if (strValue == "manashield") {
							prop.condition = CONDITION_MANASHIELD;
						} else if (strValue == "drunk") {
							prop.condition = CONDITION_DRUNK;
						} else if (strValue == "exhausted") {
							prop.condition = CONDITION_EXHAUST_COMBAT;
						} else if (strValue == "regeneration") {
							prop.condition = CONDITION_REGENERATION;
						} else if (strValue == "soul") {
							prop.condition = CONDITION_SOUL;
						} else if (strValue == "drown") {
							prop.condition = CONDITION_DROWN;
						} else if (strValue == "muted") {
							prop.condition = CONDITION_MUTED;
						}
					}

					if (readXMLInteger(tmpNode, "b1", intValue)) {
						scriptVars.b1 = (intValue == 1);
					}

					if (readXMLInteger(tmpNode, "b2", intValue)) {
						scriptVars.b2 = (intValue == 1);
					}

					if (readXMLInteger(tmpNode, "b3", intValue)) {
						scriptVars.b3 = (intValue == 1);
					}

					if (readXMLInteger(tmpNode, "n1", intValue)) {
						scriptVars.n1 = intValue;
					}

					if (readXMLInteger(tmpNode, "n2", intValue)) {
						scriptVars.n2 = intValue;
					}

					if (readXMLInteger(tmpNode, "n3", intValue)) {
						scriptVars.n3 = intValue;
					}

					ResponseList subResponseList;

					xmlNodePtr subNode = tmpNode->children;

					while (subNode) {
						if (xmlStrcmp(subNode->name, (const xmlChar*)"action") == 0) {
							ResponseAction action;

							if (readXMLString(subNode, "name", strValue)) {
								std::string tmpStr = asLowerCaseString(strValue);

								if (tmpStr == "topic") {
									if (readXMLInteger(subNode, "value", intValue)) {
										action.actionType = ACTION_SETTOPIC;
										action.intValue = intValue;
									} else {
										std::cout << "Warning: [Npc::loadInteraction] [Action 'name'] Could not find attribute 'value' " << std::endl;
									}
								} else if (tmpStr == "price") {
									if (readXMLString(subNode, "value", strValue)) {
										action.actionType = ACTION_SETPRICE;
										action.strValue = strValue;
										action.intValue = atoi(strValue.c_str());
									} else {
										std::cout << "Warning: [Npc::loadInteraction] [Action 'price'] Could not find attribute 'value' " << std::endl;
									}
								} else if (tmpStr == "amount") {
									if (readXMLString(subNode, "value", strValue)) {
										action.actionType = ACTION_SETAMOUNT;
										action.strValue = strValue;
										action.intValue = atoi(strValue.c_str());
									} else {
										std::cout << "Warning: [Npc::loadInteraction] [Action 'amount'] Could not find attribute 'value' " << std::endl;
									}
								} else if (tmpStr == "item") {
									if (readXMLInteger(subNode, "value", intValue)) {
										action.actionType = ACTION_SETITEM;
										action.intValue = intValue;
									} else {
										std::cout << "Warning: [Npc::loadInteraction] [Action 'item'] Could not find attribute 'value' " << std::endl;
									}
								} else if (tmpStr == "subtype") {
									if (readXMLInteger(subNode, "value", intValue)) {
										action.actionType = ACTION_SETSUBTYPE;
										action.intValue = intValue;
									} else {
										std::cout << "Warning: [Npc::loadInteraction] [Action 'subtype'] Could not find attribute 'value' " << std::endl;
									}
								} else if (tmpStr == "spell") {
									if (readXMLString(subNode, "value", strValue)) {
										action.actionType = ACTION_SETSPELL;
										action.strValue = strValue;

										if (strValue != "|SPELL|") {
											InstantSpell* spell = g_spells->getInstantSpellByName(strValue);

											if (!spell) {
												std::cout << "Warning: [Npc::loadInteraction] Could not find an instant spell called " << strValue << std::endl;
											}
										}
									} else {
										std::cout << "Warning: [Npc::loadInteraction] [Action 'spell'] Could not find attribute 'value' " << std::endl;
									}
								} else if (tmpStr == "listname") {
									if (readXMLString(subNode, "value", strValue)) {
										action.actionType = ACTION_SETLISTNAME;
										action.strValue = strValue;
									} else {
										std::cout << "Warning: [Npc::loadInteraction] [Action 'listname'] Could not find attribute 'value' " << std::endl;
									}
								} else if (tmpStr == "listpname") {
									if (readXMLString(subNode, "value", strValue)) {
										action.actionType = ACTION_SETLISTPNAME;
										action.strValue = strValue;
									} else {
										std::cout << "Warning: [Npc::loadInteraction] [Action 'listpname'] Could not find attribute 'value' " << std::endl;
									}
								} else if (tmpStr == "teachspell") {
									if (readXMLString(subNode, "value", strValue)) {
										action.actionType = ACTION_TEACHSPELL;
										action.strValue = strValue;

										if (strValue != "|SPELL|") {
											InstantSpell* spell = g_spells->getInstantSpellByName(strValue);

											if (!spell) {
												std::cout << "Warning: [Npc::loadInteraction] Could not find an instant spell called " << strValue << std::endl;
											}
										}
									} else {
										std::cout << "Warning: [Npc::loadInteraction] [Action 'teachspell'] Could not find attribute 'value' " << std::endl;
									}
								} else if (tmpStr == "sell") {
									if (readXMLString(subNode, "value", strValue)) {
										action.actionType = ACTION_SELLITEM;
										action.strValue = strValue;
										action.intValue = atoi(strValue.c_str());
									} else {
										std::cout << "Warning: [Npc::loadInteraction] [Action 'sell'] Could not find attribute 'value' " << std::endl;
									}
								} else if (tmpStr == "buy") {
									if (readXMLString(subNode, "value", strValue)) {
										action.actionType = ACTION_BUYITEM;
										action.strValue = strValue;
										action.intValue = atoi(strValue.c_str());
									} else {
										std::cout << "Warning: [Npc::loadInteraction] [Action 'buy'] Could not find attribute 'value' " << std::endl;
									}
								} else if (tmpStr == "takemoney") {
									if (readXMLString(subNode, "value", strValue)) {
										action.actionType = ACTION_TAKEMONEY;
										action.strValue = strValue;
										action.intValue = atoi(strValue.c_str());
									} else {
										std::cout << "Warning: [Npc::loadInteraction] [Action 'takemoney'] Could not find attribute 'value' " << std::endl;
									}
								} else if (tmpStr == "givemoney") {
									if (readXMLString(subNode, "value", strValue)) {
										action.actionType = ACTION_GIVEMONEY;
										action.strValue = strValue;
										action.intValue = atoi(strValue.c_str());
									} else {
										std::cout << "Warning: [Npc::loadInteraction] [Action 'givemoney'] Could not find attribute 'value' " << std::endl;
									}
								} else if (tmpStr == "level") {
									if (readXMLString(subNode, "value", strValue)) {
										action.actionType = ACTION_SETLEVEL;
										action.strValue = strValue;
										action.intValue = atoi(strValue.c_str());
									} else {
										std::cout << "Warning: [Npc::loadInteraction] [Action 'level'] Could not find attribute 'value' " << std::endl;
									}
								} else if (tmpStr == "giveitem") {
									if (readXMLString(subNode, "value", strValue)) {
										action.actionType = ACTION_GIVEITEM;
										action.strValue = strValue;
										action.intValue = atoi(strValue.c_str());
									} else {
										std::cout << "Warning: [Npc::loadInteraction] [Action 'giveitem'] Could not find attribute 'value' " << std::endl;
									}
								} else if (tmpStr == "takeitem") {
									if (readXMLString(subNode, "value", strValue)) {
										action.actionType = ACTION_TAKEITEM;
										action.strValue = strValue;
										action.intValue = atoi(strValue.c_str());
									} else {
										std::cout << "Warning: [Npc::loadInteraction] [Action 'takeitem'] Could not find attribute 'value' " << std::endl;
									}
								} else if (tmpStr == "effect") {
									if (readXMLString(subNode, "value", strValue)) {
										action.actionType = ACTION_SETEFFECT;
										action.intValue = getMagicEffect(strValue);
									} else {
										std::cout << "Warning: [Npc::loadInteraction] [Action 'effect'] Could not find attribute 'value' " << std::endl;
									}
								} else if (tmpStr == "idle") {
									if (readXMLInteger(subNode, "value", intValue)) {
										action.actionType = ACTION_SETIDLE;
										action.intValue = intValue;
									} else {
										std::cout << "Warning: [Npc::loadInteraction] [Action 'idle'] Could not find attribute 'value' " << std::endl;
									}
								} else if (tmpStr == "script") {
									if (readXMLString(subNode, "value", strValue)) {
										action.actionType = ACTION_SCRIPT;
										action.strValue = strValue;
									} else if (subNode->children) {
										xmlNodePtr scriptNode = subNode->children;

										while (scriptNode) {
											if (xmlStrcmp(scriptNode->name, (const xmlChar*)"text") == 0 ||
											        scriptNode->type == XML_CDATA_SECTION_NODE) {
												if (readXMLContentString(scriptNode, strValue)) {
													trim_left(strValue, "\r");
													trim_left(strValue, "\n");
													trim_left(strValue, " ");

													if (strValue.length() > action.strValue.length()) {
														action.actionType = ACTION_SCRIPT;
														action.strValue = strValue;
													}
												}
											}

											scriptNode = scriptNode->next;
										}
									} else {
										std::cout << "Warning: [Npc::loadInteraction] [Action 'script'] Could not find attribute 'value' " << std::endl;
									}
								} else if (tmpStr == "scriptparam") {
									if (readXMLString(subNode, "value", strValue)) {
										action.actionType = ACTION_SCRIPTPARAM;
										action.strValue = strValue;
									} else {
										std::cout << "Warning: [Npc::loadInteraction] [Action 'scriptparam'] Could not find attribute 'value' " << std::endl;
									}
								} else if (tmpStr == "storage") {
									if (readXMLInteger(subNode, "value", intValue)) {
										action.actionType = ACTION_SETSTORAGE;
										action.intValue = intValue;
									} else {
										std::cout << "Warning: [Npc::loadInteraction] [Action 'storage'] Could not find attribute 'value' " << std::endl;
									}
								} else if (tmpStr == "addqueue") {
									if (readXMLString(subNode, "value", strValue)) {
										action.actionType = ACTION_ADDQUEUE;
										action.strValue = strValue;
										action.intValue = atoi(strValue.c_str());
									} else {
										std::cout << "Warning: [Npc::loadInteraction] [Action 'addqueue'] Could not find attribute 'value' " << std::endl;
									}
								} else if (tmpStr == "teleport") {
									if (readXMLString(subNode, "value", strValue)) {
										std::vector<std::string> posList = explodeString(strValue, ";");
										action.actionType = ACTION_SETTELEPORT;
										action.strValue = strValue;
										action.pos.x = 0;
										action.pos.y = 0;
										action.pos.z = 0;

										if (posList.size() == 3) {
											action.pos.x = atoi(posList[0].c_str());
											action.pos.y = atoi(posList[1].c_str());
											action.pos.z = atoi(posList[2].c_str());
										}
									} else {
										std::cout << "Warning: [Npc::loadInteraction] [Action 'teleport'] Could not find attribute 'value' " << std::endl;
									}
								} else {
									std::cout << "Warning: [Npc::loadInteraction] Unknown action " << strValue << std::endl;
								}
							}

							if (readXMLInteger(subNode, "key", intValue)) {
								action.key = intValue;
							}

							if (action.actionType != ACTION_NONE) {
								prop.actionList.push_back(action);
							}
						} else if (xmlStrcmp(subNode->name, (const xmlChar*)"interact") == 0) {
							if (subResponseList.empty()) {
								ResponseList nodeResponseList = loadInteraction(subNode);
								subResponseList.insert(subResponseList.end(),
								                       nodeResponseList.begin(), nodeResponseList.end());
							}
						}

						subNode = subNode->next;
					}

					//Check if this interaction has a |list| keyword
					bool hasListKeyword = false;

					for (std::list<std::string>::iterator it = prop.inputList.begin();
					        it != prop.inputList.end(); ++it) {
						if (it->find("|list|") != std::string::npos) {
							hasListKeyword = true;
							break;
						}
					}

					//Iterate through all input keywords and replace all |LIST| with the item list
					if (hasListKeyword && !prop.itemList.empty()) {
						for (std::list<ListItem>::iterator it = prop.itemList.begin(); it != prop.itemList.end(); ++it) {
							NpcResponse::ResponseProperties listItemProp = prop;

							for (std::list<std::string>::iterator iit = listItemProp.inputList.begin();
							        iit != listItemProp.inputList.end(); ++iit) {
								std::string& input = (*iit);

								if (input.find("|list|") == std::string::npos) {
									continue;
								}

								//Replace |LIST| with the keyword in the list
								replaceString(input, "|list|", it->keywords);

								ResponseAction action;

								action.actionType = ACTION_SETITEM;
								action.intValue = it->itemId;
								listItemProp.actionList.push_front(action);

								action.actionType = ACTION_SETSELLPRICE;
								action.intValue = it->sellPrice;
								listItemProp.actionList.push_front(action);

								action.actionType = ACTION_SETBUYPRICE;
								action.intValue = it->buyPrice;
								listItemProp.actionList.push_front(action);

								action.actionType = ACTION_SETSUBTYPE;
								action.intValue = it->subType;
								listItemProp.actionList.push_front(action);

								action.actionType = ACTION_SETLISTNAME;

								if (!it->name.empty()) {
									action.strValue = it->name;
								} else {
									const ItemType& itemType = Item::items[it->itemId];

									if (itemType.id != 0) {
										action.strValue = itemType.article + " " + itemType.name;
									}
								}

								listItemProp.actionList.push_front(action);

								action.actionType = ACTION_SETLISTPNAME;

								if (!it->pluralName.empty()) {
									action.strValue = it->pluralName;
								} else {
									const ItemType& itemType = Item::items[it->itemId];

									if (itemType.id != 0) {
										action.strValue = itemType.getPluralName();
									}
								}

								listItemProp.actionList.push_front(action);

								ResponseList list;

								for (ResponseList::iterator respIter = subResponseList.begin();
								        respIter != subResponseList.end(); ++respIter) {
									list.push_back(new NpcResponse(*(*respIter)));
								}

								//Create a new response for this list item
								NpcResponse* response = new NpcResponse(listItemProp, list, scriptVars);
								_responseList.push_back(response);
							}
						}
					} else {
						NpcResponse* response = new NpcResponse(prop, subResponseList, scriptVars);
						_responseList.push_back(response);
					}
				}

				tmpNode = tmpNode->next;
			}
		}

		node = node->next;
	}

	return _responseList;
}

NpcState* Npc::getState(const Player* player, bool makeNew /*= true*/)
{
	for (StateList::iterator it = stateList.begin(); it != stateList.end(); ++it) {
		if ((*it)->playerId == player->getID()) {
			return *it;
		}
	}

	if (!makeNew) {
		return NULL;
	}

	NpcState* state = new NpcState;
	state->playerId = player->getID();
	state->price = 0;
	state->sellPrice = 0;
	state->buyPrice = 0;
	state->amount = 1;
	state->itemId = 0;
	state->subType = -1;
	state->ignore = false;
	state->inBackpacks = false;
	state->spellName = "";
	state->listName = "";
	state->listPluralName = "";
	state->level = -1;
	state->topic = 0;
	state->focusState = -1;
	state->isQueued = false;
	state->respondToText = "";
	state->lastResponse = NULL;
	state->subResponse = NULL;
	state->lastResponseTime = OTSYS_TIME();
	stateList.push_back(state);
	return state;
}

bool Npc::canSee(const Position& pos) const
{
	if (pos.z != getPosition().z) {
		return false;
	}

	return Creature::canSee(getPosition(), pos, talkRadius, talkRadius);
}

std::string Npc::getDescription(int32_t lookDistance) const
{
	std::ostringstream s;
	s << name << ".";
	return s.str();
}

void Npc::onCreatureAppear(const Creature* creature, bool isLogin)
{
	Creature::onCreatureAppear(creature, isLogin);

	if (creature == this && walkTicks > 0) {
		addEventWalk();
	}

	if (creature == this) {
		if (m_npcEventHandler) {
			m_npcEventHandler->onCreatureAppear(creature);
		}
	}
	//only players for script events
	else if (Player* player = const_cast<Player*>(creature->getPlayer())) {
		if (m_npcEventHandler) {
			m_npcEventHandler->onCreatureAppear(creature);
		}

		NpcState* npcState = getState(player);

		if (npcState && canSee(player->getPosition())) {
			onPlayerEnter(player, npcState);
		}
	}
}

void Npc::onCreatureDisappear(const Creature* creature, uint32_t stackpos, bool isLogout)
{
	Creature::onCreatureDisappear(creature, stackpos, isLogout);

	if (creature == this) {
		//Close all open shop window's
		closeAllShopWindows();

		/*
		Can't use this yet because Jiddo's scriptsystem isn't able to handle it.
		if(m_npcEventHandler){
			m_npcEventHandler->onCreatureDisappear(creature);
		}
		*/
	} else if (Player* player = const_cast<Player*>(creature->getPlayer())) {
		//only players for script events
		if (m_npcEventHandler) {
			m_npcEventHandler->onCreatureDisappear(creature);
		}

		NpcState* npcState = getState(player);

		if (npcState) {
			onPlayerLeave(player, npcState);
		}
	}
}

void Npc::onCreatureMove(const Creature* creature, const Tile* newTile, const Position& newPos,
                         const Tile* oldTile, const Position& oldPos, bool teleport)
{
	Creature::onCreatureMove(creature, newTile, newPos, oldTile, oldPos, teleport);

	if (creature == this) {
		if (m_npcEventHandler) {
			m_npcEventHandler->onCreatureMove(creature, oldPos, newPos);
		}
	} else if (Player* player = const_cast<Player*>(creature->getPlayer())) {
		if (m_npcEventHandler) {
			m_npcEventHandler->onCreatureMove(creature, oldPos, newPos);
		}

		bool canSeeNewPos = canSee(newPos);
		bool canSeeOldPos = canSee(oldPos);

		if (canSeeNewPos && !canSeeOldPos) {
			NpcState* npcState = getState(player);

			if (npcState) {
				onPlayerEnter(player, npcState);
			}
		} else if (!canSeeNewPos && canSeeOldPos) {
			NpcState* npcState = getState(player);

			if (npcState) {
				onPlayerLeave(player, npcState);
			}
		} else if (canSeeNewPos && canSeeOldPos) {
			NpcState* npcState = getState(player);

			if (npcState) {
				const NpcResponse* response = getResponse(player, npcState, EVENT_PLAYER_MOVE, false);
				processResponse(player, npcState, response);
			}
		}
	}
}

void Npc::onCreatureSay(const Creature* creature, SpeakClasses type, const std::string& text, Position* pos/* = NULL*/)
{
	if (creature->getID() == this->getID()) {
		return;
	}

	//only players for script events
	if (Player* player = const_cast<Player*>(creature->getPlayer())) {
		if (m_npcEventHandler) {
			m_npcEventHandler->onCreatureSay(player, type, text);
		}

		if (type == SPEAK_SAY || type == SPEAK_PRIVATE_PN) {
			const Position& pos = creature->getPosition();

			if (canSee(pos)) {
				NpcState* npcState = getState(player);

				npcState->respondToText = text;

				if (!text.empty()) {
					if (hasBusyReply && focusCreature != 0 && (uint32_t)focusCreature != player->getID()) {
						//Check if we have a busy reply
						const NpcResponse* response = getResponse(player, npcState, EVENT_BUSY, text, false);

						if (response) {
							turnToCreature(player);
							processResponse(player, npcState, response, true);
						}
					} else {
						const NpcResponse* response = getResponse(player, npcState, text, true);

						if (response) {
							setCreatureFocus(player);
							processResponse(player, npcState, response, true);
						}
					}
				}
			}
		}
	}
}

void Npc::onPlayerCloseChannel(const Player* player)
{
	if (m_npcEventHandler) {
		m_npcEventHandler->onPlayerCloseChannel(player);
	}
}

void Npc::onPlayerEnter(Player* player, NpcState* state)
{
	const NpcResponse* response = getResponse(player, state, EVENT_PLAYER_ENTER, false);
	processResponse(player, state, response);
}

void Npc::onPlayerLeave(Player* player, NpcState* state)
{
	if (player) {
		player->closeShopWindow();
		const NpcResponse* response = getResponse(player, state, EVENT_PLAYER_LEAVE, false);
		processResponse(player, state, response);

		if (state->focusState == -1) {
			//has not started a conversation yet
			state->focusState = 0;
		}
	}
}

void Npc::onThink(uint32_t interval)
{
	Creature::onThink(interval);

	if (m_npcEventHandler) {
		m_npcEventHandler->onThink();
	}

	if (getTimeSinceLastMove() >= walkTicks) {
		addEventWalk();
	}

	isIdle = true;
	hasUsedIdleReply = false;

	for (StateList::iterator it = stateList.begin(); it != stateList.end();) {
		NpcState* npcState = *it;

		const NpcResponse* response = NULL;
		Player* player = g_game.getPlayerByID(npcState->playerId);
		bool closeConversation = (player == NULL);
		bool closeDueToTimeout = false;

		if (!npcState->isQueued) {
			if (npcState->focusState == -1) {
				//has not started a conversation yet
			} else if (npcState->focusState == 0) {
				//closing conversation because focus is set to 0 or isidle set to true
				closeConversation = true;
			} else if (idleTimeout > 0 && (OTSYS_TIME() - npcState->lastResponseTime) > idleTimeout * 1000) {
				//closing conversation due to idle
				closeDueToTimeout = true;
				closeConversation = true;
			}
		}

		//execute our latest response
		if (player && npcState->lastResponse) {
			turnToCreature(player);
			executeResponse(player, npcState, npcState->lastResponse);
			npcState->subResponse = npcState->lastResponse;
			npcState->lastResponse = NULL;
		}

		if (closeConversation) {
			if ((uint32_t)focusCreature == npcState->playerId && !hasScriptedFocus) {
				setCreatureFocus(NULL);
			}

			if (queueList.empty()) {
				if (closeDueToTimeout) {
					onPlayerLeave(player, npcState);
				}
			} else {
				Player* nextPlayer = NULL;

				while (!queueList.empty()) {
					nextPlayer = g_game.getPlayerByID(*queueList.begin());
					queueList.erase(queueList.begin());

					if (nextPlayer) {
						NpcState* nextPlayerState = getState(nextPlayer, false);

						if (nextPlayerState) {
							response = getResponse(nextPlayer, nextPlayerState, nextPlayerState->respondToText, true);

							if (response) {
								setCreatureFocus(nextPlayer);
								processResponse(nextPlayer, nextPlayerState, response, true);
							}

							nextPlayerState->isQueued = false;
							break;
						}
					}
				}
			}

			delete *it;
			stateList.erase(it++);
			//std::cout << "Closing conversation." << std::endl;

			continue;
		}

		//idle response
		response = getResponse(player, npcState, EVENT_IDLE, true);

		if (response) {
			hasUsedIdleReply = true;
			processResponse(player, npcState, response);
		}

		//think response
		response = getResponse(player, npcState, EVENT_THINK, true);

		if (response) {
			processResponse(player, npcState, response);
		}

		if (npcState->focusState == 1 || !queueList.empty()) {
			isIdle = false;
		}

		++it;
	}

	if (isIdle && !hasScriptedFocus) {
		setCreatureFocus(NULL);
	}
}

void Npc::processResponse(Player* player, NpcState* npcState, const NpcResponse* response, bool delayResponse /*= false*/)
{
	if (response) {
		bool resetTopic = true;

		if (response->getFocusState() == 0) {
			npcState->focusState = 0;
		} else if (response->getFocusState() == 1) {
			npcState->focusState = 1;
		}

		if (response->getAmount() != -1) {
			if (npcState->itemId > 0) {
				const ItemType& it = Item::items[npcState->itemId];

				if (it.id != 0 && it.stackable == false) {
					npcState->amount = std::min<int32_t>(response->getAmount(), 100);
				} else {
					npcState->amount = response->getAmount();
				}
			} else {
				npcState->amount = response->getAmount();
			}
		}

		for (ActionList::const_iterator it = response->getFirstAction(); it != response->getEndAction(); ++it) {
			switch (it->actionType) {
				case ACTION_SETTOPIC:
					npcState->topic = std::max<int32_t>(it->intValue, 0);
					resetTopic = false;
					break;
				case ACTION_SETSELLPRICE:
					npcState->sellPrice = it->intValue;
					break;
				case ACTION_SETBUYPRICE:
					npcState->buyPrice = it->intValue;
					break;
				case ACTION_SETITEM:
					npcState->itemId = it->intValue;
					break;
				case ACTION_SETSUBTYPE:
					npcState->subType = it->intValue;
					break;
				case ACTION_SETEFFECT:
					g_game.addMagicEffect(player->getPosition(), it->intValue);
					break;
				case ACTION_SETPRICE: {
					if (it->strValue == "|SELLPRICE|") {
						npcState->price = npcState->sellPrice;
					} else if (it->strValue == "|BUYPRICE|") {
						npcState->price = npcState->buyPrice;
					} else {
						npcState->price = it->intValue;
					}

					break;
				}
				case ACTION_SETTELEPORT: {
					Position teleportTo = it->pos;

					if (it->strValue == "|TEMPLE|") {
						teleportTo = player->getTemplePosition();
					}

					g_game.internalTeleport(player, teleportTo);
					break;
				}

				case ACTION_SETIDLE: {
					npcState->focusState = 0;
					break;
				}

				case ACTION_SETLEVEL: {
					if (it->strValue == "|SPELLLEVEL|") {
						npcState->level = -1;
						InstantSpell* spell = g_spells->getInstantSpellByName(npcState->spellName);

						if (spell) {
							npcState->level = spell->getLevel();
						}
					} else {
						npcState->level = it->intValue;
					}

					break;
				}

				case ACTION_SETSPELL: {
					InstantSpell* spell = g_spells->getInstantSpellByName(it->strValue);

					if (spell) {
						npcState->spellName = it->strValue;
					} else {
						npcState->spellName = "";
					}

					break;
				}

				case ACTION_SETLISTNAME: {
					npcState->listName = it->strValue;
					break;
				}

				case ACTION_SETLISTPNAME: {
					npcState->listPluralName = it->strValue;
					break;
				}

				case ACTION_SETAMOUNT: {
					int32_t amount;

					if (it->strValue == "|AMOUNT|") {
						amount = npcState->amount;
					} else {
						amount = it->intValue;
					}

					if (npcState->itemId > 0 && amount > 100) {
						amount = 100;
					}

					npcState->amount = amount;
					break;
				}

				case ACTION_TEACHSPELL: {
					std::string spellName;

					if (it->strValue == "|SPELL|") {
						spellName = npcState->spellName;
					} else {
						spellName = it->strValue;
					}

					player->learnInstantSpell(spellName);
					break;
				}

				case ACTION_SETSTORAGE: {
					if (it->key > 0) {
						player->addStorageValue(it->key, it->intValue);
					}

					break;
				}

				case ACTION_ADDQUEUE: {
					QueueList::iterator it = std::find(queueList.begin(), queueList.end(), player->getID());

					if (it == queueList.end()) {
						queueList.push_back(player->getID());
						npcState->isQueued = true;
					}

					break;
				}

				case ACTION_SELLITEM: {
					uint64_t moneyCount;

					if (it->strValue == "|PRICE|") {
						moneyCount = npcState->price * npcState->amount;
					} else {
						moneyCount = it->intValue;
					}

					const ItemType& it = Item::items[npcState->itemId];

					if (it.id != 0) {
						int32_t subType;

						if (it.hasSubType()) {
							subType = npcState->subType;
						} else {
							subType = -1;
						}

						int32_t itemCount = player->__getItemTypeCount(it.id, subType);

						if (itemCount >= npcState->amount) {
							g_game.removeItemOfType(player, it.id, npcState->amount, subType);
							g_game.addMoney(player, moneyCount, FLAG_NOLIMIT);
						}
					}

					break;
				}

				case ACTION_BUYITEM: {
					uint64_t moneyCount;

					if (it->strValue == "|PRICE|") {
						moneyCount = npcState->price * npcState->amount;
					} else {
						moneyCount = it->intValue;
					}

					const ItemType& it = Item::items[npcState->itemId];

					if (it.id != 0) {
						int32_t subType;

						if (it.hasSubType()) {
							subType = npcState->subType;
						} else {
							subType = -1;
						}

						if (g_game.removeMoney(player, moneyCount)) {
							if (it.stackable) {
								int32_t amount = npcState->amount;

								while (amount > 0) {
									int32_t stackCount = std::min<int32_t>(100, amount);
									Item* item = Item::CreateItem(it.id, stackCount);

									if (g_game.internalPlayerAddItem(player, item) != RET_NOERROR) {
										delete item;
										break;
									}

									amount -= stackCount;
								}
							} else {
								for (int32_t i = 0; i < npcState->amount; ++i) {
									Item* item = Item::CreateItem(it.id, subType);

									if (g_game.internalPlayerAddItem(player, item) != RET_NOERROR) {
										delete item;
										break;
									}
								}
							}
						} else {
							std::cout << "Error [Npc::processResponse] Not enough money: " << player->getName() << "\tNpc: " << getName() << std::endl;
						}
					}

					break;
				}

				case ACTION_TAKEITEM: {
					int32_t itemId;

					if (it->strValue == "|ITEM|") {
						itemId = npcState->itemId;
					} else {
						itemId = it->intValue;
					}

					const ItemType& it = Item::items[npcState->itemId];

					if (it.id != 0) {
						int32_t subType;

						if (it.hasSubType()) {
							subType = npcState->subType;
						} else {
							subType = -1;
						}

						int32_t itemCount = player->__getItemTypeCount(itemId, subType);

						if (itemCount >= npcState->amount) {
							g_game.removeItemOfType(player, itemId, npcState->amount, subType);
						}
					}

					break;
				}

				case ACTION_GIVEITEM: {
					int32_t itemId;

					if (it->strValue == "|ITEM|") {
						itemId = npcState->itemId;
					} else {
						itemId = it->intValue;
					}

					const ItemType& it = Item::items[itemId];

					if (it.id != 0) {
						int32_t subType;

						if (it.hasSubType()) {
							subType = npcState->subType;
						} else {
							subType = -1;
						}

						for (int32_t i = 0; i < npcState->amount; ++i) {
							Item* item = Item::CreateItem(it.id, subType);

							if (g_game.internalPlayerAddItem(player, item) != RET_NOERROR) {
								delete item;
							}
						}
					}

					break;
				}

				case ACTION_TAKEMONEY: {
					uint64_t moneyCount;

					if (it->strValue == "|PRICE|") {
						moneyCount = npcState->price * npcState->amount;
					} else {
						moneyCount = it->intValue;
					}

					g_game.removeMoney(player, moneyCount);
					break;
				}

				case ACTION_GIVEMONEY: {
					uint64_t moneyCount;

					if (it->strValue == "|PRICE|") {
						moneyCount = npcState->price * npcState->amount;
					} else {
						moneyCount = it->intValue;
					}

					g_game.addMoney(player, moneyCount);
					break;
				}

				case ACTION_SCRIPT: {
					if (m_scriptInterface->reserveScriptEnv()) {
						ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
						env->setRealPos(getPosition());
						env->setNpc(this);

						std::ostringstream scriptstream;
						//attach various variables that could be interesting
						scriptstream << "cid = " << player->getID() << std::endl;
						scriptstream << "text = \"" << LuaScriptInterface::escapeString(npcState->respondToText) << "\"" << std::endl;
						scriptstream << "name = \"" << LuaScriptInterface::escapeString(player->getName()) << "\"" << std::endl;
						scriptstream << "idleTimeout = " << idleTimeout << std::endl;

						scriptstream << "itemlist = {" << std::endl;
						uint32_t n = 0;

						for (std::list<ListItem>::const_iterator iit = response->prop.itemList.begin(); iit != response->prop.itemList.end(); ++iit) {
							bool addDelim = (n + 1 != response->prop.itemList.size());
							scriptstream << "{id = " << iit->itemId
							             << ", subtype = " << iit->subType
							             << ", buy=" << iit->buyPrice
							             << ", sell=" << iit->sellPrice << "}";

							if (addDelim) {
								scriptstream << "," << std::endl;
							}

							++n;
						}

						scriptstream << "}" << std::endl;

						scriptstream << "_state = {" << std::endl;
						scriptstream << "topic = " << npcState->topic << ',' << std::endl;
						scriptstream << "itemid = " << npcState->itemId << ',' << std::endl;
						scriptstream << "subtype = " << npcState->subType << ',' << std::endl;
						scriptstream << "amount = " << npcState->amount << ',' << std::endl;
						scriptstream << "price = " << npcState->price << ',' << std::endl;
						scriptstream << "level = " << npcState->level << ',' << std::endl;
						scriptstream << "spellname = \"" << LuaScriptInterface::escapeString(npcState->spellName) << "\"" << ',' << std::endl;
						scriptstream << "listname = \"" << LuaScriptInterface::escapeString(npcState->listName) << "\"" << ',' << std::endl;
						scriptstream << "listpname = \"" << LuaScriptInterface::escapeString(npcState->listPluralName) << "\"" << ',' << std::endl;

						scriptstream << "n1 = " << npcState->scriptVars.n1 << ',' << std::endl;
						scriptstream << "n2 = " << npcState->scriptVars.n2 << ',' << std::endl;
						scriptstream << "n3 = " << npcState->scriptVars.n3 << ',' << std::endl;

						scriptstream << "b1 = " << (npcState->scriptVars.b1 ? "true" : "false" ) << ',' << std::endl;
						scriptstream << "b2 = " << (npcState->scriptVars.b2 ? "true" : "false" ) << ',' << std::endl;
						scriptstream << "b3 = " << (npcState->scriptVars.b3 ? "true" : "false" ) << ',' << std::endl;

						scriptstream << "s1 = \"" << LuaScriptInterface::escapeString(npcState->scriptVars.s1) << "\"" << ',' << std::endl;
						scriptstream << "s2 = \"" << LuaScriptInterface::escapeString(npcState->scriptVars.s2) << "\"" << ',' << std::endl;
						scriptstream << "s3 = \"" << LuaScriptInterface::escapeString(npcState->scriptVars.s3) << "\"" << std::endl;
						scriptstream << "}" << std::endl;

						scriptstream << it->strValue;

						//std::cout << scriptstream.str() << std::endl;
						if (m_scriptInterface->loadBuffer(scriptstream.str(), NULL) != -1) {
							lua_State* L = m_scriptInterface->getLuaState();
							lua_getglobal(L, "_state");
							NpcScriptInterface::popState(L, npcState);
						}

						m_scriptInterface->releaseScriptEnv();
					}

					break;
				}

				default:
					break;
			}
		}

		if (resetTopic && response->getTopic() == npcState->topic) {
			npcState->topic = 0;
		}

		lastResponseTime = OTSYS_TIME();
		npcState->lastResponseTime = lastResponseTime;

		if (delayResponse) {
			npcState->lastResponse = response;
		} else {
			turnToCreature(player);
			executeResponse(player, npcState, response);
			npcState->subResponse = response;
			npcState->lastResponse = NULL;
		}
	}
}

void Npc::executeResponse(Player* player, NpcState* npcState, const NpcResponse* response)
{
	if (response->getResponseType() == RESPONSE_DEFAULT) {
		std::string responseString = formatResponse(player, npcState, response);

		if (!responseString.empty()) {
			if (response->publicize()) {
				doSay(responseString);
			} else {
				doSayToPlayer(player, responseString);
			}
		}
	} else {
		int32_t functionId = -1;
		ResponseScriptMap::iterator it = responseScriptMap.find(response->getText());

		if (it != responseScriptMap.end()) {
			functionId = it->second;
		} else {
			functionId = m_scriptInterface->getEvent(response->getText());
			responseScriptMap[response->getText()] = functionId;
		}

		if (functionId != -1) {
			if (m_scriptInterface->reserveScriptEnv()) {
				ScriptEnvironment* env = m_scriptInterface->getScriptEnv();

				lua_State* L = m_scriptInterface->getLuaState();

				env->setScriptId(functionId, m_scriptInterface);
				env->setRealPos(getPosition());
				env->setNpc(this);

				m_scriptInterface->pushFunction(functionId);
				int32_t paramCount = 0;

				for (ActionList::const_iterator it = response->getFirstAction(); it != response->getEndAction(); ++it) {
					if (it->actionType == ACTION_SCRIPTPARAM) {
						if (it->strValue == "|PLAYER|") {
							lua_pushnumber(L, player->getID());
						} else if (it->strValue == "|TEXT|") {
							lua_pushstring(L, npcState->respondToText.c_str());
						} else {
							std::cout << "Warning [Npc::processResponse] Unknown script param: " << it->strValue << std::endl;
							break;
						}

						++paramCount;
					}
				}

				NpcScriptInterface::pushState(L, npcState);
				lua_setglobal(L, "_state");
				m_scriptInterface->callFunction(paramCount);
				lua_getglobal(L, "_state");
				NpcScriptInterface::popState(L, npcState);
				m_scriptInterface->releaseScriptEnv();
			} else {
				std::cout << "[Error] Call stack overflow." << std::endl;
			}
		}
	}
}

void Npc::doSay(const std::string& text)
{
	g_game.internalCreatureSay(this, SPEAK_SAY, text, false);
}

void Npc::doSayToPlayer(Player* player, const std::string& text)
{
	if (player) {
		player->sendCreatureSay(this, SPEAK_PRIVATE_NP, text);
		player->onCreatureSay(this, SPEAK_PRIVATE_NP, text);
	}
}

void Npc::doMove(Direction dir)
{
	g_game.internalMoveCreature(this, dir);
}

void Npc::doTurn(Direction dir)
{
	g_game.internalCreatureTurn(this, dir);
}

uint32_t Npc::getListItemPrice(uint16_t itemId, ShopEvent_t type)
{
	for (ItemListMap::iterator it = itemListMap.begin(); it != itemListMap.end(); ++it) {
		std::list<ListItem>& itemList = it->second;

		for (std::list<ListItem>::iterator iit = itemList.begin(); iit != itemList.end(); ++iit) {
			if (iit->itemId == itemId) {
				if (type == SHOPEVENT_BUY) {
					return iit->buyPrice;
				} else if (type == SHOPEVENT_SELL) {
					return iit->sellPrice;
				}
			}
		}
	}

	return 0;
}

void Npc::onPlayerTrade(Player* player, ShopEvent_t type, int32_t callback, uint16_t itemId,
                        uint8_t count, uint8_t amount, bool ignore/* = false*/, bool inBackpacks/* = false*/)
{
	int8_t subType = -1;
	const ItemType& it = Item::items[itemId];

	if (it.hasSubType() && !it.stackable) {
		subType = count;
	}

	if (type == SHOPEVENT_BUY) {
		NpcState* npcState = getState(player, true);

		if (npcState) {
			npcState->amount = amount;
			npcState->subType = subType;
			npcState->itemId = itemId;
			npcState->buyPrice = getListItemPrice(itemId, SHOPEVENT_BUY);
			npcState->ignore = ignore;
			npcState->inBackpacks = inBackpacks;

			const NpcResponse* response = getResponse(player, npcState, EVENT_PLAYER_SHOPBUY, false);
			processResponse(player, npcState, response);
		}
	} else if (type == SHOPEVENT_SELL) {
		NpcState* npcState = getState(player, true);

		if (npcState) {
			npcState->amount = amount;
			npcState->subType = subType;
			npcState->itemId = itemId;
			npcState->sellPrice = getListItemPrice(itemId, SHOPEVENT_SELL);
			npcState->ignore = ignore;

			const NpcResponse* response = getResponse(player, npcState, EVENT_PLAYER_SHOPSELL, false);
			processResponse(player, npcState, response);
		}
	}

	if (m_npcEventHandler) {
		m_npcEventHandler->onPlayerTrade(player, callback, itemId, count, amount, ignore, inBackpacks);
	}

	player->sendSaleItemList();
}

void Npc::onPlayerEndTrade(Player* player, int32_t buyCallback, int32_t sellCallback)
{
	lua_State* L = getScriptInterface()->getLuaState();

	if (buyCallback != -1) {
		luaL_unref(L, LUA_REGISTRYINDEX, buyCallback);
	}

	if (sellCallback != -1) {
		luaL_unref(L, LUA_REGISTRYINDEX, sellCallback);
	}

	removeShopPlayer(player);

	NpcState* npcState = getState(player);

	if (npcState) {
		const NpcResponse* response = getResponse(player, npcState, EVENT_PLAYER_SHOPCLOSE, false);
		processResponse(player, npcState, response);
	}

	if (m_npcEventHandler) {
		m_npcEventHandler->onPlayerEndTrade(player);
	}
}

bool Npc::getNextStep(Direction& dir, uint32_t& flags)
{
	if (Creature::getNextStep(dir, flags)) {
		return true;
	}

	if (walkTicks <= 0) {
		return false;
	}

	if (!isIdle || focusCreature != 0) {
		return false;
	}

	if (getTimeSinceLastMove() < walkTicks) {
		return false;
	}

	return getRandomStep(dir);
}

bool Npc::canWalkTo(const Position& fromPos, Direction dir)
{
	if (masterRadius == 0) {
		return false;
	}

	Position toPos = getNextPosition(dir, fromPos);

	if (!Spawns::getInstance()->isInZone(masterPos, masterRadius, toPos)) {
		return false;
	}

	Tile* tile = g_game.getTile(toPos.x, toPos.y, toPos.z);

	if (!tile || tile->__queryAdd(0, this, 1, 0) != RET_NOERROR) {
		return false;
	}

	if (!floorChange && (tile->floorChange() || tile->getTeleportItem())) {
		return false;
	}

	return true;
}

bool Npc::getRandomStep(Direction& dir)
{
	std::vector<Direction> dirList;
	const Position& creaturePos = getPosition();

	if (canWalkTo(creaturePos, NORTH)) {
		dirList.push_back(NORTH);
	}

	if (canWalkTo(creaturePos, SOUTH)) {
		dirList.push_back(SOUTH);
	}

	if (canWalkTo(creaturePos, EAST)) {
		dirList.push_back(EAST);
	}

	if (canWalkTo(creaturePos, WEST)) {
		dirList.push_back(WEST);
	}

	if (!dirList.empty()) {
		std::random_shuffle(dirList.begin(), dirList.end());
		dir = dirList[random_range(0, dirList.size() - 1)];
		return true;
	}

	return false;
}

void Npc::doMoveTo(Position target)
{
	std::list<Direction> listDir;

	if (g_game.getPathToEx(this, target, listDir, 1, 1, true, true)) {
		startAutoWalk(listDir);
	}
}

void Npc::turnToCreature(Creature* creature)
{
	const Position& creaturePos = creature->getPosition();
	const Position& myPos = getPosition();
	int32_t dx = myPos.x - creaturePos.x;
	int32_t dy = myPos.y - creaturePos.y;

	Direction dir = SOUTH;
	float tan = 0;

	if (dx != 0) {
		tan = (float)dy / dx;
	} else {
		tan = 10;
	}

	if (std::abs(tan) < 1) {
		if (dx > 0) {
			dir = WEST;
		} else {
			dir = EAST;
		}
	} else {
		if (dy > 0) {
			dir = NORTH;
		} else {
			dir = SOUTH;
		}
	}

	g_game.internalCreatureTurn(this, dir);
}

void Npc::setCreatureFocus(Creature* creature)
{
	if (creature) {
		focusCreature = creature->getID();
		turnToCreature(creature);
	} else {
		focusCreature = 0;
	}
}

const NpcResponse* Npc::getResponse(const ResponseList& list, const Player* player,
                                    NpcState* npcState, const std::string& text, bool exactMatch /*= false*/, NpcEvent_t eventType /*=EVENT_NONE*/)
{
	std::string textString = asLowerCaseString(text);
	std::vector<std::string> wordList = explodeString(textString, " ");

	// We choose the match that matches the most keywords
	// _and_ matches all of it's conditions.
	// If we only have a patial keyword match, we ignore it (all keywords must be matched)

	const NpcResponse* bestMatch = NULL;
	int32_t bestKeywordCount = 0;

	// Cache some info
	int64_t money = -1;

	// First loop we try with current topic
	int32_t currentTopic = npcState->topic;
	// If we get no matches, we (hypothetically) change it to 0 and see if we get any matches
	int32_t loopCount = 1;

	while (loopCount <= 2) {
		if (loopCount == 2) {
			// If it didn't work with 0 before, it won't work now
			if (currentTopic == 0) {
				break;
			} else {
				currentTopic = 0;    // Change to 0 and try again
			}
		}

		loopCount++;

		for (ResponseList::const_iterator it = list.begin(); it != list.end(); ++it) {
			NpcResponse* iresponse = *it;
			uint32_t params = iresponse->getParams();

			if (eventType != EVENT_NONE && iresponse->getEventType() != eventType) {
				continue;
			}

			if (eventType == EVENT_NONE && iresponse->getEventType() != EVENT_NONE) {
				continue;
			}

			if (iresponse->getEventType() == EVENT_NONE || iresponse->getFocusState() != -1) {
				if (npcState->focusState != 1 && iresponse->getFocusState() != 1) {
					continue;    //We are idle, and this response does not activate the npc.
				}

				if (npcState->focusState == 1 && iresponse->getFocusState() == 1) {
					continue;    //We are not idle, and this response would activate us again.
				}
			}

			if (iresponse->getTopic() != -1 && currentTopic != iresponse->getTopic()) {
				continue;    //Not right topic
			}

			// If we got a topic already, we must have response with the same topic
			if (currentTopic != 0 && iresponse->getTopic() != currentTopic) {
				continue;
			}

			if (hasBitSet(RESPOND_MALE, params)) {
				if (!player->isMale()) {
					continue;
				}
			}

			if (hasBitSet(RESPOND_FEMALE, params)) {
				if (!player->isFemale()) {
					continue;
				}
			}

			if (hasBitSet(RESPOND_PZBLOCK, params)) {
				if (!player->isPzLocked()) {
					continue;
				}
			}

			if (hasBitSet(RESPOND_PREMIUM, params)) {
				if (!player->isPremium()) {
					continue;
				}
			}

			// This is an ugly, restrictive hack
			// vocations shouldn't be hardcoded
			if (hasBitSet(RESPOND_DRUID, params)) {
				if (player->getVocationId() != VOCATION_DRUID && player->getVocationId() != VOCATION_ELDERDRUID) {
					continue;
				}
			}

			if (hasBitSet(RESPOND_KNIGHT, params)) {
				if (player->getVocationId() != VOCATION_KNIGHT && player->getVocationId() != VOCATION_ELITEKNIGHT) {
					continue;
				}
			}

			if (hasBitSet(RESPOND_PALADIN, params)) {
				if (player->getVocationId() != VOCATION_PALADIN && player->getVocationId() != VOCATION_ROYALPALADIN) {
					continue;
				}
			}

			if (hasBitSet(RESPOND_SORCERER, params)) {
				if (player->getVocationId() != VOCATION_SORCERER && player->getVocationId() != VOCATION_MASTERSORCERER) {
					continue;
				}
			}

			if (hasBitSet(RESPOND_PROMOTED, params)) {
				if (player->getVocationId() == VOCATION_NONE ||
				        player->getVocationId() == VOCATION_SORCERER ||
				        player->getVocationId() == VOCATION_DRUID ||
				        player->getVocationId() == VOCATION_KNIGHT ||
				        player->getVocationId() == VOCATION_PALADIN) {
					continue;
				}
			}

			if (hasBitSet(RESPOND_LOWLEVEL, params)) {
				if (iresponse->getLevel() > 0) {
					if ((int32_t)player->getLevel() >= iresponse->getLevel()) {
						continue;
					}
				} else {
					if ((int32_t)player->getLevel() >= npcState->level) {
						continue;
					}
				}
			}

			if (hasBitSet(RESPOND_HIGHLEVEL, params)) {
				if (iresponse->getLevel() > 0) {
					if ((int32_t)player->getLevel() < iresponse->getLevel()) {
						continue;
					}
				} else if ((int32_t)player->getLevel() < npcState->level) {
					continue;
				}
			}

			if (hasBitSet(RESPOND_KNOWSPELL, params)) {
				if (!player->hasLearnedInstantSpell(npcState->spellName)) {
					continue;
				}
			}

			if (hasBitSet(RESPOND_CANNOTLEARNSPELL, params)) {
				Spell* spell = g_spells->getInstantSpellByName(npcState->spellName);

				if (!spell) {
					std::cout << "[WARNING]: Non-existant spell in cannotlearnspell tag" << std::endl;
				} else {
					if (player->getLevel() >= spell->getLevel() &&
					        player->getMagicLevel() >= spell->getMagicLevel() &&
					        (spell->isPremium() ? player->isPremium() : true)) {
						continue;
					}
				}
			}

			if (iresponse->getCondition() != CONDITION_NONE) {
				if (!player->hasCondition(iresponse->getCondition())) {
					continue;
				}
			}

			if (iresponse->getHealth() != -1) {
				if (player->getHealth() >= iresponse->getHealth()) {
					continue;
				}
			}

			if (iresponse->getKnowSpell() != "") {
				std::string spellName = iresponse->getKnowSpell();

				if (spellName == "|SPELL|") {
					spellName = npcState->spellName;
				}

				if (!player->hasLearnedInstantSpell(spellName)) {
					continue;
				}
			}

			if (iresponse->scriptVars.b1) {
				if (!npcState->scriptVars.b1) {
					continue;
				}
			}

			if (iresponse->scriptVars.b2) {
				if (!npcState->scriptVars.b2) {
					continue;
				}
			}

			if (iresponse->scriptVars.b3) {
				if (!npcState->scriptVars.b3) {
					continue;
				}
			}

			if (iresponse->scriptVars.n1 != -1) {
				if (npcState->scriptVars.n1 != iresponse->scriptVars.n1) {
					continue;
				}
			}

			if (iresponse->scriptVars.n2 != -1) {
				if (npcState->scriptVars.n2 != iresponse->scriptVars.n2) {
					continue;
				}
			}

			if (iresponse->scriptVars.n3 != -1) {
				if (npcState->scriptVars.n3 != iresponse->scriptVars.n3) {
					continue;
				}
			}

			bool storageMatch = false;

			for (StorageConditions::const_iterator iter = iresponse->prop.storageConditions.begin(); iter != iresponse->prop.storageConditions.end(); ++iter) {
				const StorageCondition& cs = *iter;

				int32_t playerStorageValue = -1;

				if (!player->getStorageValue(cs.id, playerStorageValue)) {
					playerStorageValue = -1;
				}

				switch (cs.op) {
					case STORAGE_LESS: {
						if (playerStorageValue >= cs.value) {
							storageMatch = true;
						}

						break;
					}

					case STORAGE_LESSOREQUAL: {
						if (playerStorageValue > cs.value) {
							storageMatch = true;
						}

						break;
					}

					case STORAGE_EQUAL: {
						if (playerStorageValue != cs.value) {
							storageMatch = true;
						}

						break;
					}

					case STORAGE_NOTEQUAL: {
						if (playerStorageValue == cs.value) {
							storageMatch = true;
						}

						break;
					}

					case STORAGE_GREATEROREQUAL: {
						if (playerStorageValue < cs.value) {
							storageMatch = true;
						}

						break;
					}

					case STORAGE_GREATER: {
						if (playerStorageValue <= cs.value) {
							storageMatch = true;
						}

						break;
					}

					default:
						break;
				}

				if (storageMatch) {
					break;
				}
			}

			if (storageMatch) {
				continue;
			}

			if (hasBitSet(RESPOND_LOWMONEY, params)) {
				if (money == -1) {
					money = g_game.getMoney(player);
				}

				if (money >= (npcState->price * npcState->amount)) {
					continue;
				}
			}

			if (hasBitSet(RESPOND_ENOUGHMONEY, params)) {
				if (money == -1) {
					money = g_game.getMoney(player);
				}

				if (money < (npcState->price * npcState->amount)) {
					continue;
				}
			}

			if (hasBitSet(RESPOND_LOWAMOUNT, params) || hasBitSet(RESPOND_NOAMOUNT, params) || hasBitSet(RESPOND_ENOUGHAMOUNT, params)) {
				int32_t itemCount = player->__getItemTypeCount(npcState->itemId, npcState->subType);

				if (hasBitSet(RESPOND_ENOUGHAMOUNT, params)) {
					if (itemCount < npcState->amount) {
						continue;
					}
				} else if (itemCount >= npcState->amount) {
					continue;
					/*
					if(hasBitSet(RESPOND_LOWAMOUNT, params))
					{
						if(npcState->amount == 1)
							continue;
					}

					if(hasBitSet(RESPOND_NOAMOUNT, params))
					{
						if(npcState->amount > 1)
							continue;
					}
					*/
				}
			}

			if (iresponse->getHaveItemID() != 0) {
				int32_t itemCount = player->__getItemTypeCount(iresponse->getHaveItemID());

				if (itemCount == 0) {
					continue;
				}
			}

			if (iresponse->getDontHaveItemID() != 0) {
				int32_t itemCount = player->__getItemTypeCount(iresponse->getDontHaveItemID());

				if (itemCount > 0) {
					continue;
				}
			}

			if (iresponse->getEventType() != EVENT_NONE) {
				switch (iresponse->getEventType()) {
					case EVENT_IDLE: {
						if ((*it)->isSingleEvent() && hasUsedIdleReply) {
							continue;
						}

						if ((*it)->getTime() != 0) {
							//state idle (each state has its own idle)
							uint32_t time = (*it)->getTime() * 1000;

							if ((OTSYS_TIME() - npcState->lastResponseTime) < time) {
								//not enough time elapsed
								continue;
							}
						} else {
							continue;
						}

						break;
					}

					default:
						break;
				}
			}

			if (iresponse->getEventType() == EVENT_NONE || iresponse->getEventType() == EVENT_BUSY) {
				// Check keywords
				if (!text.empty() && !iresponse->getInputList() .empty()) {
					int32_t matches = matchKeywords(*it, wordList, exactMatch);

					if (matches > bestKeywordCount) {
						bestKeywordCount = matches;
						bestMatch = iresponse;
					}
				} else if (bestKeywordCount == 0) {
					bestMatch = iresponse;
				}
			} else {
				// First match is always the best
				return iresponse;
			}
		}

		if (bestMatch) {
			return bestMatch;
		}

		// If we got no match yet, try again with other topic
	}

	return NULL;
}

int32_t Npc::matchKeywords(NpcResponse* response, std::vector<std::string> wordList, bool exactMatch)
{
	int32_t bestMatchCount = 0;

	if (wordList.empty()) {
		return 0;
	}

	const std::list<std::string>& inputList = response->getInputList();

	for (std::list<std::string>::const_iterator it = inputList.begin(); it != inputList.end(); ++it) {
		int32_t matchCount = 0;
		std::vector<std::string>::iterator lastWordMatchIter = wordList.begin();
		std::string keywords = (*it);
		std::vector<std::string> keywordList = explodeString(keywords, ";");

		for (std::vector<std::string>::iterator keyIter = keywordList.begin(); keyIter != keywordList.end(); ++keyIter) {
			if (!exactMatch && (*keyIter) == "|*|") {
				//Match anything.
			} else if ((*keyIter) == "|amount|") {
				if (lastWordMatchIter == wordList.end()) {
					continue;
				}

				//TODO: Should iterate through each word until a number or a new keyword is found.
				int32_t amount = atoi(lastWordMatchIter->c_str());

				if (amount > 0) {
					if (amount <= 500) {
						response->setAmount(amount);
					} else {
						response->setAmount(500);
					}
				} else {
					response->setAmount(1);
					continue;
				}
			} else {
				bool fullMatch = false;

				if (!keyIter->empty()) {
					if ((*keyIter)[keyIter->size() - 1] == '$') {
						fullMatch = true;
					}
				}

				std::vector<std::string>::iterator wordIter = wordList.end();

				for (wordIter = lastWordMatchIter; wordIter != wordList.end(); ++wordIter) {
					if (fullMatch) {
						if (*wordIter == keyIter->substr(0, keyIter->size() - 1)) {
							break;
						}
					} else if (wordIter->find(*keyIter, 0) == 0) {
						break;
					}
				}

				if (wordIter == wordList.end()) {
					continue;
				}

				lastWordMatchIter = wordIter + 1;
			}

			++matchCount;
		}

		if ((size_t)matchCount == keywordList.size() && matchCount > bestMatchCount) {
			bestMatchCount = matchCount;
		}
	}

	return bestMatchCount;
}

const NpcResponse* Npc::getResponse(const Player* player, NpcState* npcState,
                                    const std::string& text, bool checkSubResponse)
{
	if (checkSubResponse && npcState->subResponse) {
		//Check previous response chain first
		const ResponseList& list = npcState->subResponse->getResponseList();
		const NpcResponse* response = getResponse(list, player, npcState, text);

		if (response) {
			return response;
		}
	}

	return getResponse(responseList, player, npcState, text);
}

const NpcResponse* Npc::getResponse(const Player* player, NpcEvent_t eventType)
{
	std::vector<NpcResponse*> result;

	for (ResponseList::const_iterator it = responseList.begin(); it != responseList.end(); ++it) {
		if ((*it)->getEventType() == eventType) {
			result.push_back(*it);
		}
	}

	if (result.empty()) {
		return NULL;
	}

	return result[random_range(0, result.size() - 1)];
}

const NpcResponse* Npc::getResponse(const Player* player, NpcState* npcState,
                                    NpcEvent_t eventType, const std::string& text, bool checkSubResponse)
{
	if (eventType == EVENT_NONE) {
		return NULL;
	}

	if (checkSubResponse && npcState->subResponse) {
		//Check previous response chain first
		const ResponseList& list = npcState->subResponse->getResponseList();
		const NpcResponse* response = getResponse(list, player, npcState, text, true, eventType);

		if (response) {
			return response;
		}
	}

	return getResponse(responseList, player, npcState, text, true, eventType);
}

const NpcResponse* Npc::getResponse(const Player* player, NpcState* npcState,
                                    NpcEvent_t eventType, bool checkSubResponse)
{
	if (eventType == EVENT_NONE) {
		return NULL;
	}

	if (checkSubResponse && npcState->subResponse) {
		//Check previous response chain first
		const ResponseList& list = npcState->subResponse->getResponseList();
		const NpcResponse* response = getResponse(list, player, npcState, "", true, eventType);

		if (response) {
			return response;
		}
	}

	return getResponse(responseList, player, npcState, "", true, eventType);
}

std::string Npc::formatResponse(Creature* creature, const NpcState* npcState, const NpcResponse* response) const
{
	std::string responseString = response->getText();

	replaceString(responseString, "|PRICE|", std::to_string(npcState->price * npcState->amount));
	replaceString(responseString, "|AMOUNT|", std::to_string(npcState->amount));
	replaceString(responseString, "|LEVEL|", std::to_string(npcState->level));

	replaceString(responseString, "|N1|", std::to_string(npcState->scriptVars.n1));
	replaceString(responseString, "|N2|", std::to_string(npcState->scriptVars.n2));
	replaceString(responseString, "|N3|", std::to_string(npcState->scriptVars.n3));

	replaceString(responseString, "|S1|", npcState->scriptVars.s1);
	replaceString(responseString, "|S2|", npcState->scriptVars.s2);
	replaceString(responseString, "|S3|", npcState->scriptVars.s3);

	replaceString(responseString, "|SPELLNAME|", npcState->spellName);

	if (npcState->spellName != "") {
		Spell* spell = g_spells->getInstantSpellByName(npcState->spellName);

		if (spell) {
			replaceString(responseString, "|SPELLMAGLEVEL|", std::to_string(spell->getMagicLevel()));
			replaceString(responseString, "|SPELLLEVEL|", std::to_string(spell->getLevel()));
		}
	}

	if (npcState->itemId != -1) {
		const ItemType& it = Item::items[npcState->itemId];

		if (npcState->amount <= 1) {
			std::ostringstream ss;
			ss << it.article + " " + it.name;
			replaceString(responseString, "|ITEMNAME|", ss.str());
		} else {
			replaceString(responseString, "|ITEMNAME|", it.getPluralName());
		}
	}

	replaceString(responseString, "|NAME|", creature->getName());
	replaceString(responseString, "|NPCNAME|", getName());
	return responseString;
}

void Npc::addShopPlayer(Player* player)
{
	ShopPlayerList::iterator it = std::find(shopPlayerList.begin(), shopPlayerList.end(), player);

	if (it == shopPlayerList.end()) {
		shopPlayerList.push_back(player);
	}
}

void Npc::removeShopPlayer(const Player* player)
{
	if (shopPlayerList.empty()) {
		return;
	}

	ShopPlayerList::iterator it = std::find(shopPlayerList.begin(), shopPlayerList.end(), player);

	if (it != shopPlayerList.end()) {
		shopPlayerList.erase(it);
	}
}

void Npc::closeAllShopWindows()
{
	for (ShopPlayerList::iterator it = shopPlayerList.begin(); it != shopPlayerList.end();) {
		Player* player = *(it);
		it = shopPlayerList.erase(it);
		player->closeShopWindow();
	}

	shopPlayerList.clear();
}

NpcScriptInterface* Npc::getScriptInterface()
{
	return m_scriptInterface;
}

NpcScriptInterface::NpcScriptInterface() :
	LuaScriptInterface("Npc interface")
{
	m_libLoaded = false;
	initState();
}

NpcScriptInterface::~NpcScriptInterface()
{
	//
}

bool NpcScriptInterface::initState()
{
	return LuaScriptInterface::initState();
}

bool NpcScriptInterface::closeState()
{
	m_libLoaded = false;
	return LuaScriptInterface::closeState();
}

bool NpcScriptInterface::loadNpcLib(const std::string& file)
{
	if (m_libLoaded) {
		return true;
	}

	if (loadFile(file) == -1) {
		std::cout << "Warning: [NpcScriptInterface::loadNpcLib] Can not load " << file << std::endl;
		return false;
	}

	m_libLoaded = true;
	return true;
}

void NpcScriptInterface::registerFunctions()
{
	LuaScriptInterface::registerFunctions();

	//npc exclusive functions
	lua_register(m_luaState, "selfSay", NpcScriptInterface::luaActionSay);
	lua_register(m_luaState, "selfMove", NpcScriptInterface::luaActionMove);
	lua_register(m_luaState, "selfMoveTo", NpcScriptInterface::luaActionMoveTo);
	lua_register(m_luaState, "selfTurn", NpcScriptInterface::luaActionTurn);
	lua_register(m_luaState, "selfFollow", NpcScriptInterface::luaActionFollow);
	lua_register(m_luaState, "selfGetPosition", NpcScriptInterface::luaSelfGetPos);
	lua_register(m_luaState, "getDistanceTo", NpcScriptInterface::luagetDistanceTo);
	lua_register(m_luaState, "doNpcSetCreatureFocus", NpcScriptInterface::luaSetNpcFocus);
	lua_register(m_luaState, "getNpcCid", NpcScriptInterface::luaGetNpcCid);
	lua_register(m_luaState, "getNpcPos", NpcScriptInterface::luaGetNpcPos);
	lua_register(m_luaState, "getNpcState", NpcScriptInterface::luaGetNpcState);
	lua_register(m_luaState, "setNpcState", NpcScriptInterface::luaSetNpcState);
	lua_register(m_luaState, "getNpcName", NpcScriptInterface::luaGetNpcName);
	lua_register(m_luaState, "getNpcParameter", NpcScriptInterface::luaGetNpcParameter);
	lua_register(m_luaState, "openShopWindow", NpcScriptInterface::luaOpenShopWindow);
	lua_register(m_luaState, "closeShopWindow", NpcScriptInterface::luaCloseShopWindow);
	lua_register(m_luaState, "doSellItem", NpcScriptInterface::luaDoSellItem);
}

int32_t NpcScriptInterface::luaSelfGetPos(lua_State* L)
{
	//selfGetPosition()
	ScriptEnvironment* env = getScriptEnv();
	Npc* npc = env->getNpc();

	if (npc) {
		Position pos = npc->getPosition();
		pushPosition(L, pos);
	} else {
		lua_pushnil(L);
	}

	return 1;
}

int32_t NpcScriptInterface::luaActionSay(lua_State* L)
{
	//selfSay(words [[, target], publicize])
	// publicize defaults to true if there is no target, false otherwise
	uint32_t parameters = lua_gettop(L);
	uint32_t target = 0;
	bool publicize = true;

	if (parameters >= 3) {
		publicize = (popNumber(L) == 1);
	}

	if (parameters >= 2) {
		target = popNumber(L);

		if (target != 0) {
			publicize = false;
		}
	}

	std::string text = popString(L);

	ScriptEnvironment* env = getScriptEnv();

	Npc* npc = env->getNpc();
	Player* player = g_game.getPlayerByID(target);

	if (npc) {
		if (publicize) {
			npc->doSay(text);
		} else {
			npc->doSayToPlayer(player, text);
		}
	}

	return 0;
}

int32_t NpcScriptInterface::luaActionMove(lua_State* L)
{
	//selfMove(direction)
	Direction dir = (Direction)popNumber(L);
	ScriptEnvironment* env = getScriptEnv();

	Npc* npc = env->getNpc();

	if (npc) {
		npc->doMove(dir);
	}

	return 0;
}

int32_t NpcScriptInterface::luaActionMoveTo(lua_State* L)
{
	//selfMoveTo(x,y,z)
	Position target;
	target.z = (int)popNumber(L);
	target.y = (int)popNumber(L);
	target.x = (int)popNumber(L);

	ScriptEnvironment* env = getScriptEnv();
	Npc* npc = env->getNpc();

	if (npc) {
		npc->doMoveTo(target);
	}

	return 0;
}

int32_t NpcScriptInterface::luaActionTurn(lua_State* L)
{
	//selfTurn(direction)
	Direction dir = (Direction)popNumber(L);

	ScriptEnvironment* env = getScriptEnv();

	Npc* npc = env->getNpc();

	if (npc) {
		npc->doTurn(dir);
	}

	return 0;
}

int32_t NpcScriptInterface::luaActionFollow(lua_State* L)
{
	//selfFollow(cid)
	uint32_t cid = popNumber(L);

	ScriptEnvironment* env = getScriptEnv();

	Player* player = g_game.getPlayerByID(cid);

	if (cid != 0 && !player) {
		lua_pushboolean(L, false);
		return 1;
	}

	Npc* npc = env->getNpc();

	if (!npc) {
		lua_pushboolean(L, false);
		return 1;
	}

	bool result = npc->setFollowCreature(player, true);
	lua_pushboolean(L, result);
	return 1;
}

int32_t NpcScriptInterface::luagetDistanceTo(lua_State* L)
{
	//getDistanceTo(uid)
	uint32_t uid = popNumber(L);

	ScriptEnvironment* env = getScriptEnv();

	Npc* npc = env->getNpc();
	Thing* thing = env->getThingByUID(uid);

	if (thing && npc) {
		Position thing_pos = thing->getPosition();
		Position npc_pos = npc->getPosition();

		if (npc_pos.z != thing_pos.z) {
			lua_pushnumber(L, -1);
		} else {
			int32_t dist = std::max<int32_t>(std::abs(npc_pos.x - thing_pos.x), std::abs(npc_pos.y - thing_pos.y));
			lua_pushnumber(L, dist);
		}
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_THING_NOT_FOUND));
		lua_pushnil(L);
	}

	return 1;
}

int32_t NpcScriptInterface::luaSetNpcFocus(lua_State* L)
{
	//doNpcSetCreatureFocus(cid)
	uint32_t cid = popNumber(L);

	ScriptEnvironment* env = getScriptEnv();

	Npc* npc = env->getNpc();

	if (npc) {
		Creature* creature = g_game.getCreatureByID(cid);

		if (creature) {
			npc->hasScriptedFocus = true;
		} else {
			npc->hasScriptedFocus = false;
		}

		npc->setCreatureFocus(creature);
	}

	return 0;
}

int32_t NpcScriptInterface::luaGetNpcPos(lua_State* L)
{
	//getNpcPos()
	ScriptEnvironment* env = getScriptEnv();

	Position pos(0, 0, 0);
	uint32_t stackpos = 0;

	Npc* npc = env->getNpc();

	if (npc) {
		pos = npc->getPosition();
		stackpos = npc->getParent()->__getIndexOfThing(npc);
	}

	pushPosition(L, pos, stackpos);
	return 1;
}

int32_t NpcScriptInterface::luaGetNpcState(lua_State* L)
{
	//getNpcState(cid)
	uint32_t cid = popNumber(L);

	ScriptEnvironment* env = getScriptEnv();
	const Player* player = g_game.getPlayerByID(cid);

	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	Npc* npc = env->getNpc();

	if (!npc) {
		lua_pushnil(L);
		return 1;
	}

	NpcState* state = npc->getState(player);
	NpcScriptInterface::pushState(L, state);
	return 1;
}

int32_t NpcScriptInterface::luaSetNpcState(lua_State* L)
{
	//setNpcState(state, cid)
	uint32_t cid = popNumber(L);

	ScriptEnvironment* env = getScriptEnv();
	const Player* player = g_game.getPlayerByID(cid);

	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	Npc* npc = env->getNpc();

	if (!npc) {
		lua_pushboolean(L, false);
		return 1;
	}

	NpcState* state = npc->getState(player);
	NpcScriptInterface::popState(L, state);
	lua_pushboolean(L, true);
	return 1;
}

int32_t NpcScriptInterface::luaGetNpcCid(lua_State* L)
{
	//getNpcCid()
	Npc* npc = getScriptEnv()->getNpc();
	if (npc) {
		lua_pushnumber(L, npc->getID());
	} else {
		lua_pushnil(L);
	}

	return 1;
}

int32_t NpcScriptInterface::luaGetNpcName(lua_State* L)
{
	//getNpcName()
	ScriptEnvironment* env = getScriptEnv();

	Npc* npc = env->getNpc();

	if (npc) {
		lua_pushstring(L, npc->getName().c_str());
	} else {
		lua_pushstring(L, "");
	}

	return 1;
}

int32_t NpcScriptInterface::luaGetNpcParameter(lua_State* L)
{
	//getNpcParameter(paramKey)
	std::string paramKey = popString(L);

	ScriptEnvironment* env = getScriptEnv();

	Npc* npc = env->getNpc();

	if (npc) {
		Npc::ParametersMap::iterator it = npc->m_parameters.find(paramKey);

		if (it != npc->m_parameters.end()) {
			lua_pushstring(L, it->second.c_str());
		} else {
			lua_pushnil(L);
		}
	} else {
		lua_pushnil(L);
	}

	return 1;
}

void NpcScriptInterface::pushState(lua_State* L, NpcState* state)
{
	lua_newtable(L);
	setField(L, "price", state->price);
	setField(L, "amount", state->amount);
	setField(L, "itemid", state->itemId);
	setField(L, "subtype", state->subType);
	setField(L, "subType", state->subType);
	setFieldBool(L, "ignore", state->ignore);
	setFieldBool(L, "ignorecapacity", state->ignore);
	setFieldBool(L, "ignoreequipped", state->ignore);
	setFieldBool(L, "inbackpacks", state->inBackpacks);
	setField(L, "topic", state->topic);
	setField(L, "level", state->level);
	setField(L, "spellname", state->spellName);
	setField(L, "listname", state->listName);
	setField(L, "listpname", state->listPluralName);
	setFieldBool(L, "isidle", state->focusState != 1);

	setField(L, "n1", state->scriptVars.n1);
	setField(L, "n2", state->scriptVars.n2);
	setField(L, "n3", state->scriptVars.n3);

	setFieldBool(L, "b1", state->scriptVars.b1);
	setFieldBool(L, "b2", state->scriptVars.b2);
	setFieldBool(L, "b3", state->scriptVars.b3);

	setField(L, "s1", state->scriptVars.s1);
	setField(L, "s2", state->scriptVars.s2);
	setField(L, "s3", state->scriptVars.s3);
}

void NpcScriptInterface::popState(lua_State* L, NpcState* &state)
{
	state->price = getField(L, "price");
	state->amount = getField(L, "amount");
	state->itemId = getField(L, "itemid");
	state->subType = getField(L, "subtype");

	if (state->subType == 0) {
		state->subType = getField(L, "subType");
	}

	state->ignore = getFieldBool(L, "ignore") || getFieldBool(L, "ignorecapacity") || getFieldBool(L, "ignoreequipped");
	state->inBackpacks = getFieldBool(L, "inbackpacks");

	state->topic = std::max<int32_t>(getField(L, "topic"), 0);
	state->level = getField(L, "level");
	state->spellName = getFieldString(L, "spellname");
	state->listName = getFieldString(L, "listname");
	state->listPluralName = getFieldString(L, "listpname");
	bool isIdle = getFieldBool(L, "isidle") || getFieldBool(L, "isIdle");

	if (isIdle) {
		state->focusState = 0;
	}

	state->scriptVars.n1 = getField(L, "n1");
	state->scriptVars.n2 = getField(L, "n2");
	state->scriptVars.n3 = getField(L, "n3");

	state->scriptVars.b1 = getFieldBool(L, "b1");
	state->scriptVars.b2 = getFieldBool(L, "b2");
	state->scriptVars.n3 = getFieldBool(L, "b3");

	state->scriptVars.s1 = getFieldString(L, "s1");
	state->scriptVars.s2 = getFieldString(L, "s2");
	state->scriptVars.s3 = getFieldString(L, "s3");
}

int32_t NpcScriptInterface::luaOpenShopWindow(lua_State* L)
{
	//openShopWindow(cid, items, onBuy callback, onSell callback)
	int32_t buyCallback = -1;
	int32_t sellCallback = -1;
	std::list<ShopInfo> items;
	Player* player = NULL;

	ScriptEnvironment* env = getScriptEnv();
	Npc* npc = env->getNpc();

	if (lua_isfunction(L, -1) == 0) {
		lua_pop(L, 1);    // skip it - use default value
	} else {
		sellCallback = popCallback(L);
	}

	if (lua_isfunction(L, -1) == 0) {
		lua_pop(L, 1);    // skip it - use default value
	} else {
		buyCallback = popCallback(L);
	}

	if (lua_istable(L, -1) == 0) {
		reportError(__FUNCTION__, "item list is not a table.");
		lua_pushboolean(L, false);
		return 1;
	}

	// first key
	lua_pushnil(L);

	while (lua_next(L, -2) != 0) {
		ShopInfo item;
		item.itemId = getField(L, "id");
		item.subType = getField(L, "subType");

		if (item.subType == 0) {
			item.subType = getField(L, "subtype");
		}

		item.buyPrice = getField(L, "buy");
		item.sellPrice = getField(L, "sell");
		item.realName = getFieldString(L, "name");
		items.push_back(item);

		lua_pop(L, 1);
	}

	lua_pop(L, 1);

	player = g_game.getPlayerByID(popNumber(L));

	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		lua_pushboolean(L, false);
		return 1;
	}

	//Close any eventual other shop window currently open.
	player->closeShopWindow(false);

	if (!npc) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		lua_pushboolean(L, false);
		return 1;
	}

	npc->addShopPlayer(player);
	player->setShopOwner(npc, buyCallback, sellCallback);
	player->openShopWindow(npc, items);

	lua_pushboolean(L, true);
	return 1;
}

int32_t NpcScriptInterface::luaCloseShopWindow(lua_State* L)
{
	//closeShopWindow(cid)
	ScriptEnvironment* env = getScriptEnv();

	Player* player = g_game.getPlayerByID(popNumber(L));

	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		lua_pushboolean(L, false);
		return 1;
	}

	Npc* npc = env->getNpc();

	if (!npc) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		lua_pushboolean(L, false);
		return 1;
	}

	int32_t buyCallback;
	int32_t sellCallback;

	Npc* merchant = player->getShopOwner(buyCallback, sellCallback);

	//Check if we actually have a shop window with this player.
	if (merchant == npc) {
		player->sendCloseShop();

		if (buyCallback != -1) {
			luaL_unref(L, LUA_REGISTRYINDEX, buyCallback);
		}

		if (sellCallback != -1) {
			luaL_unref(L, LUA_REGISTRYINDEX, sellCallback);
		}

		player->setShopOwner(NULL, -1, -1);
		npc->removeShopPlayer(player);
	}

	lua_pushboolean(L, true);
	return 1;
}

int32_t NpcScriptInterface::luaDoSellItem(lua_State* L)
{
	//doSellItem(cid, itemid, amount, <optional> subtype, <optional> actionid, <optional: default: 1> canDropOnMap)
	int32_t parameters = lua_gettop(L);

	bool canDropOnMap = true;

	if (parameters > 5) {
		canDropOnMap = (popNumber(L) == 1);
	}

	uint32_t actionId = 0;

	if (parameters > 4) {
		actionId = popNumber(L);
	}

	uint32_t subType = 1;

	if (parameters > 3) {
		int32_t n = popNumber(L);

		if (n != -1) {
			subType = n;
		}
	}

	uint32_t amount = (uint32_t)popNumber(L);
	uint32_t itemId = (uint32_t)popNumber(L);

	Player* player = g_game.getPlayerByID(popNumber(L));
	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		lua_pushboolean(L, false);
		return 1;
	}

	uint32_t sellCount = 0;

	const ItemType& it = Item::items[itemId];
	if (it.stackable) {
		while (amount > 0) {
			int32_t stackCount = std::min<int32_t>(100, amount);
			Item* item = Item::CreateItem(it.id, stackCount);

			if (item && actionId != 0) {
				item->setActionId(actionId);
			}

			if (g_game.internalPlayerAddItem(player, item, canDropOnMap) != RET_NOERROR) {
				delete item;
				lua_pushnumber(L, sellCount);
				return 1;
			}

			amount = amount - stackCount;
			sellCount += stackCount;
		}
	} else {
		for (uint32_t i = 0; i < amount; ++i) {
			Item* item = Item::CreateItem(it.id, subType);

			if (item && actionId != 0) {
				item->setActionId(actionId);
			}

			if (g_game.internalPlayerAddItem(player, item, canDropOnMap) != RET_NOERROR) {
				delete item;
				lua_pushnumber(L, sellCount);
				return 1;
			}

			++sellCount;
		}
	}

	lua_pushnumber(L, sellCount);
	return 1;
}

NpcEventsHandler::NpcEventsHandler(Npc* npc)
{
	m_npc = npc;
	m_loaded = false;
}

NpcEventsHandler::~NpcEventsHandler()
{
	//
}

bool NpcEventsHandler::isLoaded() const
{
	return m_loaded;
}

NpcScript::NpcScript(const std::string& file, Npc* npc) :
	NpcEventsHandler(npc)
{
	m_scriptInterface = npc->getScriptInterface();

	if (m_scriptInterface->reserveScriptEnv()) {
		m_scriptInterface->getScriptEnv()->setNpc(npc);

		if (m_scriptInterface->loadFile(file, npc) == -1) {
			std::cout << "[Warning - NpcScript::NpcScript] Can not load script: " << file << std::endl;
			std::cout << m_scriptInterface->getLastLuaError() << std::endl;
			m_loaded = false;
			m_scriptInterface->releaseScriptEnv();
			return;
		}

		m_scriptInterface->releaseScriptEnv();
	}

	m_onCreatureSay = m_scriptInterface->getEvent("onCreatureSay");
	m_onCreatureDisappear = m_scriptInterface->getEvent("onCreatureDisappear");
	m_onCreatureAppear = m_scriptInterface->getEvent("onCreatureAppear");
	m_onCreatureMove = m_scriptInterface->getEvent("onCreatureMove");
	m_onPlayerCloseChannel = m_scriptInterface->getEvent("onPlayerCloseChannel");
	m_onPlayerEndTrade = m_scriptInterface->getEvent("onPlayerEndTrade");
	m_onThink = m_scriptInterface->getEvent("onThink");
	m_loaded = true;
}

NpcScript::~NpcScript()
{
	//
}

void NpcScript::onCreatureAppear(const Creature* creature)
{
	if (m_onCreatureAppear == -1) {
		return;
	}

	//onCreatureAppear(creature)
	if (m_scriptInterface->reserveScriptEnv()) {
		ScriptEnvironment* env = m_scriptInterface->getScriptEnv();

		lua_State* L = m_scriptInterface->getLuaState();

		env->setScriptId(m_onCreatureAppear, m_scriptInterface);
		env->setRealPos(m_npc->getPosition());
		env->setNpc(m_npc);

		m_scriptInterface->pushFunction(m_onCreatureAppear);
		lua_pushnumber(L, creature->getID());
		m_scriptInterface->callFunction(1);
		m_scriptInterface->releaseScriptEnv();
	} else {
		std::cout << "[Error - NpcScript::onCreatureAppear] NPC Name: " << m_npc->getName() << " - Call stack overflow" << std::endl;
	}
}

void NpcScript::onCreatureDisappear(const Creature* creature)
{
	if (m_onCreatureDisappear == -1) {
		return;
	}

	//onCreatureDisappear(id)
	if (m_scriptInterface->reserveScriptEnv()) {
		ScriptEnvironment* env = m_scriptInterface->getScriptEnv();

		lua_State* L = m_scriptInterface->getLuaState();

		env->setScriptId(m_onCreatureDisappear, m_scriptInterface);
		env->setRealPos(m_npc->getPosition());
		env->setNpc(m_npc);

		m_scriptInterface->pushFunction(m_onCreatureDisappear);
		lua_pushnumber(L, creature->getID());
		m_scriptInterface->callFunction(1);
		m_scriptInterface->releaseScriptEnv();
	} else {
		std::cout << "[Error - NpcScript::onCreatureDisappear] NPC Name: " << m_npc->getName() << " - Call stack overflow" << std::endl;
	}
}

void NpcScript::onCreatureMove(const Creature* creature, const Position& oldPos, const Position& newPos)
{
	if (m_onCreatureMove == -1) {
		return;
	}

	//onCreatureMove(creature, oldPos, newPos)
	if (m_scriptInterface->reserveScriptEnv()) {
		ScriptEnvironment* env = m_scriptInterface->getScriptEnv();

		lua_State* L = m_scriptInterface->getLuaState();

		env->setScriptId(m_onCreatureMove, m_scriptInterface);
		env->setRealPos(m_npc->getPosition());
		env->setNpc(m_npc);

		m_scriptInterface->pushFunction(m_onCreatureMove);
		lua_pushnumber(L, creature->getID());
		LuaScriptInterface::pushPosition(L, oldPos, 0);
		LuaScriptInterface::pushPosition(L, newPos, 0);
		m_scriptInterface->callFunction(3);
		m_scriptInterface->releaseScriptEnv();
	} else {
		std::cout << "[Error - NpcScript::onCreatureMove] NPC Name: " << m_npc->getName() << " - Call stack overflow" << std::endl;
	}
}

void NpcScript::onCreatureSay(const Creature* creature, SpeakClasses type, const std::string& text, Position* pos/* = NULL*/)
{
	if (m_onCreatureSay == -1) {
		return;
	}

	//onCreatureSay(cid, type, msg)
	if (m_scriptInterface->reserveScriptEnv()) {
		ScriptEnvironment* env = m_scriptInterface->getScriptEnv();

		env->setScriptId(m_onCreatureSay, m_scriptInterface);
		env->setRealPos(m_npc->getPosition());
		env->setNpc(m_npc);

		lua_State* L = m_scriptInterface->getLuaState();
		m_scriptInterface->pushFunction(m_onCreatureSay);
		lua_pushnumber(L, creature->getID());
		lua_pushnumber(L, type);
		lua_pushstring(L, text.c_str());
		m_scriptInterface->callFunction(3);
		m_scriptInterface->releaseScriptEnv();
	} else {
		std::cout << "[Error - NpcScript::onCreatureSay] NPC Name: " << m_npc->getName() << " - Call stack overflow" << std::endl;
	}
}

void NpcScript::onPlayerTrade(const Player* player, int32_t callback, uint16_t itemid,
                              uint8_t count, uint8_t amount, bool ignore, bool inBackpacks)
{
	if (callback == -1) {
		return;
	}

	//"onBuy"(cid, itemid, count, amount, ignore, inbackpacks)
	if (m_scriptInterface->reserveScriptEnv()) {
		ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
		env->setScriptId(-1, m_scriptInterface);
		env->setRealPos(m_npc->getPosition());
		env->setNpc(m_npc);

		lua_State* L = m_scriptInterface->getLuaState();
		LuaScriptInterface::pushCallback(L, callback);
		lua_pushnumber(L, player->getID());
		lua_pushnumber(L, itemid);
		lua_pushnumber(L, count);
		lua_pushnumber(L, amount);
		lua_pushboolean(L, ignore);
		lua_pushboolean(L, inBackpacks);
		m_scriptInterface->callFunction(6);
		m_scriptInterface->releaseScriptEnv();
	} else {
		std::cout << "[Error - NpcScript::onPlayerTrade] NPC Name: " << m_npc->getName() << " - Call stack overflow" << std::endl;
	}
}

void NpcScript::onPlayerCloseChannel(const Player* player)
{
	if (m_onPlayerCloseChannel == -1) {
		return;
	}

	//onPlayerCloseChannel(cid)
	if (m_scriptInterface->reserveScriptEnv()) {
		ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
		env->setScriptId(m_onPlayerCloseChannel, m_scriptInterface);
		env->setRealPos(m_npc->getPosition());
		env->setNpc(m_npc);

		lua_State* L = m_scriptInterface->getLuaState();
		m_scriptInterface->pushFunction(m_onPlayerCloseChannel);
		lua_pushnumber(L, player->getID());
		m_scriptInterface->callFunction(1);
		m_scriptInterface->releaseScriptEnv();
	} else {
		std::cout << "[Error - NpcScript::onPlayerCloseChannel] NPC Name: " << m_npc->getName() << " - Call stack overflow" << std::endl;
	}
}

void NpcScript::onPlayerEndTrade(const Player* player)
{
	if (m_onPlayerCloseChannel == -1) {
		return;
	}

	//onPlayerEndTrade(cid)
	if (m_scriptInterface->reserveScriptEnv()) {
		ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
		env->setScriptId(m_onPlayerCloseChannel, m_scriptInterface);
		env->setRealPos(m_npc->getPosition());
		env->setNpc(m_npc);

		lua_State* L = m_scriptInterface->getLuaState();
		m_scriptInterface->pushFunction(m_onPlayerEndTrade);
		lua_pushnumber(L, player->getID());
		m_scriptInterface->callFunction(1);
		m_scriptInterface->releaseScriptEnv();
	} else {
		std::cout << "[Error - NpcScript::onPlayerEndTrade] NPC Name: " << m_npc->getName() << " - Call stack overflow" << std::endl;
	}
}

void NpcScript::onThink()
{
	if (m_onThink == -1) {
		return;
	}

	//onThink()
	if (m_scriptInterface->reserveScriptEnv()) {
		ScriptEnvironment* env = m_scriptInterface->getScriptEnv();

		env->setScriptId(m_onThink, m_scriptInterface);
		env->setRealPos(m_npc->getPosition());
		env->setNpc(m_npc);

		m_scriptInterface->pushFunction(m_onThink);
		m_scriptInterface->callFunction(0);
		m_scriptInterface->releaseScriptEnv();
	} else {
		std::cout << "[Error - NpcScript::onThink] NPC Name: " << m_npc->getName() << " - Call stack overflow" << std::endl;
	}
}
