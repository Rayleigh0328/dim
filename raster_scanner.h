#include <set>
#include <vector>

#include "scanner.h"

class RasterScanner: public Scanner
{
public:
	vector<long long> scan(int height, int width, const set<position>& ps);
};