#include "hilbert_scanner.h"

int HilbertScanner::padding(int height, int width)
{
	// padding, so that height == width == 2^n
	int tmp = max(height, width);
	int new_size = 1;
	while (new_size < tmp)
	{
		new_size = new_size << 1;
	}
	return new_size;
}


vector<long long> HilbertScanner::scan(int height, int width, const set<position>& ps)
{
	// Here height, width should be padded 
	// so that height == width == 2^n

	int n = padding(height, width);
	vector<long long> ret;
	ret.clear();
	for (auto &e : ps)
		ret.push_back(xy2d(n, e.row, e.col));
	sort(ret.begin(), ret.end());
	return ret;
}


//convert (x,y) to d
long long HilbertScanner::xy2d (int n, int x, int y) 
{
    int rx, ry, s;
    long long d=0;
    for (s=n/2; s>0; s/=2) 
    {
        rx = (x & s) > 0;
        ry = (y & s) > 0;
        d += (long long) s * s * ((3 * rx) ^ ry);
        rot(s, &x, &y, rx, ry);
    }
    return d;
}

//convert d to (x,y)
void HilbertScanner::d2xy(int n, long long d, int *x, int *y) 
{
    int rx, ry, s;
    long long t=d;
    *x = *y = 0;
    for (s=1; s<n; s*=2) 
    {
        rx = 1 & (t/2);
        ry = 1 & (t ^ rx);
        rot(s, x, y, rx, ry);
        *x += s * rx;
        *y += s * ry;
        t /= 4;
    }
}

//rotate/flip a quadrant appropriately
void HilbertScanner::rot(int n, int *x, int *y, int rx, int ry) {
    if (ry == 0) 
    {
        if (rx == 1) 
        {
            *x = n-1 - *x;
            *y = n-1 - *y;
        }

        //Swap x and y
        int t  = *x;
        *x = *y;
        *y = t;
    }
}