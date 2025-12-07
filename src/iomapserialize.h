// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_IOMAPSERIALIZE_H
#define FS_IOMAPSERIALIZE_H

class Container;
class House;
class Item;
class Map;
class PropStream;
class PropWriteStream;
class Thing;
class Tile;

class IOMapSerialize
{
public:
	static void loadHouseItems(Map* map);
	static bool saveHouseItems();
	static bool loadHouseInfo();
	static bool saveHouseInfo();

	static bool saveHouse(House* house);

private:
	static void saveItem(PropWriteStream& stream, const std::shared_ptr<const Item>& item);
	static void saveTile(PropWriteStream& stream, const std::shared_ptr<const Tile>& tile);

	static bool loadContainer(PropStream& propStream, const std::shared_ptr<Container>& container);
	static bool loadItem(PropStream& propStream, const std::shared_ptr<Thing>& parent);
};

#endif // FS_IOMAPSERIALIZE_H
