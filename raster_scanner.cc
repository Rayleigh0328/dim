#include "raster_scanner.h"

vector<long long> RasterScanner::scan(int height, int width, const set<position>& ps)
{
	vector<long long> ret;
	ret.clear();
	// 
	for (auto p : ps)
	{
		ret.push_back((long long) p.row * width + p.col);
	}
	return ret;
}

