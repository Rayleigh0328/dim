#include <iostream>
#include <cstdio>
#include <fstream>
#include <set>

#include "input_processor.cc"
#include "position.h"
#include "scanner.h"
#include "raster_scanner.h"
#include "hilbert_scanner.h"
#include "dimension_code.h"

#define USE_HILBERT_SCAN
// #define USE_RASTER_SCAN

using namespace std;

int main(int argc, char **argv)
{
	// the only argument passed to main is the input file name
	cout << "Input matrix name: " << argv[1] << endl;

	// process the input to get a set of points
	int height, width, tot_nz;
	set<position> ps;
	int ret = InputProcessor::process(argv[1], height, width, tot_nz, ps);

	if (ret < 0)
		cerr << "Error reading input file: " << argv[1] << "!" << endl;

	cout << "height: " << height << endl;
	cout << "width: " << width << endl;
	cout << "# of NZ: " << tot_nz << endl;
 	
 	#ifdef USE_RASTER_SCAN
	 	RasterScanner s;
	 	vector<long long> serialized = s.scan(height, width, ps);
	 	cout << "Raster Serialization finished" << endl;
 	#endif
 	
 	#ifdef USE_HILBERT_SCAN
	 	HilbertScanner s;
	 	vector<long long> serialized = s.scan(height, width, ps);
	 	cout << "Hilbert Serialization finished" << endl;
 	#endif
	
 	// Following is just test code, check the number of positions in vector
 	// is the same as in set. And the vector is increasing.
 	
 	// cout << "vector size: " << serialized.size() << endl;
 	// bool flag = true;
 	// for (int i=1;i<serialized.size();++i)
 	// 	if (serialized[i] <= serialized[i-1])
 	// 	{
 	// 		flag = false;
 	// 		cout << serialized[i-1] << " " << serialized[i] << endl;
 	// 		break;
 	// 	}
 	// cout << flag << endl;

 	vector<long long> codeword = DimensionCode::encode(serialized);
 	vector<long long> recon = DimensionCode::decode(codeword);

 	bool flag_pass = (recon.size() == serialized.size());
	for (int i=0;i<recon.size();++i)
		if (serialized[i] != recon[i])
		{
			flag_pass = false;
			break;
		}

	if (flag_pass)
	{
		cout << "CORRECT" << endl;
		cout << "Dim vector length: " << codeword.size() << endl;
		return 0;
	}
	else
	{
		cout << "WRONG!!!" << endl;
		return -1;
	}
}