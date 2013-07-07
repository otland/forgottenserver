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

#include "thing.h"
#include "cylinder.h"
#include "tile.h"
#include "creature.h"
#include "item.h"
#include "player.h"

Thing::Thing()
{
	parent = NULL;
	useCount = 0;
}


Thing::~Thing()
{
	//
	//std::cout << "thing destructor " << this << std::endl;
}

Cylinder* Thing::getTopParent()
{
	//tile
	if (getParent() == NULL) {
		return dynamic_cast<Cylinder*>(this);
	}

	Cylinder* aux = getParent();
	Cylinder* prevaux = dynamic_cast<Cylinder*>(this);

	while (aux->getParent() != NULL) {
		prevaux = aux;
		aux = aux->getParent();
	}

	if (dynamic_cast<Cylinder*>(prevaux)) {
		return prevaux;
	}

	return aux;
}

const Cylinder* Thing::getTopParent() const
{
	//tile
	if (getParent() == NULL) {
		return dynamic_cast<const Cylinder*>(this);
	}

	const Cylinder* aux = getParent();

	const Cylinder* prevaux = dynamic_cast<const Cylinder*>(this);

	while (aux->getParent() != NULL) {
		prevaux = aux;
		aux = aux->getParent();
	}

	if (dynamic_cast<const Cylinder*>(prevaux)) {
		return prevaux;
	}

	return aux;
}

Tile* Thing::getTile()
{
	Cylinder* cylinder = getTopParent();

	//get root cylinder
	if (cylinder->getParent()) {
		cylinder = cylinder->getParent();
	}

	return dynamic_cast<Tile*>(cylinder);
}

const Tile* Thing::getTile() const
{
	const Cylinder* cylinder = getTopParent();

	//get root cylinder
	if (cylinder->getParent()) {
		cylinder = cylinder->getParent();
	}

	return dynamic_cast<const Tile*>(cylinder);
}

const Position& Thing::getPosition() const
{
	const Tile* tile = getTile();

	if (!tile) {
		return Tile::null_tile.getTilePosition();
	}

	return tile->getTilePosition();
}

bool Thing::isRemoved() const
{
	if (parent == NULL) {
		return true;
	}

	const Cylinder* aux = getParent();

	if (aux->isRemoved()) {
		return true;
	}

	return false;
}
