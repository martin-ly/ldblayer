#ifndef BASIC_OPERATIONS_H
#define BASIC_OPERATIONS_H

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "database_abstract.h"

using namespace std;

struct TestPair 
{
	using Expectation = vector<pair<string,string>>;
	
	ldblayer::DatabaseAbstract* database;	
	vector<Expectation> es; // expected key/val for each part
};

class BasicOperations : public ::testing::TestWithParam<TestPair>
{ 	
protected:
	virtual void SetUp();
	virtual void TearDown();
	
	BasicOperations();
	~BasicOperations();
	
private:
	
	
	
	ldblayer::DatabaseAbstract* db;
};


namespace {

	template <size_t n>
	const vector<string> &genCases()
	{
		static vector<string> ys;
		if (!ys.empty()) return ys;
		
		for (auto x : genCases<n-1>())
		{
			for (char c = 'a'; c <= 'f'; ++c)
				ys.push_back(x + c);
		}
		
		return ys;
	}
	
	template<>
	const vector<string> &genCases<0>()
	{
		static const vector<string> ys { string{} };
		return ys;
	}
	
	INSTANTIATE_TEST_CASE_P(Comb0, BasicOperations, ::testing::Values( {  } ));
	
	
	//INSTANTIATE_TEST_CASE_P(Comb0, BasicOperations, ::testing::ValuesIn( genCases<0>() ));
/*	INSTANTIATE_TEST_CASE_P(Comb1, BasicOperations, ::testing::ValuesIn(genCases<1>()));
	INSTANTIATE_TEST_CASE_P(Comb2, BasicOperations, ::testing::ValuesIn(genCases<2>()));
	INSTANTIATE_TEST_CASE_P(Comb3, BasicOperations, ::testing::ValuesIn(genCases<3>()));
	INSTANTIATE_TEST_CASE_P(Comb8, BasicOperations, ::testing::ValuesIn(genCases<5>())); */
}

#endif