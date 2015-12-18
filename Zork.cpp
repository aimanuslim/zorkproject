/*
 * Zork.cpp
 *
 *  Created on: Dec 9, 2015
 *      Author: amduslim
 */

#include <iostream>
#include <stdlib.h>
#include <cstddef>
#include <map>
#include <utility>
#include <string>

#include "tinyxml.h"
#include "tinystr.h"
#include "Zork.h"
#include "Object.h"
#include "Room.h"
#include "Item.h"
#include "Container.h"
#include "Creature.h"
#include "Map.h"

using namespace std;

typedef std::map<std::string, Container*> containerMap;
typedef std::pair<std::string, Container*> containerPair;

typedef std::map<std::string, Item*> itemMap;
typedef std::pair<std::string, Item*> itemPair;

typedef std::map<std::string, Creature*> creatureMap;
typedef std::pair<std::string, Creature*> creaturePair;

Map * initializeObj(string filename){
	Map * myMap = new Map();
	TiXmlDocument doc(filename.c_str());
	doc.LoadFile();
	TiXmlDocument* xdoc = doc.GetDocument();
	TiXmlElement*e=xdoc->FirstChildElement();
	TiXmlHandle headHandle(e); // pass in map

	int i = 0;
	int j = 0;
	int k = 0;

	// find items
	itemMap itemM;
	Item * myItem;
	TiXmlHandle ch(NULL);
	while(headHandle.Child(i).Node()){
//		std::cout << headHandle.Child(i).ToElement()->Value() << std::endl;
		ch = headHandle.Child(i);
		string s(headHandle.Child(i).ToElement()->Value());
		if(s == "item"){
			myItem = new Item(ch.FirstChild("name").ToElement()->GetText());
			if(ch.FirstChild("description").Node()) myItem->description = ch.FirstChild("description").ToElement()->GetText();
			if(ch.FirstChild("writing").Node()) myItem->writing = ch.FirstChild("writing").ToElement()->GetText();
			if(ch.FirstChild("status").Node()) myItem->status = ch.FirstChild("status").ToElement()->GetText();
			if(ch.FirstChild("turnon").FirstChild("print").Node()) myItem->turn_on_notify = ch.FirstChild("turnon").FirstChild("print").ToElement()->GetText();
			if(ch.FirstChild("turnon").FirstChild("action").Node()) myItem->turn_on_update = ch.FirstChild("turnon").FirstChild("action").ToElement()->GetText();
			itemM.insert( itemPair(myItem->name, myItem));
			myMap->allItemandContainers.insert( std::pair<string, Object*>(myItem->name, myItem) );
		}
		i++;
	}

	// find containers
	i = 0;
	containerMap containerM;
	TiXmlHandle th(NULL);
	TiXmlHandle condh(NULL);
	Container * myContainer;
	while(headHandle.Child(i).Node()){
//		std::cout << headHandle.Child(i).ToElement()->Value() << std::endl;
		// handle for iterating through properties of a container
		TiXmlHandle ch1(headHandle.Child(i));
		// initialize container list
		list<Container> containerList;
		string s1(headHandle.Child(i).ToElement()->Value());
		if(s1 == "container"){
			myContainer = new Container(ch1.FirstChild("name").ToElement()->GetText());
			if(ch1.FirstChild("description").Node()) myContainer->description = ch1.FirstChild("description").ToElement()->GetText();
			if(ch1.FirstChild("accept").Node()) myContainer->accept.push_back(ch1.FirstChild("accept").ToElement()->GetText());
			if(ch1.FirstChild("item").Node()) myContainer->items.push_back(itemM[ch1.FirstChild("item").ToElement()->GetText()]);
			if(ch1.FirstChild("status").Node()) myContainer->status = ch1.FirstChild("status").ToElement()->GetText();

			if(ch1.FirstChild("trigger").Node()){
				th = ch1.FirstChild("trigger"); // handle for trigger
				myContainer->triggers = new Triggers();

				k =0;
				// fill in actions and prints
				while(th.Child(k).Node()){
					if(th.Child(k).Node()){
						string ap(th.Child(k).ToElement()->Value());
						if(ap == "print") myContainer->triggers->print.push_back(th.Child(k).ToElement()->GetText());
						if(ap == "action") myContainer->triggers->action.push_back(th.Child(k).ToElement()->GetText());
					}
					k++;
				}

				condh = th.FirstChild("condition"); // handle for conditions for the trigger
				if(condh.FirstChild("has").Node()) myContainer->triggers->cond.insert( std::pair<string,string>("has", condh.FirstChild("has").ToElement()->GetText()));
				if(condh.FirstChild("object").Node()) myContainer->triggers->cond.insert( std::pair<string,string>("object", condh.FirstChild("object").ToElement()->GetText()));
				if(condh.FirstChild("owner").Node()) myContainer->triggers->cond.insert( std::pair<string,string>("owner", condh.FirstChild("owner").ToElement()->GetText()));
				if(condh.FirstChild("status").Node()) myContainer->triggers->cond.insert( std::pair<string,string>("status", condh.FirstChild("status").ToElement()->GetText()));
			}
			string n(myContainer->name);
			containerM.insert( containerPair(n, myContainer) );
			myMap->allItemandContainers.insert( std::pair<string, Object*>(n, myContainer) );
		}
		i++;
	}

	// find creatures
	creatureMap creatuM;
	TiXmlHandle creature_handle(NULL);
	TiXmlHandle creature_th(NULL);
	TiXmlHandle attacked_th(NULL);
	TiXmlHandle crea_condh(NULL);
	TiXmlHandle vulne_h(NULL);
	TiXmlHandle attack_h(NULL);
	Creature * myCreature;
	i = 0;
	while(headHandle.Child(i).Node()){
		creature_handle = headHandle.Child(i);
		string cs(headHandle.Child(i).ToElement()->Value());
		if(cs == "creature"){
			myCreature = new Creature(creature_handle.FirstChild("name").ToElement()->GetText());
			if(creature_handle.FirstChild("status").Node()) myCreature->status = creature_handle.FirstChild("status").ToElement()->GetText();
			if(creature_handle.FirstChild("description").Node()) myCreature->description = creature_handle.FirstChild("description").ToElement()->GetText();
			// multiple vulnerabilities
			vulne_h = creature_handle;
			k = 0;
			while(vulne_h.Child(k).Node()){
				string v(vulne_h.Child(k).ToElement()->Value());
				if(v == "vulnerability") myCreature->vulnerability.push_back(vulne_h.Child(k).ToElement()->GetText());
				k++;
			}

			// fill in attacked trigger
			if(creature_handle.FirstChild("attack").Node()){
				attacked_th = creature_handle.FirstChild("attack");
				myCreature->attacked = new Triggers();
				k =0;
				if(attacked_th.FirstChild("command").Node()){ myCreature->attacked->command = attacked_th.FirstChild("command").ToElement()->GetText(); }
				while(attacked_th.Child(k).Node()){
					if(attacked_th.Child(k).Node()){
						string ap(attacked_th.Child(k).ToElement()->Value());
						if(ap == "print") myCreature->attacked->print.push_back(attacked_th.Child(k).ToElement()->GetText());
						if(ap == "action") myCreature->attacked->action.push_back(attacked_th.Child(k).ToElement()->GetText());
					}
					k++;
				}

				// fill in condition
				crea_condh = attacked_th.FirstChild("condition");
				if(crea_condh.FirstChild("has").Node()) myCreature->attacked->cond.insert( std::pair<string,string>("has", crea_condh.FirstChild("has").ToElement()->GetText()));
				if(crea_condh.FirstChild("object").Node()) myCreature->attacked->cond.insert( std::pair<string,string>("object", crea_condh.FirstChild("object").ToElement()->GetText()));
				if(crea_condh.FirstChild("owner").Node()) myCreature->attacked->cond.insert( std::pair<string,string>("owner", crea_condh.FirstChild("owner").ToElement()->GetText()));
				if(crea_condh.FirstChild("status").Node()) myCreature->attacked->cond.insert( std::pair<string,string>("status", crea_condh.FirstChild("status").ToElement()->GetText()));
			}

			// fill in creature trigger

			creature_th = creature_handle.FirstChild("trigger");
			myCreature->creatureTrigger = new Triggers();
			k = 0;
			if(creature_th.FirstChild("command").Node()){ myCreature->creatureTrigger->command = creature_th.FirstChild("command").ToElement()->GetText(); }
			while(creature_th.Child(k).Node()){
				if(creature_th.Child(k).Node()){
					string ap(creature_th.Child(k).ToElement()->Value());
					if(ap == "print") myCreature->creatureTrigger->print.push_back(creature_th.Child(k).ToElement()->GetText());
					if(ap == "action") myCreature->creatureTrigger->action.push_back(creature_th.Child(k).ToElement()->GetText());
				}
				k++;
			}

			// fill in condition
			crea_condh = creature_th.FirstChild("condition");
			if(crea_condh.FirstChild("has").Node()) myCreature->creatureTrigger->cond.insert( std::pair<string,string>("has", crea_condh.FirstChild("has").ToElement()->GetText()));
			if(crea_condh.FirstChild("object").Node()) myCreature->creatureTrigger->cond.insert( std::pair<string,string>("object", crea_condh.FirstChild("object").ToElement()->GetText()));
			if(crea_condh.FirstChild("owner").Node()) myCreature->creatureTrigger->cond.insert( std::pair<string,string>("owner", crea_condh.FirstChild("owner").ToElement()->GetText()));
			if(crea_condh.FirstChild("status").Node()) myCreature->creatureTrigger->cond.insert( std::pair<string,string>("status", crea_condh.FirstChild("status").ToElement()->GetText()));


			// put into
			creatuM.insert( creaturePair(myCreature->name, myCreature) );
			myMap->allItemandContainers.insert( std::pair<string, Object*>(myCreature->name, myCreature) );
		}
		i++;
	}


	// populate room
	RoomNode * ptrRN;
	i = 0;
	while(headHandle.Child(i).Node()){
		string s2(headHandle.Child(i).ToElement()->Value());
		// handle for iterating through all the items present in the room.
		TiXmlHandle ch3(headHandle.Child(i));
		if(s2 == "room"){
			// create a RoomNode
			ptrRN = new RoomNode();

			// initialize room name
			ptrRN->room = new Room(ch3.FirstChild("name").ToElement()->GetText());

			if(ch3.FirstChild("description").Node()) ptrRN->room->description = ch3.FirstChild("description").ToElement()->GetText();
			if(ch3.FirstChild("type").Node()) ptrRN->room->type = ch3.FirstChild("type").ToElement()->GetText();
			// loop through items
			j = 0;
			while(ch3.Child(j).Node()){
				string is(ch3.Child(j).ToElement()->Value()); // what kind of object in the room
				if(is == "item") ptrRN->room->items.push_back(itemM[ch3.Child(j).ToElement()->GetText()]);
				j++;
			}

			// loop through containers
			j = 0;
			while(ch3.Child(j).Node()){
				string is(ch3.Child(j).ToElement()->Value()); // what kind of object in the room
				if(is == "container") ptrRN->room->containers.push_back(containerM[ch3.Child(j).ToElement()->GetText()]);
				j++;
			}

			// loop through all creatures
			j = 0;
			while(ch3.Child(j).Node()){
				string is(ch3.Child(j).ToElement()->Value()); // what kind of object in the room
				if(is == "creature") ptrRN->room->creatures.push_back(creatuM[ch3.Child(j).ToElement()->GetText()]);
				j++;
			}

			// loop through all border
			j = 0;
			Border * b;
//			list<Border*>::const_iterator bit;
			while(ch3.Child(j).Node()){
				string is(ch3.Child(j).ToElement()->Value()); // what kind of object in the room
				if(is == "border"){
					b = new Border;
					b->name = ch3.Child(j).FirstChild("name").ToElement()->GetText();
					b->direction = ch3.Child(j).FirstChild("direction").ToElement()->GetText();
					ptrRN->nextRoom.insert(pair<string, string>(b->direction, b->name));
//					bit = ptrRN->room->border.begin();
//					cout << (*bit)->name << endl;
//					bit++;
					delete b;
				}
				j++;
			}

			// get triggers
			// (may not have actions for room triggers)
			TiXmlHandle roomth(ch3.FirstChild("trigger"));
			ptrRN->room->triggers = new Triggers();
			if(roomth.FirstChild("type").Node()){
//				cout << roomth.FirstChild("type").ToElement()->GetText() << endl;
				ptrRN->room->triggers->type = roomth.FirstChild("type").ToElement()->GetText();
			}
			if(roomth.FirstChild("command").Node()) ptrRN->room->triggers->command = roomth.FirstChild("command").ToElement()->GetText();
			k = 0;
			while(roomth.Child(k).Node()){
				string ap(roomth.Child(k).ToElement()->Value());
				if(ap == "action") ptrRN->room->triggers->action.push_back(roomth.Child(k).ToElement()->GetText());
				if(ap == "print") ptrRN->room->triggers->print.push_back(roomth.Child(k).ToElement()->GetText());
				k++;
			}

			// condition handle for the trigger
			TiXmlHandle roomcondth(roomth.FirstChild("condition"));
			// check for object, has, status, owner
			if(roomcondth.FirstChild("has").Node()) ptrRN->room->triggers->cond.insert( std::pair<string,string>("has", roomcondth.FirstChild("has").ToElement()->GetText()));
			if(roomcondth.FirstChild("status").Node()) ptrRN->room->triggers->cond.insert( std::pair<string,string>("status", roomcondth.FirstChild("status").ToElement()->GetText()));
			if(roomcondth.FirstChild("owner").Node()) ptrRN->room->triggers->cond.insert( std::pair<string,string>("owner", roomcondth.FirstChild("owner").ToElement()->GetText()));
			if(roomcondth.FirstChild("object").Node()) ptrRN->room->triggers->cond.insert( std::pair<string,string>("object", roomcondth.FirstChild("object").ToElement()->GetText()));
			myMap->roomM.insert( pair<string, RoomNode*>(ptrRN->room->name, ptrRN) );

		}
		i++; // next child (next element in map)

	}
	return myMap;
}

