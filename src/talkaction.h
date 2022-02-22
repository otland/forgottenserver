// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_TALKACTION_H_E6AABAC0F89843469526ADF310F3131C
#define FS_TALKACTION_H_E6AABAC0F89843469526ADF310F3131C

#include "luascript.h"
#include "baseevents.h"
#include "const.h"

class TalkAction;
using TalkAction_ptr = std::unique_ptr<TalkAction>;

enum TalkActionResult_t {
	TALKACTION_CONTINUE,
	TALKACTION_BREAK,
	TALKACTION_FAILED,
};

class TalkAction : public Event
{
	public:
		explicit TalkAction(LuaScriptInterface* interface) : Event(interface) {}

		bool configureEvent(const pugi::xml_node& node) override;

		const std::string& getWords() const {
			return words;
		}
		const std::vector<std::string>& getWordsMap() const {
			return wordsMap;
		}
		void setWords(std::string word) {
			words = word;
			wordsMap.push_back(word);
		}
		std::string getSeparator() const {
			return separator;
		}
		void setSeparator(std::string sep) {
			separator = sep;
		}

		//scripting
		bool executeSay(Player* player, const std::string& words, const std::string& param, SpeakClasses type) const;

		AccountType_t getRequiredAccountType() const {
			return requiredAccountType;
		}

		void setRequiredAccountType(AccountType_t reqAccType) {
			requiredAccountType = reqAccType;
		}

		bool getNeedAccess() const {
			return needAccess;
		}

		void setNeedAccess(bool b) {
			needAccess = b;
		}

	private:
		std::string getScriptEventName() const override;

		std::string words;
		std::vector<std::string> wordsMap;
		std::string separator = "\"";
		bool needAccess = false;
		AccountType_t requiredAccountType = ACCOUNT_TYPE_NORMAL;
};

class TalkActions final : public BaseEvents
{
	public:
		TalkActions();
		~TalkActions();

		// non-copyable
		TalkActions(const TalkActions&) = delete;
		TalkActions& operator=(const TalkActions&) = delete;

		TalkActionResult_t playerSaySpell(Player* player, SpeakClasses type, const std::string& words) const;

		bool registerLuaEvent(TalkAction* event);
		void clear(bool fromLua) override final;

	private:
		LuaScriptInterface& getScriptInterface() override;
		std::string getScriptBaseName() const override;
		Event_ptr getEvent(const std::string& nodeName) override;
		bool registerEvent(Event_ptr event, const pugi::xml_node& node) override;

		std::map<std::string, TalkAction> talkActions;

		LuaScriptInterface scriptInterface;
};

#endif
