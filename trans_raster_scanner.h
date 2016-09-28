#ifndef TRANS_RASTER_SCANNER
#define TRANS_RASTER_SCANNER

#include <set>
#include <vector>

#include "scanner.h"

class TransRasterScanner: public Scanner
{
public:
	vector<long long> scan(int height, int width, const set<position>& ps);
};

#endif