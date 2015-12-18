/*
 * Map.cpp
 *
 *  Created on: Dec 10, 2015
 *      Author: amduslim
 */

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <iterator>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <boost/algorithm/string.hpp>

#include "Container.h"
#include "Map.h"

using namespace std;


Map::Map(): inventory(new Container("inventory")), victory(false) {
	// TODO Auto-generated constructor stub
	commandMapping.insert( pair<string, Command>("n", n));
	commandMapping.insert( pair<string, Command>("s", s));
	commandMapping.insert( pair<string, Command>("e", e));
	commandMapping.insert( pair<string, Command>("w", w));
	commandMapping.insert( pair<string, Command>("i", i));
	commandMapping.insert( pair<string, Command>("take", take));
	commandMapping.insert( pair<string, Command>("open", open) );
	commandMapping.insert( pair<string, Command>("open exit", open_exit) );
	commandMapping.insert( pair<string, Command>("read", read_act));
	commandMapping.insert( pair<string, Command>("drop", drop));
	commandMapping.insert( pair<string, Command>("put", put));
	commandMapping.insert( pair<string, Command>("turn", turn_on));
	commandMapping.insert( pair<string, Command>("attack", attack));

	reverseCM.insert( pair<Command, string>(n, "n"));
	reverseCM.insert( pair<Command, string>(s, "s"));
	reverseCM.insert( pair<Command, string>(e, "e"));
	reverseCM.insert( pair<Command, string>(w, "w"));
	reverseCM.insert( pair<Command, string>(i, "i"));
	reverseCM.insert( pair<Command, string>(take, "take"));
	reverseCM.insert( pair<Command, string>(open, "open") );
	reverseCM.insert( pair<Command, string>(open_exit, "open exit") );
	reverseCM.insert( pair<Command, string>(read_act, "read"));
	reverseCM.insert( pair<Command, string>(drop, "drop"));
	reverseCM.insert( pair<Command, string>(put, "put"));
	reverseCM.insert( pair<Command, string>(turn_on, "turn"));
	reverseCM.insert( pair<Command, string>(attack, "attack"));


}

void Map::showInventory(){
	std::list<Item>::iterator i_iter;

	if(inventory->items.begin() == inventory->items.end()) {
		cout << "Inventory: empty" << endl;
	}
	else {
		std::list<Item*>::iterator i_iter;
		int vecsize = inventory->items.size();
		int i = 0;
		cout << "Inventory: ";
		for(i_iter = inventory->items.begin(); i_iter != inventory->items.end(); ++i_iter){
			cout << (*i_iter)->name ;
			i++;
			if(i != vecsize) std::cout << ", ";
		}
		cout << endl;
	}

}

void Map::moveRoom(Command c){
	string direction;
	switch(c){
		case n: direction = "north"; break;
		case s: direction = "south"; break;
		case e: direction = "east"; break;
		case w: direction = "west"; break;
		default: break;
	}
	if(currRoom->nextRoom.find(direction) == currRoom->nextRoom.end()){
		cout << "Can't go that way." << endl;
	}
	else if(currRoom->nextRoom[direction] == "removed_border"){
		cout << "Can't go that way." << endl;
	}
	else{
		currRoom = roomM[currRoom->nextRoom[direction]];
		cout << currRoom->room->description << endl;
	}
}

bool Map::validateInput(InputFormat in){
	if(in.com == no_command){
		std::cout << "Error." << std::endl;
		return false;
	}

	switch(in.com){
			case n: case s: case e: case w:
			case i:
				if(!(in.obj1 == "")|| !(in.obj2 == "")){
					printf("Error");
					return false;
				}
				break;
			case take:
			case open_exit:
			case open:
			case read_act:
			case turn_on:
			case drop:
				if(in.obj2 != ""){
					printf("Error");
					return false;
				}
				break;
			case put:
			case attack:
				if((in.obj2 == "")){
					printf("Error");
					return false;
				}
				break;
			default: break;
	}
	return true;
}

InputFormat Map::parseInput(string in){
//	string in;
////	cout << ">";
//	cin >> in;

	std::istringstream buf(in);
	std::istream_iterator<std::string> beg(buf), end;

	std::vector<std::string> tokens(beg, end);
	std::vector<string>::iterator sit = tokens.begin();

	InputFormat myinput;
	myinput.obj1 = "";
	myinput.obj2 = "";
	myinput.com = no_command;

	switch(tokens.size()){
		case 4:
			myinput.obj2 = tokens[3];
			myinput.obj1 = tokens[1];
			if(commandMapping.find(tokens[0]) != commandMapping.end()) myinput.com = commandMapping[tokens[0]];
			break;
		case 3: // for turn on
			myinput.obj1 = tokens[2];
			if(commandMapping.find(tokens[0]) != commandMapping.end()) myinput.com = commandMapping[tokens[0]];
			break;
		case 2:
			myinput.obj1 = tokens[1];
			if(commandMapping.find(tokens[0]) != commandMapping.end()) myinput.com = commandMapping[tokens[0]];
			if(myinput.obj1 == "exit" && myinput.com == open) myinput.com = open_exit;
			break;
		case 1:
			if(commandMapping.find(tokens[0]) != commandMapping.end()) myinput.com = commandMapping[tokens[0]];
	}
	return myinput;
}

