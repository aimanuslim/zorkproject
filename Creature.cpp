/*
 * Creature.cpp
 *
 *  Created on: Dec 9, 2015
 *      Author: amduslim
 */

#include "Creature.h"
#include "Object.h"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include <sstream>

Creature::Creature(string name): Object(name),  attacked(NULL), creatureTrigger(NULL) {
	// TODO Auto-generated constructor stub

}

Triggers * Creature::checkAttackedTrigger(Room * r, Item * weapon){
	if(!attacked) return NULL;

//	// if its a has condition
//	if((attacked->cond.find("has") != attacked->cond.end())){
//		Item * item = r->findItem(attacked->cond["object"]);
//		Container * cont = r->findContainer(attacked->cond["owner"]);
//		if(item && cont){
//			if(attacked->cond["has"] == "yes"){
//				// if item is in container check
//				if(cont->findItem(item->name)){
//					return attacked;
//				}
//			} else {
//				// if item is not in container check
//				if(!(cont->findItem(item->name))){
//					return attacked;
//				}
//			}
//		return NULL;
//		} else {

	std::list<string>::iterator findIter = std::find(vulnerability.begin(), vulnerability.end(), weapon->name);
	if(findIter != vulnerability.end()){ // weapon is a vulnerability
		// object and status
		if(weapon->status == attacked->cond["status"]){
			// if both object exist, and its status matches the trigger's specified status
			return attacked;
		}
	}
	return NULL;

}

Triggers * Creature::checkCreatureTrigger(Room * r, Container * inv, InputFormat myCom, std::map<Command, string> mapping){
	if(!creatureTrigger) return NULL;

	if(creatureTrigger->command != ""){
		std::istringstream buf(creatureTrigger->command);
		std::istream_iterator<std::string> beg(buf), end;
		std::vector<std::string> cobrk(beg, end); // word list
		if(mapping[myCom.com] != cobrk[0] || myCom.obj1 != cobrk[1] || myCom.obj2 != cobrk[3]) return NULL;
	}


	if(creatureTrigger->triggered == true && creatureTrigger->type == "single") return NULL;

	// if its a has condition
	if((creatureTrigger->cond.find("has") != creatureTrigger->cond.end())){
		Item * item = r->findItem(creatureTrigger->cond["object"]);
		Container * cont = r->findContainer(creatureTrigger->cond["owner"]);
		if(item && cont){
			if(creatureTrigger->cond["has"] == "yes"){
				// if item is in container check
				if(cont->findItem(item->name)){
					if(creatureTrigger->type=="single" && creatureTrigger->triggered == false){
						creatureTrigger->triggered = true;
						return creatureTrigger;
					}
					if(creatureTrigger->type=="permanent") return creatureTrigger;
				}
			} else {
				// if item is not in container check
				if(!(cont->findItem(item->name))){
					if(creatureTrigger->type=="single" && creatureTrigger->triggered == false){
						creatureTrigger->triggered = true;
						return creatureTrigger;
					}
					if(creatureTrigger->type=="permanent") return creatureTrigger;
				}
			}
		}
	} else {
		// object and status
		Item * obj = r->findItem(creatureTrigger->cond["object"]);
		if((obj)){
			if(obj->status == creatureTrigger->cond["status"]){
				// if both object exist, and its status matches the trigger's specified status
				if(creatureTrigger->type=="single" && creatureTrigger->triggered == false){
					creatureTrigger->triggered = true;
					return creatureTrigger;
				}
				if(creatureTrigger->type=="permanent") return creatureTrigger;
			}
		}

	}

	Item * obj = inv->findItem(creatureTrigger->cond["object"]);
	if((obj)){
		if(obj->status == creatureTrigger->cond["status"]){
			// if both object exist, and its status matches the trigger's specified status
			if(creatureTrigger->type=="single" && creatureTrigger->triggered == false){
				creatureTrigger->triggered = true;
				return creatureTrigger;
			}
			if(creatureTrigger->type=="permanent") return creatureTrigger;
		}
	}

	return NULL;
}

Creature::~Creature() {
	// TODO Auto-generated destructor stub
	delete attacked;
	delete creatureTrigger;
	vulnerability.clear();
}
