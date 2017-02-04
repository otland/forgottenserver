/**
* The Forgotten Server - a free and open-source MMORPG server emulator
* Copyright (C) 2017  Mark Samman <mark.samman@gmail.com>
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

#include "store.h"

#include "pugicast.h"
#include "database.h"

StoreOffer& StoreCategory::createOffer(uint32_t offerId) {
	offers.emplace_back(offerId);
	return offers.back();
}

void Store::getTransactionHistory(uint32_t accountId, uint16_t page, uint32_t entriesPerPage, std::vector<StoreTransaction>& out) const {
	Database& db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT * FROM `store_history` WHERE `account_id` = " << accountId << " ORDER BY `timestamp` DESC LIMIT " << entriesPerPage << " OFFSET " << (page * entriesPerPage);

	DBResult_ptr result = db.storeQuery(query.str());
	if (result) {
		do {
			out.emplace_back(result->getNumber<uint32_t>("id"), result->getNumber<int32_t>("coins"), result->getString("description"), result->getNumber<time_t>("timestamp"));
		} while (result->next());
	}
}

void Store::onTransactionCompleted(uint32_t accountId, int32_t coins, const std::string& description) const {
	Database& db = Database::getInstance();

	std::ostringstream query;
	query << "INSERT INTO `store_history` (`account_id`, `coins`, `description`, `timestamp`) VALUES (" << accountId << "," << coins << "," << db.escapeString(description) << "," << time(nullptr) << ")";

	db.executeQuery(query.str());
}

bool Store::loadFromXml(bool /*reloading = false*/) {
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/store/store.xml");
	if (!result) {
		printXMLError("Error - Store::loadFromXml", "data/store/store.xml", result);
		return false;
	}

	loaded = true;

	std::string name;
	StoreOfferState_t state;
	std::string description;
	StringVector icons;

	for (auto categoryNode : doc.child("store").children()) {
		pugi::xml_attribute attr;

		name.clear();
		state = StoreOfferState_t::STORE_OFFERSTATE_NONE;
		description.clear();
		icons.clear();

		if ((attr = categoryNode.attribute("name"))) {
			name = attr.as_string();
		} else {
			std::cout << "[Error - Store::loadFromXml] Missing category name." << std::endl;
			continue;
		}

		if ((attr = categoryNode.attribute("description"))) {
			description = attr.as_string();
		}

		if ((attr = categoryNode.attribute("icons"))) {
			icons = explodeString(attr.as_string(), ";");
		}

		categories.emplace_back(name, state, description, icons);
		StoreCategory& category = categories.back();

		for (auto offerNode : categoryNode.children()) {
			StoreOffer& offer = category.createOffer(runningId++);

			if ((attr = offerNode.attribute("name"))) {
				offer.name = attr.as_string();
			} else {
				std::cout << "[Error - Store::loadFromXml] Missing offer name in category: " << category.name << std::endl;
				category.offers.pop_back();
				continue;
			}

			if ((attr = offerNode.attribute("state"))) {
				StoreOfferState_t offerState = static_cast<StoreOfferState_t>(pugi::cast<uint32_t>(attr.value()));
				if (offerState > category.state) {
					category.state = offerState;
				}
			}

			if ((attr = offerNode.attribute("description"))) {
				offer.description = attr.as_string();
			}

			if ((attr = offerNode.attribute("message"))) {
				offer.message = attr.as_string();
			}

			if ((attr = offerNode.attribute("icons"))) {
				offer.icons = explodeString(attr.as_string(), ";");
			}

			if ((attr = offerNode.attribute("price"))) {
				offer.price = attr.as_uint();
			}

			for (auto subOfferNode : offerNode.children()) {
				name.clear();
				state = StoreOfferState_t::STORE_OFFERSTATE_NONE;
				description.clear();
				icons.clear();

				if ((attr = subOfferNode.attribute("name"))) {
					name = attr.as_string();
				}

				if ((attr = subOfferNode.attribute("description"))) {
					description = attr.as_string();
				}

				if ((attr = subOfferNode.attribute("icons"))) {
					icons = explodeString(attr.as_string(), ";");
				}

				offer.subOffers.push_back(SubOffer(name, state, description, icons));
			}

			if ((attr = offerNode.attribute("script"))) {
				if (!scriptInterface) {
					scriptInterface.reset(new LuaScriptInterface("Store Interface"));
					scriptInterface->initState();
				}

				std::string script = attr.as_string();
				if (scriptInterface->loadFile("data/store/scripts/" + script) == 0) {
					offer.scriptInterface = scriptInterface.get();
					offer.renderEvent = scriptInterface->getEvent("onRender");
					offer.buyEvent = scriptInterface->getEvent("onBuy");
				} else {
					std::cout << "[Warning - Store::loadFromXml] Can not load script: " << script << std::endl;
					std::cout << scriptInterface->getLastLuaError() << std::endl;
					return false;
				}
			}
		}
	}

	return true;
}

