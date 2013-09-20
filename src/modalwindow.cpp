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

#include "modalwindow.h"

ModalWindow::ModalWindow(uint32_t id, const std::string& title, const std::string& message)
	: defaultEnterButton(0xFF), defaultEscapeButton(0xFF), priority(false)
{
	this->id = id;
	this->title = title;
	this->message = message;
}

std::string ModalWindow::getTitle() const
{
	return title;
}

std::string ModalWindow::getMessage() const
{
	return message;
}

uint32_t ModalWindow::getID() const
{
	return id;
}

uint32_t ModalWindow::getButtonCount() const
{
	return buttons.size();
}

uint32_t ModalWindow::getChoiceCount() const
{
	return choices.size();
}

void ModalWindow::addChoice(uint8_t choiceId, const std::string& text)
{
	choices.emplace_back(text, choiceId);
}

void ModalWindow::addButton(uint8_t buttonId, const std::string& text)
{
	buttons.emplace_back(text, buttonId);
}

const ModalWindowChoiceList& ModalWindow::getButtons() const
{
	return buttons;
}

const ModalWindowChoiceList& ModalWindow::getChoices() const
{
	return choices;
}

void ModalWindow::setDefaultEnterButton(uint8_t enterButtonId)
{
	defaultEnterButton = enterButtonId;
}

uint8_t ModalWindow::getDefaultEnterButton() const
{
	return defaultEnterButton;
}

void ModalWindow::setDefaultEscapeButton(uint8_t escapeButtonId)
{
	defaultEscapeButton = escapeButtonId;
}

uint8_t ModalWindow::getDefaultEscapeButton() const
{
	return defaultEscapeButton;
}

bool ModalWindow::hasPriority() const
{
	return priority;
}

void ModalWindow::setPriority(bool priority)
{
	this->priority = priority;
}
