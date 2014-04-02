#ifndef LEVELDATABASETRANSACTION_H
#define LEVELDATABASETRANSACTION_H

#include <string>
#include <leveldb/write_batch.h>

class LevelDatabase;
class LevelDatabaseLayer;

class LevelDatabaseTransaction 
{
public:
    LevelDatabaseTransaction(LevelDatabase* database, LevelDatabaseLayer* layer = nullptr);    
    void setActiveLayer(LevelDatabaseLayer* layer);
    
    void Put(const std::string& key, const std::string& value);
    void Delete(const std::string& key);
    
    leveldb::Status commit();
private:
    LevelDatabase* db;
    LevelDatabaseLayer* activeLayer;
    leveldb::WriteBatch writeBatch;
};

#endif