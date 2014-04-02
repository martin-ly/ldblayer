#include "layer_db.h"

using namespace test;

INSTANTIATE_TEST_CASE_P(simpledata, LayerRaw, ::testing::Values(no_keys, one_key, simple_keys));

// simple database operations
void LayerRaw::simpleUpload(LevelDatabaseAbstract* db, keyval values) 
{    
    for (auto i = values.rbegin(); i != values.rend(); i++) {
        EXPECT_TRUE(db->Put(i->first, i->second).ok());    
    }
}

void LayerRaw::simpleCheckData(LevelDatabaseAbstract* db, bool expect)
{
    keyval values = GetParam();
    
    simpleCheckData(db, values, expect);
}

void LayerRaw::simpleCheckData(LevelDatabaseAbstract* db, const keyval& values, bool expect) 
{
    for (auto row: values) {
        string value;
		
		if (expect) {
			EXPECT_TRUE(db->Get(row.first, &value).ok());        
			EXPECT_EQ(row.second, value) << "Not found in database value: " << row.first << " - actual found: " << value;
		} else {
			EXPECT_FALSE(db->Get(row.first, &value).ok());
		}
    }
}


TEST_P(LayerRaw, put) 
{   
    keyval values = GetParam();
    simpleUpload(&db1, values);
}

TEST_P(LayerRaw, get) 
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


TEST_P(LayerRaw, del) 
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
