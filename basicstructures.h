#ifndef TOADS_BATTLE_BASIC_STRUCTURES_H
#define TOADS_BATTLE_BASIC_STRUCTURES_H

class point
{
public:
	int x, y;
	point() {
		x = y = 0;
	}
	point(int a, int b) {
		x = a;
		y = b;
	}
    ~point(){};
};


class step
{
public:
	point beginPoint, endPoint;

	step() {
		beginPoint = endPoint = point();
	}
	
	step(point A, point B) {
		beginPoint = A;
		endPoint = B;
	}
	
	step(int xa, int ya, int xb, int yb) {
		beginPoint = point(xa, ya);
		endPoint = point(yb, yb);
	}

    ~step(){};
	
};

#endif
