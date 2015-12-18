/*
 * Container.cpp
 *
 *  Created on: Dec 9, 2015
 *      Author: amduslim
 */

#include "Container.h"

#include <stdlib.h>
#include <iostream>
#include <string>

using std::string;

Container::Container(const string name): Object(name), triggers(NULL), isOpen(false) {
	// TODO Auto-generated constructor stub

}

void Container::printItems(){
	isOpen = true;

	std::list<Item*>::iterator it;
	if(items.empty()){
		std::cout << name << " is empty." << std::endl;
	}
	else{
		int i = 0;
		std::cout << name << " contains ";
		int vecsize = items.size();
		for(it=items.begin();it!=items.end();++it){
			std::cout << (*it)->name;
			i++;
			if(i != vecsize) std::cout << ", ";
		}
		std::cout << std::endl;
	}
}

Triggers * Container::checkTriggers(){
	if(!triggers) return NULL;

	// if its a has condition
	if((triggers->cond.find("has") != triggers->cond.end())){
		Item * item = findItem(triggers->cond["object"]);
		if(item){
			if(triggers->cond["has"] == "yes"){
				// if item is in container
				if(triggers->type=="single" && triggers->triggered == false){
					triggers->triggered = true;
					return triggers;
				}
				if(triggers->type=="permanent") return triggers;
			} else {
				// if item is not in container check
				if(triggers->type=="single" && triggers->triggered == false){
					triggers->triggered = true;
					return triggers;
				}
				if(triggers->type=="permanent") return triggers;
			}
		}

		// May not need to check inventory
//		if((triggers->cond["owner"] != "inventory") && item){
//			if(triggers->cond["has"] == "yes"){
//				// if item is in container check
//				if(inv->findItem(item->name)){
//					if(triggers->type=="single") triggered = true;
//					if(triggered == false) return triggers;
//				}
//			} else {
//				// if item is not in container check
//				if(!(inv->findItem(item->name))){
//					if(triggers->type=="single") triggered = true;
//					if(triggered == false) return triggers;
//				}
//			}
//		}
	} else {
		// object and status
		Item * obj = findItem(triggers->cond["object"]);
		if(!(obj)){
			if(obj->status == triggers->cond["status"]){
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

Item *  Container::findItem(string itemName){
	std::list<Item*>::iterator i_iter;
	for(i_iter=this->items.begin(); i_iter!=this->items.end();++i_iter){
		if((*i_iter)->name==itemName){
			return (*i_iter);
		}
	}
	return NULL;
}

void Container::removeItem(string obj){
	std::list<Item*>::iterator i_iter;
	for(i_iter=this->items.begin(); i_iter!=this->items.end();++i_iter){
		if((*i_iter)->name==obj){
			items.remove((*i_iter));
			return;
		}
	}
}

Item *  Container::findItemIfOpen(string itemName){
	if(!isOpen) return NULL;
	std::list<Item*>::iterator i_iter;
	for(i_iter=this->items.begin(); i_iter!=this->items.end();++i_iter){
		if((*i_iter)->name==itemName){
			return (*i_iter);
		}
	}
	return NULL;
}


Container::~Container() {
	// TODO Auto-generated destructor stub
	delete triggers;
	items.clear();
	accept.clear();
}
