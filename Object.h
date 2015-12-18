/*
 * Object.h
 *
 *  Created on: Dec 2, 2015
 *      Author: amduslim
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "Zork.h"
#include "Triggers.h"
#include <string>
using std::string;

class Object {


public:
	Object(string nm);
	string name;
	string type;
	string description;
	string status;
	virtual ~Object();


};

#endif /* OBJECT_H_ */
