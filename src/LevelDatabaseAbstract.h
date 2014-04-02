#ifndef LEVELDATABASEABSTRACT_H
#define LEVELDATABASEABSTRACT_H

#include <leveldb/status.h>

class LevelDatabaseAbstract 
{
public:
    virtual leveldb::Status Put(const std::string& key, const std::string& value) = 0;
    virtual leveldb::Status Get(const std::string& key, std::string* value) = 0;
    virtual leveldb::Status Del(const std::string& key) = 0;
};

#endif