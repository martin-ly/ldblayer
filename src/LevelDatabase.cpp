#include "LevelDatabase.h"
#include "LevelDatabaseLayer.h"
#include "LevelDatabaseTransaction.h"
#include "LevelDatabaseIterator.h"
#include <assert.h>

using std::string;

LevelDatabase::LevelDatabase()  noexcept
: db(nullptr)
{
}

LevelDatabase::LevelDatabase(const leveldb::Options& options, const std::string& dbpath) 
 : db(nullptr)
{    
    assert(open(options, path).ok());
}

LevelDatabase::~LevelDatabase()
{
    if (db)
        delete db;
}

leveldb::Status LevelDatabase::open(const leveldb::Options& options, const std::string& path) 
{
    if (db)
        delete db;
    
    leveldb::Status status = leveldb::DB::Open(options, path.c_str(), &db);
    if (status.ok()) {
        this->path = path;        
    } else {
        db = nullptr;
    }
    
    return status;
}

leveldb::Status LevelDatabase::Put(const string& key, const string& value)
{
    assert(db);
    return db->Put(writeOptions, key, value);
}

leveldb::Status LevelDatabase::Del(const std::string& key) 
{
    assert(db);
    return db->Delete(writeOptions, key);
}

leveldb::Status LevelDatabase::Get(const string& key, std::string* value)
{
    assert(db);
    return db->Get(readOptions, key, value);
}

LevelDatabaseLayer LevelDatabase::getLayer(const std::string& layerName) 
{
    layers.insert(layerName);
    
    LevelDatabaseLayer layer(this, layerName);
    return layer;
}

leveldb::Status LevelDatabase::Write(leveldb::WriteBatch* batch) 
{
    assert(db);
    return db->Write(writeOptions, batch);
}

LevelDatabaseTransaction LevelDatabase::createTransaction()
{
    assert(db);
    return LevelDatabaseTransaction(this);
}

LevelDatabaseIterator LevelDatabase::createIterator(LevelDatabaseLayer* layout)
{
    assert(db);
    
    leveldb::Iterator* it = db->NewIterator(readOptions);    
    
    return LevelDatabaseIterator(it, layout);
}

leveldb::Status LevelDatabase::destroy() 
{
    assert(!path.empty());
    
    if (db) {
        delete db;
        db = nullptr;
    }
    
    return leveldb::DestroyDB(path, leveldb::Options());
}


