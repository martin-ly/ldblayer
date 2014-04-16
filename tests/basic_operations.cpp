#include "basic_operations.h"

BasicOperations::BasicOperations() 
{
}

BasicOperations::~BasicOperations()
{
}

void BasicOperations::SetUp()
{
	string k = "a";
	string v = "0";
	// fill database and expected view
	
	for (char c : GetParam())
	{
		size_t i;
		string name = "a";
		switch (c)
		{
			case 'a' ... 'z':
				i = c - 'a';
				if (i >= es.size()) es.resize(i+1);
				es[i].emplace_back(k, v);
				name[0] = c;
				//sdb.use(name).Put(k, v);
				
				db->Put(k, v);
				
				cout << k << v << endl;
				
				break;
		}
		
		++k[0]; ++v[0];
	}
}


void BasicOperations::TearDown()
{
}

TEST_P(BasicOperations, test)
{
}


