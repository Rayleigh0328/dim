#include "trie_node.h"

using namespace std;

// serial_num is a counter to 
// count how many objs have been created
int TrieNode::serial_num = 0;

TrieNode::TrieNode()
{
	label = serial_num++;
	child_map.clear();
}

// reset serial num
void TrieNode::reset_label_count()
{
	serial_num = 0;
}