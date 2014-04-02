#ifndef LEVELDATABASE_H
#define LEVELDATABASE_H

#include <set>
#include <leveldb/db.h>
#include "LevelDatabaseAbstract.h"

class LevelDatabaseLayer;
class LevelDatabaseTransaction;
class LevelDatabaseIterator;

class LevelDatabase : public LevelDatabaseAbstract
{     
public:
    using layer_t = std::set<std::string>;
    
    LevelDatabase() noexcept;
    LevelDatabase(const leveldb::Options& options, const std::string& path);
    LevelDatabase(const LevelDatabase&) = delete;
    ~LevelDatabase();
    
    LevelDatabase& operator=(const LevelDatabase&) = delete;
    
    leveldb::Status open(const leveldb::Options& options, const std::string& path);
    
    leveldb::Status Put(const std::string& key, const std::string& value);
    leveldb::Status Del(const std::string& key);
    leveldb::Status Get(const std::string& key, std::string* value);
    leveldb::Status Write(leveldb::WriteBatch* batch);    
    
    LevelDatabaseLayer getLayer(const std::string& layerName);
    LevelDatabaseTransaction createTransaction();
    LevelDatabaseIterator createIterator(LevelDatabaseLayer* layout);
    const layer_t& getAllLayouts() const noexcept { return layers; }
    
    leveldb::Status destroy();   
    
    leveldb::DB* getDb() noexcept { return db; }

private:
    leveldb::DB* db;
    leveldb::ReadOptions readOptions;
    leveldb::WriteOptions writeOptions;
    
    std::string path;
    
    layer_t layers;
};

#endif