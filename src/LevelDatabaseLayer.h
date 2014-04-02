#ifndef LEVELDATABASELAYER_H
#define LEVELDATABASELAYER_H

#include "LevelDatabaseAbstract.h"

class LevelDatabase;

class LevelDatabaseLayer : public LevelDatabaseAbstract
{
public:
    LevelDatabaseLayer();
    LevelDatabaseLayer(LevelDatabase* levelDatabase, const std::string& layerName);
    
    void open(LevelDatabase* levelDatabase, const std::string& layerName) noexcept;
    void close();
    
    virtual leveldb::Status Put(const std::string& key, const std::string& value);
	virtual leveldb::Status Get(const std::string& key, std::string* value);
	virtual leveldb::Status Del(const std::string& key);
	virtual LevelDatabaseTransaction createTransaction();
	virtual LevelDatabaseIterator createIterator();
    
    const std::string& getLayerName() const noexcept { return prefix; }
    const std::string& getPrefix() const noexcept { return prefix; }   
    
    LevelDatabase* db() const noexcept { return m_db; }
    
private:
    LevelDatabase* m_db;
    std::string prefix;
};

#endif