/*
 * Zork.h
 *
 *  Created on: Dec 2, 2015
 *      Author: amduslim
 */

#ifndef ZORK_H_
#define ZORK_H_
#include <string>
using std::string;



typedef struct {
	string name;
	string direction;
} Border;

enum Command{
	n, s, e, w,
	i,
	take,
	open,
	open_exit,
	read_act,
	drop,
	put,
	turn_on,
	attack,
	no_command
};

typedef struct {
	Command com;
	string obj1;
	string obj2;
} InputFormat;


#endif /* ZORK_H_ */
