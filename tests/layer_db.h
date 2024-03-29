#ifndef LAYERDB_H
#define LAYERDB_H

#include <list>
#include <string>
#include <gtest/gtest.h>
#include "database.h"
#include "layer.h"

using std::string;
using std::list;
using std::pair;


#ifndef GTEST_HAS_PARAM_TEST
#error "Gtest haven't support for param tests"
#endif

using keyval = list<pair<string, string>>;
using namespace ldblayer;

// To use a test fixture, derive a class from testing::Test.
class LayerDatabase : public ::testing::TestWithParam<keyval>
{   
protected:
	Layer db1;
	Layer db2;
	Layer db3;
    Database dbPhisical;
    
    virtual void SetUp();
    virtual void TearDown();
    
    ~LayerDatabase();
};

class LayerRaw : public LayerDatabase 
{
protected:
	void simpleUpload(DatabaseAbstract* db, keyval values);
	void simpleCheckData(DatabaseAbstract* db, bool expect = true);
	void simpleCheckData(DatabaseAbstract* db, const keyval& values, bool expect = true);
};

class TestLayerTransaction : public LayerRaw 
{
protected:
	void transactionUpload(std::unique_ptr<TransactionAbstract>& transaction, keyval values);
};

class TestLayerIterator : public TestLayerTransaction 
{
protected:
	
};

namespace test 
{
    static const keyval no_keys = { };
    static const keyval one_key = { { "key_a", "data_a" } };
    static const keyval simple_keys = { {"key_a", "data_a"},{"key_b", "data_b"}, {"key_c", "data_c"} };
    static const keyval duplicate_keys = { {"key_a", "data_a"}, {"key_a", "key_b"}, {"key_b", "data_b"}, {"key_c", "data_c"} };
	
	static const keyval begin_keys = { {"a_key_a", "a_data_a"}, {"a_key_b", "a_data_b"}, {"a_key_c", "a_data_c"}, {"a_key_d", "a_data_d"} };
	static const keyval midle_keys = { {"b_key_a", "b_data_a"}, {"b_key_b", "b_data_b"}, {"b_key_c", "b_data_c"}, {"b_key_d", "b_data_d"} };
	static const keyval end_keys = { {"c_key_a", "c_data_a"}, {"c_key_b", "c_data_b"}, {"c_key_c", "c_data_c"}, {"c_key_d", "c_data_d"} };
}

#endif