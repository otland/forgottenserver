////////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
////////////////////////////////////////////////////////////////////////
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////

#ifndef __WAYPOINTS__
#define __WAYPOINTS__
#include "otsystem.h"

class Waypoint
{
	public:
		Waypoint(const std::string& _name, const Position& _pos):
			name(_name), pos(_pos) {}

		std::string name;
		Position pos;
};

typedef boost::shared_ptr<Waypoint> WaypointPtr;
typedef std::map<std::string, WaypointPtr> WaypointMap;

class Waypoints
{
	public:
		// Does not require either constructor nor destructor
		inline void addWaypoint(WaypointPtr waypoint);
		WaypointPtr getWaypointByName(const std::string& name) const;
		const WaypointMap& getWaypointsMap() const {
			return waypoints;
		}

	protected:
		WaypointMap waypoints;
};


inline void Waypoints::addWaypoint(WaypointPtr waypoint)
{
	waypoints[waypoint->name] = waypoint;
}

inline WaypointPtr Waypoints::getWaypointByName(const std::string& name) const
{
	WaypointMap::const_iterator it = waypoints.find(name);

	if (it != waypoints.end()) {
		return it->second;
	}

	return WaypointPtr();
}
#endif
