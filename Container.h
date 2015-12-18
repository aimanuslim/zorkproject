/*
 * Container.h
 *
 *  Created on: Dec 9, 2015
 *      Author: amduslim
 */

#ifndef CONTAINER_H_
#define CONTAINER_H_
#include <list>
#include "Item.h"
#include "Object.h"
using std::list;
using std::string;


class Container: public Object {
public:
	list<string> accept;
	list<Item*> items;
	Triggers * triggers;
	bool isOpen;
	Triggers * checkTriggers();
	Item * findItem(string itemName);
	Item * findItemIfOpen(string itemName);
	void removeItem(string);
	void printItems();
	Container(const string name);
	virtual ~Container();
};

#endif /* CONTAINER_H_ */