void Map::applyTrigger(Object * obj, Triggers * trig){
	std::list<string>::iterator print_iter;
	for(print_iter = trig->print.begin(); print_iter != trig->print.end(); ++print_iter){
		cout << *print_iter << endl;
	}

	std::list<string>::iterator action_iter;
	for(action_iter = trig->action.begin(); action_iter != trig->action.end(); ++action_iter){
		parseActionandApply((*action_iter));
	}


}

void Map::processInput(InputFormat in){

	Item * foundItem;
	Container * foundCont;
	Item * I;
	Creature * creature;
	Triggers * t;
	std::list<string>::iterator f;
	switch(in.com){
			case n: case s: case e: case w: moveRoom(in.com); return;
			case i: showInventory(); return;
			case take:
				// find object in current room
				foundItem = currRoom->room->findItem(in.obj1);
				if(!foundItem) { // find in containers instead
					foundItem = currRoom->room->findInContainers(in.obj1);
				}
				if(!foundItem){
					cout << "Error." << endl;
				} else {
					// add to inventory
					std::cout << "Item " << in.obj1 << " added to inventory." << std::endl;
					currRoom->room->items.remove(foundItem);
					currRoom->room->removeFromContainer(foundItem->name);
					Add(foundItem, inventory);
					checkCreatureContainerTrigger(in);
				}
				break;
			case open_exit:
				if(currRoom->room->type == "exit"){
					victory = true;
				} else {
//					cout << "No where to exit from." << endl;
					cout << "Error." << endl;
				}
				break;
			case open:
				foundCont = currRoom->room->findContainer(in.obj1);
				if(!(foundCont)){
					cout << "Error." << endl;
				} else{
					// open container
					foundCont->printItems();
				}
				break;
			case read_act:
				I = inventory->findItem(in.obj1);
				if(!(I)){
//					std::cout << "You realized you don't have " << in.obj1 << std::endl;
					std::cout << "Error." << std::endl;
				} else {
					I->printWriting();
				}
				break;
			case turn_on:
				I = inventory->findItem(in.obj1);
				if(!(I)){
//					cout << "You realized you didn't have" << in.obj1 << "."<< endl;
					std::cout << "Error." << std::endl;
				} else {
					if((I->turn_on_notify != "") && (I->turn_on_update != "")){
						cout << "You activate the " << in.obj1 << "." << endl;
						cout << I->turn_on_notify << endl;
//						Update(I, I->turn_on_update);
						parseActionandApply(I->turn_on_update);
						checkCreatureContainerTrigger(in);
					} else {
						cout << in.obj1 << " cannot be activated." << endl;
					}
				}
				break;
			case drop:
				I = inventory->findItem(in.obj1);
				if(!(I)){
					cout << "You really want to drop " << in.obj1 << ", but you realized you don't have any." << endl;
				}else{
//					cout << "You dropped " << in.obj1 << " in " << currRoom->room->name << "." << endl;
					cout << in.obj1 << " dropped." << endl;
					inventory->items.remove(I);
					currRoom->room->items.push_back(I);
				}
				break;
			case put:
				I = inventory->findItem(in.obj1);
				if(!(I)){
//					cout << "You want to put " << in.obj1 << " in " << in.obj2 << " but you realized you don't have any " << in.obj1 << "." << endl;
					cout << "Error" << endl;
				} else {
					inventory->removeItem(in.obj1);
					foundCont = currRoom->room->findContainer(in.obj2);
					if(foundCont){ foundCont->items.push_back(I); }
					else { cout << "No " << in.obj2 << " in here."; }
					cout << "Item " << in.obj1 << " added to " << in.obj2 << "." << endl;
					checkCreatureContainerTrigger(in);
				}
				break;
			case attack:
				creature = currRoom->room->findCreature(in.obj1);
				if(!creature){ cout << "Error." << endl; return;}
				I = inventory->findItem(in.obj2);
				if(!I){ cout << "Error." << endl; return;}
//				cout << "You assaulted " << in.obj1 << " with " << in.obj2 << "." << endl;

				// Dragon case. No attack trigger, but has 'normal' trigger
				f = std::find(creature->vulnerability.begin(), creature->vulnerability.end(), in.obj2);
				if(creature->attacked == NULL && (f != creature->vulnerability.end())) {
					cout << "You assault the " << in.obj1 << " with " << in.obj2 << "." << endl;
					return;
				} else {
					t = creature->checkAttackedTrigger(currRoom->room, I);
					if(t){
						cout << "You assault the " << in.obj1 << " with " << in.obj2 << "." << endl;
						applyTrigger(dynamic_cast<Object*>(creature), t);  return; }
					else {
						cout << "Error." << endl;
					}
				}
//				else { cout << "Its not very effective.. " << endl; }

				break;
			default: break;
	}





}




