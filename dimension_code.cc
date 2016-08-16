#include "dimension_code.h"

#include "trie_node.h"

using namespace std;

vector<DimensionCode::BIAS_TYPE> DimensionCode::encode(const vector<BIAS_TYPE> &raw)
{
	/************ Lempel-Ziv Grammar Transform ********/

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

	#ifdef PRINT_PARTITION
		stringstream par_ss;
		par_ss << "Partition of vector: " << endl;
		par_ss << a.front() << endl;
	#endif

	// deal with first element
	rules[0].push_back(grammar_element(a.front(), 0));
	a.pop_front();

	while (!a.empty())
	{
		#ifdef PRINT_PARTITION
			par_ss << a.front() << " ";
		#endif 

		TrieNode* cur = root;
		BIAS_TYPE prev = a.front();
		BIAS_TYPE abs_pos;
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
			abs_pos = a.front();
			diff = abs_pos - prev;

			prev = a.front();
			a.pop_front();

			if (cur->child_map.find(diff) == cur->child_map.end())
			{
				cur->child_map[diff] = new TrieNode();
				// create new rules
				int num = rules.size();
				rules.push_back(vector<grammar_element>());
				rules[num].push_back(grammar_element(0, cur->label));
				rules[num].push_back(grammar_element(abs_pos - result.first, 0));
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
	/************ Lempel-Ziv Grammar Transform ********/

	/************* Grammar Simplification *************/

	map<LABEL_TYPE, int> var_count;
	var_count.clear();
	// make sure label 0 is never marked useless
	var_count[0] = 2;

	for (int i=0;i<rules.size();++i)
		for (int j=0;j<rules[i].size();++j)
		{
			LABEL_TYPE cur_lab = rules[i][j].second;
			if (var_count.find(cur_lab) == var_count.end())
			{
				var_count.emplace(cur_lab, 1);
			}
			else
			{
				++var_count[cur_lab];
			}
		}

	set<LABEL_TYPE> useless_var_set;
	useless_var_set.clear();
	for (auto &e : var_count)
		if (e.second == 1) useless_var_set.insert(e.first);	

	#ifdef PRINT_USELESS_VAR
		stringstream useless_var_ss;
		useless_var_ss << "Useless vars: " << endl;
		for (auto &e : useless_var_set)
			useless_var_ss << e << " ";
		useless_var_ss << endl;
		cout << useless_var_ss.str() << endl;
	#endif

	//for (int i=rules.size()-1; i>=0; --i)
	for (int i=0; i<rules.size(); ++i)
	{
		//if (useless_var_set.find(i) != useless_var_set.end()) continue;
		for (int j=rules[i].size()-1; j>=0; --j)
		{
			BIAS_TYPE cur_bias = rules[i][j].first;
			LABEL_TYPE cur_lab = rules[i][j].second;
			if (useless_var_set.find(cur_lab) != useless_var_set.end())
			{
				// insert var[cur_lab] to var[i] at cur_bias;
				for (int k=0; k < rules[cur_lab].size(); ++k)
					rules[i].push_back(grammar_element(cur_bias + rules[cur_lab][k].first , rules[cur_lab][k].second));
			}
		}
	}

	vector<vector<DimensionCode::grammar_element>> simplified_rules;	
	simplified_rules.clear();

	// substitute variables that appears only once
	// back substitution is done according to topological order
	for (int i=0;i<rules.size();++i)
	{
		simplified_rules.push_back(vector<DimensionCode::grammar_element>());
		if (useless_var_set.find(i) != useless_var_set.end()) continue;

		for (int j=0;j<rules[i].size();++j)
			if (useless_var_set.find(rules[i][j].second) == useless_var_set.end())
				simplified_rules[i].push_back(rules[i][j]);
	}

	// reordering the element in each production rules gives final simplified grammar
	for (int i=0;i<simplified_rules.size();++i)
		sort(simplified_rules[i].begin(), simplified_rules[i].end(), DimensionCode::cmp_grammar_element_bias);

	#ifdef PRINT_SIMPLIFIED_GRAMMAR
		stringstream simp_ss;
		for (int i=0; i<simplified_rules.size();++i)
		{
			simp_ss << "V_" << i << ": "; 
			for (int j=0;j<simplified_rules[i].size();++j)
				simp_ss << "(" << simplified_rules[i][j].first << "," << simplified_rules[i][j].second  << ") ";
			simp_ss << endl;
		}
		cout << "Grammar after simiplification: " << endl;
		cout << simp_ss.str() << endl;
	#endif
	/************* Grammar Simplification *************/

	/************* Final Dimension Vector *************/

	// relabel the variabels into 3 categories
		// 0: 0
		// even: size 2 and second one is terminal labeled
		// odd: otherwise.
	map<LABEL_TYPE, LABEL_TYPE> label_map;
	label_map.clear();
	label_map.emplace(0,0);

	int odd_count = 1;
	int even_count = 2;
	for (int i=1;i<simplified_rules.size();++i)
	{
		if (simplified_rules[i].empty()) continue;
		if (simplified_rules[i].size() == 2 && simplified_rules[i][1].second == 0)
		{
			label_map.emplace(i, even_count);
			even_count += 2;
		}
		else
		{
			label_map.emplace(i, odd_count);
			odd_count +=2;
		} 
	}

	#ifdef PRINT_RELABELED_GRAMMAR
		stringstream reord_ss;
		for (int i=0; i<simplified_rules.size();++i)
		{
			if (label_map.find(i) == label_map.end()) continue;
			reord_ss << "V_" << label_map[i] << ": "; 
			for (int j=0;j<simplified_rules[i].size();++j)
				reord_ss << "(" << simplified_rules[i][j].first << "," << label_map[simplified_rules[i][j].second]  << ") ";
			reord_ss << endl;
		}
		cout << "Grammar after relabeling: " << endl;
		cout << reord_ss.str() << endl;
	#endif

	int cnt_0_var = 0;
	int cnt_0_term = 0;
	for (int i=0;i<simplified_rules[0].size();++i)
		if (simplified_rules[0][i].second == 0) ++cnt_0_term;
		else ++cnt_0_var;

	// as Lempel-Ziv transform is applied, there will be no odd-number labeled element
	vector<DimensionCode::BIAS_TYPE> ret;
	ret.clear();
	// 2 * # of variable labeled elements in v0
	ret.push_back(2 * cnt_0_var);
	// add # of terminal labeled elements in v0
	ret.push_back(ret[ret.size()-1] + cnt_0_term);
	// add 2 * # of even-number labeled elements
	ret.push_back(ret[ret.size()-1] + even_count - 2);

	// add var labled element in v0
	for (int i=0;i<simplified_rules[0].size();++i)
		if (simplified_rules[0][i].second != 0)
		{
			ret.push_back(simplified_rules[0][i].first);
			ret.push_back(label_map[simplified_rules[0][i].second]);
		}

	for (int i=0;i<simplified_rules[0].size();++i)
		if (simplified_rules[0][i].second == 0)
		{
			ret.push_back(simplified_rules[0][i].first);
		}

	// add even-number labeled variable
	for (int i=1;i<simplified_rules.size();++i)
		if (label_map.find(i) != label_map.end())
		{
			ret.push_back(label_map[simplified_rules[i][0].second]);
			ret.push_back(simplified_rules[i][1].first);
		}

	#ifdef PRINT_DIM_VECTOR_LENGTH
		stringstream dim_vect_len_ss;
		dim_vect_len_ss << "Dimension Vector Length: " << ret.size() << endl;
		cout << dim_vect_len_ss.str() << endl;
	#endif

	#ifdef PRINT_DIM_VECTOR
		stringstream dim_vect_ss;
		dim_vect_ss << "Dimension Vector Content: " << endl;
		for (auto e : ret) dim_vect_ss << e << " ";
		dim_vect_ss << endl;
		cout << dim_vect_ss.str() << endl;
	#endif

	return ret;
	/************* Final Dimension Vector *************/
}







bool DimensionCode::cmp_grammar_element_bias(const DimensionCode::grammar_element &a, const DimensionCode::grammar_element &b)
{
	return a.first < b.first;
}









vector<DimensionCode::BIAS_TYPE> DimensionCode::decode(const vector<BIAS_TYPE> &codeword)
{
	int p = 0;
	int end_0_var = codeword[p++] + 3;
	int end_0_term = codeword[p++] + 3;
	int end_even = codeword[p++] + 3;

	vector<vector<grammar_element>> rules;

	// construct v0
	rules.push_back(vector<grammar_element>());
	for (; p<end_0_var;p+=2)
		rules[0].push_back(grammar_element(codeword[p], codeword[p+1]));
	for (; p<end_0_term;++p)
		rules[0].push_back(grammar_element(codeword[p],0));

	// construct even var
	int cnt_even = 0;
	for (; p < end_even; p+=2)
	{
		rules.push_back(vector<grammar_element>());
		++cnt_even;
		rules[cnt_even].push_back(grammar_element(0,codeword[p]));
		rules[cnt_even].push_back(grammar_element(codeword[p+1],0));
	}

	#ifdef PRINT_RECONSTRUCTED_GRAMMAR
		stringstream res_ss;
		res_ss << end_0_var << " " << end_0_term << " " << end_even << endl;
		for (int i=0; i<rules.size();++i)
		{
			res_ss << "V_" << i << ": "; 
			for (int j=0;j<rules[i].size();++j)
				res_ss << "(" << rules[i][j].first << "," << rules[i][j].second  << ") ";
			res_ss << endl;
		}
		cout << "Reconstructed grammar: " << endl;
		cout << res_ss.str() << endl;
	#endif


	vector<BIAS_TYPE> ret;
	ret.clear();

	vector<grammar_element> vect1(rules[0]);
	vector<grammar_element> vect2;

	int iteration_num = 0;
	while (true)
	{
		// cout << "#it: " << iteration_num << endl;
		if (iteration_num & 1) 
		{
			parallel_replace(vect2, vect1, rules);
			if (check_all_term(vect1)) 
			{
				//ret = vect1;
				for (int i=0;i<vect1.size();++i) ret.push_back(vect1[i].first);
				break;
			}
		}
		else
		{
			parallel_replace(vect1, vect2, rules);
			if (check_all_term(vect2)) 
			{
				//ret = vect2;
				for (int i=0;i<vect2.size();++i) ret.push_back(vect2[i].first);
				break;
			}
		}
		++iteration_num;		
	}

	sort(ret.begin(), ret.end());
	return ret;
}


void DimensionCode::parallel_replace(const vector<grammar_element> &a, vector<grammar_element> &b, const vector<vector<grammar_element>> &rules)
{
	b.clear();
	for (auto &e : a)
		if (e.second == 0) b.push_back(e);
		else
		{
			LABEL_TYPE actual_lab = e.second / 2;
			BIAS_TYPE cur_bias = e.first;
			for (auto &e2 : rules[actual_lab])
				b.push_back(grammar_element(e2.first + cur_bias, e2.second));
		}
}


bool DimensionCode::check_all_term(const vector<grammar_element> &a)
{
	for (auto &e : a)
		if (e.second != 0) return false;
	return true;
}























