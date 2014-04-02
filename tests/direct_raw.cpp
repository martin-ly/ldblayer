#include "direct_db.h"

using namespace test;

INSTANTIATE_TEST_CASE_P(DirectDatabase, RawDatabasaOperations, ::testing::Values(no_keys, one_key, simple_keys));

// simple database operations
void RawDatabasaOperations::simpleUpload() 
{
    keyval values = GetParam();   
    
    for (auto i = values.rbegin(); i != values.rend(); i++) {
        EXPECT_TRUE(db.Put(i->first, i->second).ok());    
    }
}

void RawDatabasaOperations::simpleCheckData()
{
    keyval values = GetParam();
    
    simpleCheckData(values);
}

void RawDatabasaOperations::simpleCheckData(const keyval& values) 
{
    for (auto row: values) {
        string value;
        EXPECT_TRUE(db.Get(row.first, &value).ok());
        
        EXPECT_EQ(row.second, value) << "Not found in database value: " << row.first << " - actual found: " << value;
    }
}


TEST_P(RawDatabasaOperations, put) 
{
    simpleUpload();
}

TEST_P(RawDatabasaOperations, get) 
{
    simpleUpload();
    simpleCheckData();
    
    // get key, that doesn't exists
    string value;
    EXPECT_FALSE(db.Get("bazinga____ahahaha!!!", &value).ok());
}


TEST_P(RawDatabasaOperations, del) 
{
    simpleUpload();
    
    keyval values = GetParam();
    keyval processed = values;
    
    for (auto row: values) {        
        EXPECT_TRUE(db.Del(row.first).ok());        
        processed.pop_front();
        simpleCheckData(processed);
    }
}
