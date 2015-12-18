/*
 * Creature.h
 *
 *  Created on: Dec 9, 2015
 *      Author: amduslim
 */

#ifndef CREATURE_H_
#define CREATURE_H_

#include "Room.h"
#include "Item.h"
#include "Triggers.h"
#include "Zork.h"

#include <string>
using std::string;

class Room;
class Map;

class Creature: public Object {
public:
	std::list<string> vulnerability;
	Triggers * attacked;
	Triggers * creatureTrigger;
	Creature(string);
	Triggers * checkCreatureTrigger(Room *, Container *, InputFormat, std::map<Command, string>);
	Triggers * checkAttackedTrigger(Room *, Item *);
	virtual ~Creature();
};

#endif /* CREATURE_H_ */
