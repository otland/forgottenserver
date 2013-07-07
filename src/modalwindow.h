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

#ifndef __MODALWINDOW_H__
#define __MODALWINDOW_H__

#include "definitions.h"

#include <list>
#include <string>

typedef std::pair<std::string, uint8_t> ModalWindowChoice;
typedef std::list<ModalWindowChoice> ModalWindowChoiceList;

class ModalWindow
{
	public:
		ModalWindow(uint32_t id, const std::string& title, const std::string& message);
		~ModalWindow() {}

		std::string getTitle() const;
		std::string getMessage() const;

		uint32_t getID() const;
		uint32_t getButtonCount() const;
		uint32_t getChoiceCount() const;
		void addChoice(uint8_t id, const std::string& text);
		void addButton(uint8_t id, const std::string& text);

		void setDefaultEnterButton(uint8_t enterButtonId);
		void setDefaultEscapeButton(uint8_t escapeButtonId);
		uint8_t getDefaultEnterButton() const;
		uint8_t getDefaultEscapeButton() const;

		bool hasPriority() const;
		void setPriority(bool priority);

		const ModalWindowChoiceList& getButtons() const;
		const ModalWindowChoiceList& getChoices() const;

	private:
		uint32_t id;
		std::string title, message;
		uint8_t defaultEnterButton, defaultEscapeButton;
		bool priority;

		ModalWindowChoiceList buttons, choices;
};

#endif
