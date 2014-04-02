#include "layer_db.h"

using namespace test;

INSTANTIATE_TEST_CASE_P(LayerDatabase, LayerDatabase, ::testing::Values(no_keys, one_key, simple_keys));

// simple database operations
void LayerDatabase::simpleUpload(LevelDatabaseAbstract* db, keyval values) 
{    
    for (auto i = values.rbegin(); i != values.rend(); i++) {
        EXPECT_TRUE(db->Put(i->first, i->second).ok());    
    }
}

void LayerDatabase::simpleCheckData(LevelDatabaseAbstract* db)
{
    keyval values = GetParam();
    
    simpleCheckData(db, values);
}

void LayerDatabase::simpleCheckData(LevelDatabaseAbstract* db, const keyval& values) 
{
    for (auto row: values) {
        string value;
        EXPECT_TRUE(db->Get(row.first, &value).ok());
        
        EXPECT_EQ(row.second, value) << "Not found in database value: " << row.first << " - actual found: " << value;
    }
}


TEST_P(LayerDatabase, put) 
{   
    keyval values = GetParam();
    simpleUpload(&db1, values);
}

TEST_P(LayerDatabase, get) 
{
    keyval values = GetParam();
    
    simpleUpload(&db2, values);
    simpleCheckData(&db2);   
    
    simpleUpload(&db1, values);
    simpleCheckData(&db1);
    simpleCheckData(&db2);
    
    if (values.empty())
        return;
    
    string key = values.begin()->first;
    string value;
    
    // 3 database should not contain any raw
    EXPECT_FALSE(db3.Get(key, &value).ok());
    
    // update 2 database first key value
    EXPECT_TRUE(db2.Put(key, "blah").ok());
    
    EXPECT_TRUE(db1.Get(key, &value).ok());
    EXPECT_EQ(values.begin()->second, value);
    // get key, that doesn't exists
    
    EXPECT_TRUE(db2.Get(key, &value).ok());
    EXPECT_EQ("blah", value);
}


TEST_P(LayerDatabase, del) 
{    
    keyval values = GetParam();
    keyval processed = values;
    
    simpleUpload(&db1, values);
    simpleUpload(&db2, values);
    
    for (auto row: values) {    
        EXPECT_TRUE(db2.Del(row.first).ok());        
        processed.pop_front();
        simpleCheckData(&db2, processed);
    }
}
