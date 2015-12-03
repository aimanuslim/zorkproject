/*
 * Object.h
 *
 *  Created on: Dec 2, 2015
 *      Author: amduslim
 */

#ifndef OBJECT_H_
#define OBJECT_H_

class Object {
public:
	Object();
	string name;
	int status;
	string type;
	vector <Trigger> triggers;
	virtual ~Object();
};

#endif /* OBJECT_H_ */
