/*
 * Triggers.h
 *
 *  Created on: Dec 9, 2015
 *      Author: amduslim
 */

#ifndef TRIGGERS_H_
#define TRIGGERS_H_


#include "tinyxml.h"
#include "tinystr.h"
#include "Object.h"

#include <string>
#include <map>
#include <list>

using std::map;
using std::string;

class Object;

typedef struct {
	bool has_condition;
	map<string, Object*> myCond;
} conditions;

class Triggers{
public:
	Triggers();
	string type;
	bool triggered;
//	map<string,string> events; // actions, prints and commands
	std::list<string> action;
	std::list<string> print;
	string command;
	map<string,string> cond;
//	conditions cond;
	virtual ~Triggers();
};




#endif /* TRIGGERS_H_ */
