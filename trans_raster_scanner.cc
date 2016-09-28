#include "trans_raster_scanner.h"

vector<long long> TransRasterScanner::scan(int height, int width, const set<position>& ps)
{
	vector<long long> ret;
	ret.clear();
	// 
	for (auto p : ps)
	{
		ret.push_back((long long) p.col * height + p.row);
	}
	sort(ret.begin(), ret.end());
	return ret;
}

