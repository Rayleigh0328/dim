#include "dimension_code.h"

#include "trie_node.h"

using namespace std;

vector<long long> DimensionCode::encode(const vector<long long> &raw)
{
	cout << "grammar transform starts" << endl;
	TrieNode *root = new TrieNode();

	int cur_index = 0;
	while (cur_index < raw.size())
	{
		long long base = raw[cur_index];
		long long diff = 0;

		TrieNode* tmp = root;
		while (raw[cur_index]-raw[cur_index])
	}
}