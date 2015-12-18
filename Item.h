/*
 * Item.h
 *
 *  Created on: Dec 9, 2015
 *      Author: amduslim
 */

#ifndef ITEM_H_
#define ITEM_H_

#include "Zork.h"
#include "Object.h"

#include <string>
using std::string;

class Item : public Object {
public:
	string writing;
	string turn_on_notify;
	string turn_on_update;
	void printWriting();
	Item(string name);
	virtual ~Item();
};



#endif /* ITEM_H_ */
