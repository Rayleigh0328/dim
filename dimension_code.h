#ifndef DIMENSION_CODE_H
#define DIMENSION_CODE_H

// Some macros that control the behavior of encoder
#define PRINT_PARTITION
#define PRINT_RESIDUAL

#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include <vector>
#include <list>

using namespace std;

class DimensionCode{
private:
	typedef long long BIAS_TYPE;
	typedef int LABEL_TYPE;
	typedef pair<BIAS_TYPE, LABEL_TYPE> grammar_element;
public:
	static vector<BIAS_TYPE> encode(const vector<BIAS_TYPE> &raw);
	// decode();
};

#endif