bool Map::checkRoomTrigger(InputFormat in){

	// check trigger for actions within room
	if(Triggers * yesTrigger = currRoom->room->checkTriggers(this->inventory, reverseCM[in.com])){
		applyTrigger(currRoom->room, yesTrigger);
		return true;
	}
	return false;
}

bool Map::checkCreatureContainerTrigger(InputFormat in){
	/* Note: looks like creatures and containers triggers are mostly checked after a certain command is executed, while the room's triggers checked before the command gets executed. */
	// check for trigger for actions on creatures in the room
	std::list<Creature*>::iterator creat_iter;
	for(creat_iter= currRoom->room->creatures.begin(); creat_iter != currRoom->room->creatures.end(); ++creat_iter){
		Triggers * yesTrigger = (*creat_iter)->checkCreatureTrigger(currRoom->room, inventory, in, reverseCM);
		if(yesTrigger){ applyTrigger(dynamic_cast<Object*>(*creat_iter), yesTrigger); return true; }
	}

	std::list<Container*>::iterator cont_iter;
	for(cont_iter= currRoom->room->containers.begin(); cont_iter != currRoom->room->containers.end(); ++cont_iter){
		Triggers * yesTrigger = (*cont_iter)->checkTriggers();
		if(yesTrigger){ applyTrigger(dynamic_cast<Object*>(*cont_iter), yesTrigger); return true; }
	}
	return false;
}

void Map::Update(Object * obj, string new_status){
	std::istringstream buf(new_status);
	std::istream_iterator<std::string> beg(buf), end;
	std::vector<std::string> vec(beg, end); // word list
	obj->status = vec[3];
}

void Map::Delete(Object * o){
	if(currRoom->room->findContainer(o->name)){
		currRoom->room->containers.remove(dynamic_cast<Container*>(o));
		removedObj.insert(pair<string, Object*>(o->name, o));
		return;
	}

	if(currRoom->room->findItem(o->name)){
		currRoom->room->items.remove(dynamic_cast<Item*>(o));
		removedObj.insert(pair<string, Object*>(o->name, o));
		return;
	}

	if(currRoom->room->findCreature(o->name)){
		currRoom->room->creatures.remove(dynamic_cast<Creature*>(o));
		removedObj.insert(pair<string, Object*>(o->name, o));
		return;
	}

	if(findItemContainer(o->name)){
		removeObjInAllRoom(o->name);
		removedObj.insert(pair<string, Object*>(o->name, o));
	}

	if(dynamic_cast<Room*>(o)){
		std::map<string, string>::iterator ri;
		std::map<string,RoomNode*>::iterator it;
		RoomNode * rn;
		for(it=roomM.begin();it!=roomM.end();++it){
			// for each room node
			rn = it->second;
			// for each nextroom (borders definition)
			for(ri=rn->nextRoom.begin();ri!=rn->nextRoom.end();++ri){
				if(ri->second==o->name){
					ri->second = "removed_border";
				}
			}
		}
		it = roomM.find(o->name);
		roomM.erase(it);
		delete o;
	}
}

void Map::removeObjInAllRoom(string obj){
	std::map<string, RoomNode*>::iterator roomit;
	Object * o;
	for(roomit = roomM.begin(); roomit != roomM.end(); ++roomit){
		o = roomit->second->room->findContainer(obj);
		if(o){ roomit->second->room->containers.remove(dynamic_cast<Container*>(o)); return; }
		o = roomit->second->room->findItem(obj);
		if(o){ roomit->second->room->items.remove(dynamic_cast<Item*>(o)); return; }
		o = roomit->second->room->findCreature(obj);
		if(o) {roomit->second->room->creatures.remove(dynamic_cast<Creature*>(o)); return;}
	}
}


Object * Map::findItemContainer(string obj_name){
	if(allItemandContainers.find(obj_name) == allItemandContainers.end()){ return NULL; }
	return allItemandContainers[obj_name];
}


