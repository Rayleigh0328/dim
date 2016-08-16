#ifndef HILBERT_SCANNER
#define HILBERT_SCANNER

#include <set>
#include <vector>

#include "scanner.h"

class HilbertScanner: public Scanner
{
public:
	vector<long long> scan(int height, int width, const set<position>& ps);
	int padding(int height, int width);
	long long xy2d (int n, int x, int y);
	void d2xy(int n, long long d, int *x, int *y);
	void rot(int n, int *x, int *y, int rx, int ry);
};

#endif