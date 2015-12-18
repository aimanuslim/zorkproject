/*
 * Map.h
 *
 *  Created on: Dec 10, 2015
 *      Author: amduslim
 */

#ifndef MAP_H_
#define MAP_H_

#include <map>
#include <string>

#include "Container.h"
#include "Room.h"
#include "Zork.h"

using std::map;
using std::string;

struct RoomNode;



class Map {
public:
	Map();
	map<string, Command> commandMapping;
	map<Command, string> reverseCM;
	map<string, RoomNode*> roomM; // will contain all rooms and their names
	RoomNode * currRoom; // the current room the player is in
	Container * inventory; // the player's inventory
	map<string, Object*> removedObj;
	map<string, Object*> allItemandContainers; // contain items and containers that may not be added to the game yet
	bool victory;
	void start();
	InputFormat parseInput();
	bool validateInput(InputFormat);
	void showInventory();
	void moveRoom(Command);
	Object * findItemContainer(string); // contains creature too
	void applyTrigger(Object *, Triggers *);
	void Add(Object *, Object *);
	void Update(Object *, string);
	void Delete(Object *);
	void processInput(InputFormat);
	InputFormat parseInput(string);
	void runGame();
	void parseActionandApply(string update_message);
	bool checkCreatureContainerTrigger(InputFormat);
	bool checkRoomTrigger(InputFormat);
	void removeObjInAllRoom(string);
	virtual ~Map();
};

#endif /* MAP_H_ */