void Map::parseActionandApply(string update_message){
	std::istringstream buf(update_message);
	std::istream_iterator<std::string> beg(buf), end;
	std::vector<std::string> wordl(beg, end); // word list
	if(wordl[0] == "Update"){
		string obj_name = wordl[1];
		string new_status = wordl[3];
		Object * o = currRoom->room->findContainer(obj_name) ? dynamic_cast<Object*>(currRoom->room->findContainer(obj_name)) : dynamic_cast<Object*>(currRoom->room->findItem(obj_name));
		if(!o) o = findItemContainer(obj_name);
		Update(o, update_message);
		return;
	}
	if(wordl[0] == "Delete"){
		string obj_name = wordl[1];
		Object * o = currRoom->room->findContainer(obj_name) ? dynamic_cast<Object*>(currRoom->room->findContainer(obj_name)) : dynamic_cast<Object*>(currRoom->room->findItem(obj_name));
		if(!o) o = currRoom->room->findCreature(obj_name);
		if(!o) o = findItemContainer(obj_name);
		Delete(o);
		return;
	}

	if(wordl[0] == "Add"){
		string obj_name = wordl[1];
		string cont = wordl[3];
		Object * obj = findItemContainer(obj_name);
		Object * place = (roomM.find(cont) != roomM.end()) ? roomM[cont]->room : findItemContainer(cont);
		Add(obj, place);
		return;
	}

	if(commandMapping.find(wordl[0]) != commandMapping.end()){ // action is a command
		InputFormat in;
		in = parseInput(update_message);
		processInput(in);
		return;
	}

	if(wordl[0] == "Game" && wordl[1] == "Over"){
		victory = true;
		return;
	}

}




void Map::Add(Object * obj, Object * place){
	// place can either be room/container
	Container * c;
	Room * r;
	// obj can either be items, creatures or containers
	Container * cont;
	Item * it;
	Creature * creat;

	// for container
	if(dynamic_cast<Container*>(place)){
		c = dynamic_cast<Container*>(place);
		c->items.push_back(dynamic_cast<Item*>(obj));
	}


	// for room
	if(dynamic_cast<Room*>(place)){
		r = dynamic_cast<Room*>(place);
		it = dynamic_cast<Item*>(obj);
		cont = dynamic_cast<Container*>(obj);
		creat = dynamic_cast<Creature*>(obj);
		if(it){ r->items.push_back(it); }
		if(cont){ r->containers.push_back(cont); }
		if(creat){ r->creatures.push_back(creat); }
	}
}

void Map::start(){
	string line;
	InputFormat in;

	bool valid = false;
	bool roomTriggered = false;
	bool ccTriggered = false;
	victory = false;

	this->currRoom = roomM["Entrance"];
	cout << this->currRoom->room->description << endl;

	while(!victory){
		valid = false;
		std::getline(cin, line);
		in  = parseInput(line);
		valid = validateInput(in);
		if(valid){
			roomTriggered = checkRoomTrigger(in);
			ccTriggered = checkCreatureContainerTrigger(in);
			if(!roomTriggered && !ccTriggered) processInput(in);
		}
//		else { cout << "Error." << endl; }
	}
	if(victory) cout << "Victory!" << endl;

}

void Map::runGame(){
	string filename;
//	cout << "Enter filename to simulate: ";
//	cin >> filename;
//	filename = "input_sample.txt";
//	filename = "Sample/samples/creaturesample.txt";
//	filename = "Sample/samples/triggersample.txt";
	filename = "Sample/samples/itemsample.txt";
//	filename = "Sample/samples/containersample.txt";
//	filename = "Sample/samples/roomsample.txt";
	std::ifstream infile(filename.c_str());
	bool valid = false;
	this->currRoom = roomM["Entrance"];
	cout << this->currRoom->room->description << endl;
	InputFormat in;
	bool roomTriggered = false;
	bool ccTriggered = false;
	victory = false;
	for( std::string line; getline( infile, line ); )
	{
		if(victory) break;
		valid = false;
		cout << "> " << line << endl;
		in  = parseInput(line);
		valid = validateInput(in);
		if(valid){
			roomTriggered = checkRoomTrigger(in);
			ccTriggered = checkCreatureContainerTrigger(in);
			if(!roomTriggered && !ccTriggered) processInput(in);
//			checkCreatureContainerTrigger(in);
//			roomTriggered = checkRoomTrigger(in);
//			ccTriggered = checkCreatureContainerTrigger(in);

		}
	}
	if(victory) cout << "Victory!" << endl;
}

Map::~Map() {
	// TODO Auto-generated destructor stub
	delete inventory;
	commandMapping.clear();
	reverseCM.clear();
	removedObj.clear();

	std::map<string, RoomNode*>::iterator roomit;
	for(roomit = roomM.begin(); roomit != roomM.end(); ++roomit){
		delete roomit->second->room;
		delete roomit->second;
	}

	std::map<string, Object*>::iterator allit;
	for(allit = allItemandContainers.begin(); allit != allItemandContainers.end(); ++allit){
		delete allit->second;
	}
}
