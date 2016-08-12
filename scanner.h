#ifndef SCANNER_H
#define SCANNER_H

#include <set>
#include <vector>

#include "position.h"

class Scanner{
public:
	virtual vector<long long> scan(int heihgt, int width, const set<position> &ps) = 0;
};

#endif