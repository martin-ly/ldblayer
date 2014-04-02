#include "direct_db.h"

#include <stdlib.h>

void DirectDatabase::SetUp() 
{
    leveldb::Options options;
    options.create_if_missing = true;

    char path_template[] = "/tmp/filesXXXXXX";
    const char* path = mkdtemp(path_template);    
    ASSERT_TRUE(path);
    
    leveldb::Status status;
    status = db.open(options, path);
    ASSERT_TRUE(status.ok()) << "Can't open database: " << status.ToString();
}

void DirectDatabase::TearDown() 
{
    db.destroy();
}