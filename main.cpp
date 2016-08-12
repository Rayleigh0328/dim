#include <iostream>
#include <cstdio>
#include <fstream>
#include <set>

#include "input_processor.cc"
#include "position.h"
#include "scanner.h"
#include "raster_scanner.h"
#include "dimension_code.h"

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
 	
 	RasterScanner rs;
 	vector<long long> serialized = rs.scan(height, width, ps);
 
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
}