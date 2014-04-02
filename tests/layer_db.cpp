#include "layer_db.h"

#include <stdlib.h>

void LayerDatabase::SetUp() 
{
    leveldb::Options options;
    options.create_if_missing = true;
    
    char path_template[] = "/tmp/filesXXXXXX";
    const char* path = mkdtemp(path_template);    
    ASSERT_TRUE(path);
    
    leveldb::Status status;
    status = dbPhisical.open(options, path);
    ASSERT_TRUE(status.ok()) << "Can't open database: " << status.ToString();  
    
    db1.open(&dbPhisical, "first");
    db2.open(&dbPhisical, "second");
    db3.open(&dbPhisical, "third");
}

void LayerDatabase::TearDown() 
{
    db1.close();
    db2.close();
    db3.close();
    dbPhisical.destroy();
}

LayerDatabase::~LayerDatabase() 
{
}