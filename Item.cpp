/*
 * Item.cpp
 *
 *  Created on: Dec 9, 2015
 *      Author: amduslim
 */

#include "Item.h"

#include <stdlib.h>
#include <iostream>

Item::Item(string name): Object(name), writing(""), turn_on_notify(""), turn_on_update("") {
	// TODO Auto-generated constructor stub

}

void Item::printWriting(){
	if(this->writing != ""){
		std::cout << this->writing << std::endl;
	} else {
//		std::cout << this->name << " has no writing on it." << std::endl;
		std::cout << "Nothing written." << std::endl;
	}
}

Item::~Item() {
	// TODO Auto-generated destructor stub
}
