/*
 * Room.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: amduslim
 */

#include "Room.h"
#include <stdlib.h>
using std::string;
Room::Room(string name) : Object(name), triggers(NULL){
	// TODO Auto-generated constructor stub

}

Item *  Room::findItem(string itemName){
	std::list<Item*>::iterator i_iter;
	for(i_iter=this->items.begin(); i_iter!=this->items.end();++i_iter){
		if((*i_iter)->name==itemName){
			return (*i_iter);
		}
	}
	return NULL;
}

Container *  Room::findContainer(string containerName){
	std::list<Container*>::iterator c_iter;
	for(c_iter=this->containers.begin(); c_iter!=(this->containers.end());++c_iter){
		if((*c_iter)->name==containerName){
			return (*c_iter);
		}
	}
	return NULL;
}

Creature * Room::findCreature(string creatureName){
	std::list<Creature*>::iterator creat_iter;
	for(creat_iter=this->creatures.begin(); creat_iter!=this->creatures.end();++creat_iter){
		if((*creat_iter)->name==creatureName){
			return (*creat_iter);
		}
	}
	return NULL;
}

Item * Room::findInContainers(string obj){
	std::list<Container*>::iterator cit;
	for(cit = containers.begin(); cit!=containers.end(); ++cit){
		if((*cit)->findItemIfOpen(obj)) return (*cit)->findItemIfOpen(obj);
	}
	return NULL;
}

void Room::removeFromContainer(string obj){
	std::list<Container*>::iterator cit;
	for(cit = containers.begin(); cit!=containers.end(); ++cit){
		if((*cit)->findItem(obj)) (*cit)->removeItem(obj);
	}
}

Triggers * Room::checkTriggers(Container * inv, string com){
	if(!triggers) return NULL;

	if(triggers->command != com) return NULL;
	// if its a has condition
	if((triggers->cond.find("has") != triggers->cond.end())){
		Item * item = findItem(triggers->cond["object"]);
		Container * cont = findContainer(triggers->cond["owner"]);
		if(item && cont){
			if(triggers->cond["has"] == "yes"){
				// if item is in container check
				if(cont->findItem(item->name)){
					if(triggers->type=="single" && triggers->triggered == false){
						triggers->triggered = true;
						return triggers;
					}
					if(triggers->type=="permanent") return triggers;
				}
			} else {
				// if item is not in container check
				if(!(cont->findItem(item->name))){
					if(triggers->type=="single" && triggers->triggered == false){
						triggers->triggered = true;
						return triggers;
					}
					if(triggers->type=="permanent") return triggers;
				}
			}
		}

		if((triggers->cond["owner"] == "inventory") && item){
			if(triggers->cond["has"] == "yes"){
				// if item is in container check
				if(inv->findItem(item->name)){
					if(triggers->type=="single" && triggers->triggered == false){
						triggers->triggered = true;
						return triggers;
					}
					if(triggers->type=="permanent") return triggers;
				}
			} else {
				// if item is not in container check
				if(!(inv->findItem(item->name))){
					if(triggers->type=="single" && triggers->triggered == false){
						triggers->triggered = true;
						return triggers;
					}
					if(triggers->type=="permanent") return triggers;
				}
			}
		}
	} else {
		// object and status
		Item * obj = findItem(triggers->cond["object"]);
		if((obj)){
			if(obj->status == triggers->cond["status"]){
				// if both object exist, and its status matches the trigger's specified status
				if(triggers->type=="single" && triggers->triggered == false){
					triggers->triggered = true;
					return triggers;
				}
				if(triggers->type=="permanent") return triggers;
			}
		}
		Container * cont = findContainer(triggers->cond["object"]);
		if((cont)){
			if(cont->status == triggers->cond["status"]){
				// if both object exist, and its status matches the trigger's specified status
				if(triggers->type=="single" && triggers->triggered == false){
					triggers->triggered = true;
					return triggers;
				}
				if(triggers->type=="permanent") return triggers;
			}
		}
	}
	return NULL;
}

Room::~Room() {
	// TODO Auto-generated destructor stub
	delete triggers;
	containers.clear();
	items.clear();
	creatures.clear();

}
