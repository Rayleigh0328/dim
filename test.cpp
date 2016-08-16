#include <iostream>
#include <cstdio>
#include <vector> 
#include <cassert>

#include "dimension_code.h"

using namespace std;

// #define TEST_CASE_1
#define TEST_CASE_2

void add_set(long long bias, const vector<long long> &s, vector<long long> &result)
{
	for (auto e : s) result.push_back(e+bias);
}

int main()
{
	#ifdef TEST_CASE_1
		const int a_size = 12;
		int a[a_size] = {3,5,7,10,13,15,17,20,24,27,31,33};
		vector<long long> seq;
		seq.clear();
		for (int i=0;i<a_size;++i)
			seq.push_back((long long)a[i]);
	#endif

	#ifdef TEST_CASE_2
		vector<long long > a1 = {0,5,7,9,11,12,15,18,23,27};
		vector<long long> a4 = {0,5,7,9,11,12,15,18,23,27,30};
		vector<long long> a5 = {0,6,7,12,14,16,18,19,22,25,30,34,40,45,49};

		vector<long long> seq;
		seq.clear();

		seq.push_back(3);
		add_set(5, 		a5, 	seq);
		add_set(59,		a1, 	seq);
		add_set(89,		a5, 	seq);
		add_set(149, 	a4, 	seq);
		add_set(189,	a4, 	seq);
		seq.push_back(239);

		assert(seq.size() == 64);
		//cout << "seq size: " << seq.size() << endl;
		sort(seq.begin(), seq.end());
		for (int i=0;i<seq.size()-1;++i)
			assert(seq[i]!=seq[i+1]);
	#endif

	// seq --> ret --> recon
	// seq == recon
	vector<long long> ret = DimensionCode::encode(seq);
	vector<long long> recon = DimensionCode::decode(ret);

	bool flag_pass = true;
	for (int i=0;i<seq.size();++i)
		if (seq[i] != recon[i])
		{
			flag_pass = false;
			break;
		}

	assert(flag_pass);
	cout << endl << "CORRECT" << endl << endl;
	return 0;
}