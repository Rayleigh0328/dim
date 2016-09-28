#ifndef DIMENSION_CODE_H
#define DIMENSION_CODE_H

// Some macros that control the behavior of encoder
// #define PRINT_PARTITION
// #define PRINT_RAW_GRAMMAR
// #define PRINT_USELESS_VAR
// #define PRINT_SIMPLIFIED_GRAMMAR
 // #define PRINT_RELABELED_GRAMMAR
// #define PRINT_DIM_VECTOR_LENGTH
// #define PRINT_DIM_VECTOR

// #define PRINT_RECONSTRUCTED_GRAMMAR


#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include <vector>
#include <list>
#include <map>
#include <set>

using namespace std;

class DimensionCode{
private:
	typedef long long BIAS_TYPE;
	typedef int LABEL_TYPE;
	typedef pair<BIAS_TYPE, LABEL_TYPE> grammar_element;

	static bool cmp_grammar_element_bias(const grammar_element &a, const grammar_element &b);
	static void parallel_replace(const vector<grammar_element> &a, vector<grammar_element> &b, const vector<vector<grammar_element>> &rules);
	static bool check_all_term(const vector<grammar_element> &a);

public:
	static vector<BIAS_TYPE> encode(const vector<BIAS_TYPE> &raw);
	static vector<BIAS_TYPE> decode(const vector<BIAS_TYPE> &raw);
};

#endif