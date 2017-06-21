#ifndef FOOD_H
#define FOOD_H

#include "Snake.h"

class Food{
private:
	Point p;
public:
	Food() { Random(p.x, p.y); }
};

#endif