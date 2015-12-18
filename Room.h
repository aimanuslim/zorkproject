/*
 * Room.h
 *
 *  Created on: Dec 2, 2015
 *      Author: amduslim
 */

#ifndef ROOM_H_
#define ROOM_H_

#include "Zork.h"
#include "Container.h"
#include "Item.h"
#include "Creature.h"
#include "Object.h"
#include "Triggers.h"
#include "Map.h"

#include <list>

using std::list;
using std::string;

class Creature;

class Room: public Object {
public:
	Room(string name);
	list <Container*> containers;
	list <Item*> items;
	list <Creature*> creatures;
//	list <Border*> border;
	Triggers * triggers;
	Item * findItem(string itemName);
	Container * findContainer(string containerName);
	Creature * findCreature(string creatureName);
	Item * findInContainers(string);
	void removeFromContainer(string);
	Triggers * checkTriggers(Container *, string);
	virtual ~Room();
private:
};

struct  RoomNode{
	Room * room;
	// this contains information about all the rooms connected to this room
	// first string is direction, second string is the name of the next room
	map<string, string> nextRoom;


};

#endif /* ROOM_H_ */
