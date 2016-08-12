#include <fstream>
#include <set>

#include "position.h"

using namespace std;

class InputProcessor{
public:
	static int process(char* filename, int &height, int &width, int &cnt, set<position> &ps)
	{
		// process input mtx file accoding to the filename 
		// return height in height, width in width
		// return number of non-zero in cnt
		// return the set of positions in ps

		// when fail to open the file, return value is set to -1

		ifstream fin;
		fin.open(filename);

		// check filename
		if (!fin.good())
		{
			return -1;
		}

		// skip comment
		string st;
		while (fin.peek() == '%') getline(fin, st);

		//  read height, size, # of NZ
		fin >> height >> width >> cnt;

		ps.clear();
		for (int i=0;i<cnt;++i)
		{
			int row, col;
			fin >> row >> col;
			ps.insert(position(row-1, col-1));
		}
		return 0;
	}

};