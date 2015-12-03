/*
 * Room.h
 *
 *  Created on: Dec 2, 2015
 *      Author: amduslim
 */

#ifndef ROOM_H_
#define ROOM_H_

#include "Zork.h"
#include "Object.h"

class Room: public Object {
public:
	Room();
	string description;
	Type type;
	vector <Dir> border;
	vector <Container> containers;
	vector <Item> items;
	vector <Creature> creatures;


	virtual ~Room();
};

#endif /* ROOM_H_ */
