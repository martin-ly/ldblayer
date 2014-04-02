#ifndef TEST_SIMPLE_LEVELDB_H
#define TEST_SIMPLE_LEVELDB_H

#include <list>
#include <string>
#include <gtest/gtest.h>
#include "LevelDatabase.h"

using std::string;
using std::list;
using std::pair;


#ifndef GTEST_HAS_PARAM_TEST
#error "Gtest haven't support for param tests"
#endif

using keyval = list<pair<string, string>>;

// To use a test fixture, derive a class from testing::Test.
class DirectDatabase : public ::testing::TestWithParam<keyval>
{   
protected:
    LevelDatabase db;
    
    virtual void SetUp();
    virtual void TearDown();
    
    ~DirectDatabase() {}    
};

class RawDatabasaOperations : public DirectDatabase 
{    
protected:
    void simpleUpload();
    void simpleCheckData();
    void simpleCheckData(const keyval& keyval);
};

class TransactionOperations : public RawDatabasaOperations 
{
protected:
    void transactionUpload();
};

class IteratorsOperations : public TransactionOperations 
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