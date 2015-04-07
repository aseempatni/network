#ifndef _FINGER_TABLE_H_
#define _FINGER_TABLE_H_

#include "node.h"
#include <math.h> 

class finger_table {
	location finger[MBIT];
	location operator[] (int i);
	int update(location loc);
};

#endif /* _FINGER_TABLE_H_ */