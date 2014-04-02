#ifndef LAYERDB_H
#define LAYERDB_H

#include <list>
#include <string>
#include <gtest/gtest.h>
#include "LevelDatabase.h"
#include "LevelDatabaseLayer.h"

using std::string;
using std::list;
using std::pair;


#ifndef GTEST_HAS_PARAM_TEST
#error "Gtest haven't support for param tests"
#endif

using keyval = list<pair<string, string>>;

// To use a test fixture, derive a class from testing::Test.
class LayerDatabase : public ::testing::TestWithParam<keyval>
{   
protected:
    LevelDatabaseLayer db1;
    LevelDatabaseLayer db2;
    LevelDatabaseLayer db3;
    LevelDatabase dbPhisical;
    
    virtual void SetUp();
    virtual void TearDown();
    
    ~LayerDatabase();
};

class LayerRaw : public LayerDatabase 
{
protected:
	void simpleUpload(LevelDatabaseAbstract* db, keyval values);
	void simpleCheckData(LevelDatabaseAbstract* db);
	void simpleCheckData(LevelDatabaseAbstract* db, const keyval& values);
};

class LayerTransaction : public LayerDatabase 
{
};

namespace test 
{
    static const keyval no_keys = { };
    static const keyval one_key = { { "key_a", "data_a" } };
    static const keyval simple_keys = { {"key_a", "data_a"},{"key_b", "key_b"}, {"key_c", "data_c"} };
    static const keyval duplicate_keys = { {"key_a", "data_a"}, {"key_a", "key_b"}, {"key_b", "key_b"}, {"key_c", "data_c"} };
}

#endif