#ifndef TRIE_NODE
#define TRIE_NODE

#include <map>

using namespace std;

struct TrieNode
{
	long long bias;
	map<long long, TrieNode*> child_map;
	static int var_num;

	TrieNode()
	{
		bias = -1;
		++var_num;
		child_map.clear();
	}

	TrieNode(long long b)
	{
		TrieNode();
		bias = b;
	}
};
#endif