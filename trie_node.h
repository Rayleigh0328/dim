#ifndef TRIE_NODE
#define TRIE_NODE

#include <map>

using namespace std;

class TrieNode
{
private:
	static int serial_num;

public:
	map<long long, TrieNode*> child_map;
	int label;

	TrieNode();

	static void reset_label_count();
};
#endif