#include "dimension_code.h"

#include "trie_node.h"

using namespace std;

vector<DimensionCode::BIAS_TYPE> DimensionCode::encode(const vector<BIAS_TYPE> &raw)
{
	cout << "grammar transform starts" << endl;

	// create a list to store data
	// to better support pop_front()

	list<BIAS_TYPE> a;
	a.clear();
	for (auto e : raw)
		a.push_back(e);

	// Create trie
	TrieNode::reset_label_count();
	TrieNode *root = new TrieNode();

	vector<vector<DimensionCode::grammar_element>> rules;
	rules.clear();
	rules.push_back(vector<DimensionCode::grammar_element>());

	// deal with first element

	#ifdef PRINT_PARTITION
		stringstream par_ss;
		par_ss << "Partition of vector: " << endl;
		par_ss << a.front() << endl;
	#endif

	rules[0].push_back(grammar_element(a.front(), 0));
	a.pop_front();

	while (!a.empty())
	{
		#ifdef PRINT_PARTITION
			par_ss << a.front() << " ";
		#endif 

		TrieNode* cur = root;
		BIAS_TYPE prev = a.front();
		a.pop_front();

		grammar_element result;
		result.first = prev;

		// search through the trie
		BIAS_TYPE diff;
		while (!a.empty())
		{
			#ifdef PRINT_PARTITION
				par_ss << a.front() << " " ;
			#endif
			diff = a.front() - prev;
			prev = a.front();
			a.pop_front();

			if (cur->child_map.find(diff) == cur->child_map.end())
			{
				cur->child_map[diff] = new TrieNode();
				// create new rules
				int num = rules.size();
				rules.push_back(vector<grammar_element>());
				rules[num].push_back(grammar_element(0, cur->label));
				rules[num].push_back(grammar_element(diff, 0));
				cur = cur->child_map[diff];
				break;
			}
			else
			{
				cur = cur->child_map[diff];
			}
		}

		result.second = cur->label;
		rules[0].push_back(result);
		#ifdef PRINT_PARTITION
			par_ss << endl;
		#endif
	}

	#ifdef PRINT_PARTITION
		cout << endl << par_ss.str() << endl;
	#endif

	#ifdef PRINT_RAW_GRAMMAR
		stringstream res_ss;
		for (int i=0; i<rules.size();++i)
		{
			res_ss << "V_" << i << ": "; 
			for (int j=0;j<rules[i].size();++j)
				res_ss << "(" << rules[i][j].first << "," << rules[i][j].second  << ") ";
			res_ss << endl;
		}
		cout << "Raw grammar after GT: " << endl;
		cout << res_ss.str() << endl;
	#endif
	// int cur_index = 0;
	// while (cur_index < raw.size())
	// {
	// 	long long base = raw[cur_index];
	// 	long long diff = 0;

	// 	TrieNode* tmp = root;
	// 	while (raw[cur_index]-raw[cur_index])
	// }
}