void printXml(){
	TiXmlDocument doc("sample.txt.xml");
		doc.LoadFile();
		TiXmlDocument* xdoc = doc.GetDocument();
//		TiXmlElement*e=xdoc->FirstChildElement()->FirstChildElement();
//		TiXmlNode*n = xdoc->FirstChild();
		TiXmlHandle docHandle(xdoc->FirstChildElement());

//	while(n){
		TiXmlElement*e=docHandle.FirstChild("room").FirstChild("trigger").FirstChild("condition").Child(0).ToElement();
		std::cout << e->GetText() << std::endl;
//		n = n->NextSibling();
//
//		std::cout << e->Value() << std::endl;
//		// for each room, enter properties
//		TiXmlElement * obj = e->FirstChildElement();
//		while(obj->NextSiblingElement()){
//
//			if(obj->GetText()){
//
//				std::cout << " " << obj->Value() << ", text: " << obj->GetText() << std::endl;
//
//			}else{
//				std::cout << " " << obj->Value() << std::endl;
//			}
//			obj = obj->NextSiblingElement();
//
//		}
//		e = e->NextSiblingElement();


//	}
}

string enterXml(){
	string s;
	cout << "Enter xml file to load: ";
	std::getline(cin, s);
	return s;
}

int main(int argc, char ** argv){

//	printXml();
//	string xmlFileName = "sample.txt.xml";
//	string xmlFileName = "Sample/samples/creaturesample.xml";
//	string xmlFileName = "Sample/samples/triggersample.xml";
//	string xmlFileName = "Sample/samples/itemsample.xml";
//	string xmlFileName = "Sample/samples/containersample.xml";
//	string xmlFileName = "Sample/samples/roomsample.xml";
	string xmlFileName = enterXml();
	//	string xmlFileName = enterXml();
	Map * GameMap = initializeObj(xmlFileName);
	GameMap->start();
//	GameMap->runGame();
	delete GameMap;
	return 0;
}