bool Store::reload() {
	categories.clear();

	loaded = false;

	scriptInterface.reset();

	return loadFromXml(true);
}

boost::optional<StoreOffer&> Store::getOfferById(uint32_t id) {
	for (auto& category : getCategories()) {
		for (auto& offer : category.getOffers()) {
			if (offer.getId() == id) {
				return boost::optional<StoreOffer&>(offer);
			}
		}
	}

	return boost::optional<StoreOffer&>();
}

StoreResult Store::executeOnRender(Player* player, StoreOffer* offer) {
	StoreResult result;
	if (offer->renderEvent != -1) {
		// onRender(player, offer)
		LuaScriptInterface* scriptInterface = offer->scriptInterface;
		if (!scriptInterface->reserveScriptEnv()) {
			std::cout << "[Error - Store::executeOnRender] Call stack overflow" << std::endl;
			return result;
		}

		ScriptEnvironment* env = scriptInterface->getScriptEnv();
		env->setScriptId(offer->renderEvent, scriptInterface);

		lua_State* L = scriptInterface->getLuaState();
		scriptInterface->pushFunction(offer->renderEvent);

		LuaScriptInterface::pushUserdata<Player>(L, player);
		LuaScriptInterface::setMetatable(L, -1, "Player");

		LuaScriptInterface::pushUserdata<StoreOffer>(L, offer);
		LuaScriptInterface::setMetatable(L, -1, "StoreOffer");

		if (scriptInterface->protectedCall(L, 2, 2) != 0) {
			LuaScriptInterface::reportError(nullptr, LuaScriptInterface::popString(L));
		} else {
			result.enable = LuaScriptInterface::getBoolean(L, -2);
			result.reason = LuaScriptInterface::getString(L, -1);

			lua_pop(L, 2);
		}

		scriptInterface->resetScriptEnv();
	}

	return result;
}

bool Store::executeOnBuy(Player* player, StoreOffer* offer, const std::string& param) {
	if (offer->buyEvent != -1) {
		// onBuy(player, offer, param)
		LuaScriptInterface* scriptInterface = offer->scriptInterface;
		if (!scriptInterface->reserveScriptEnv()) {
			std::cout << "[Error - Store::executeOnBuy] Call stack overflow" << std::endl;
			return false;
		}

		ScriptEnvironment* env = scriptInterface->getScriptEnv();
		env->setScriptId(offer->buyEvent, scriptInterface);

		lua_State* L = scriptInterface->getLuaState();
		scriptInterface->pushFunction(offer->buyEvent);

		LuaScriptInterface::pushUserdata<Player>(L, player);
		LuaScriptInterface::setMetatable(L, -1, "Player");

		LuaScriptInterface::pushUserdata<StoreOffer>(L, offer);
		LuaScriptInterface::setMetatable(L, -1, "StoreOffer");

		if (param.size() == 0) {
			lua_pushnil(L);
		} else {
			LuaScriptInterface::pushString(L, param);
		}

		return scriptInterface->callFunction(3);
	}

	return false;
}
