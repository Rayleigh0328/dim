#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>

#include "hilbert_scanner.h"

using namespace std;

typedef int DATA_TYPE;

const int pow = 4;

int main()
{
	HilbertScanner hs;

	int n = 1 << pow;
	vector<vector<DATA_TYPE>> a;
	for (int i=0;i<n;++i)
	{
		a.push_back(vector<DATA_TYPE>());
		for (int j=0;j<n;++j)
			a[i].push_back(hs.xy2d(n,i,j));
	}
	for (int i=0;i<n;++i)
	{
		for (int j=0;j<n;++j)
			cout << a[i][j] << "\t";
		cout << endl;
	}
	return 0;
}