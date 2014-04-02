#ifndef LEVELDBITERATOR_H
#define LEVELDBITERATOR_H

#include <leveldb/iterator.h>

class LevelDatabase;
class LevelDatabaseLayer;


class LevelDatabaseIterator
{
public:
    LevelDatabaseIterator(leveldb::Iterator* iterator, LevelDatabaseLayer* layout = nullptr);
    ~LevelDatabaseIterator();
    
    bool seekToFirst();
    bool seekToLast();
    bool seek(const std::string& key);
    bool next();
    bool prev();
    bool isValid() const noexcept { return m_isValid; }
    
    std::string operator* () const { return value(); }
    std::string key() const  { return it->key().ToString(); }
    std::string value() const { return it->value().ToString(); } 
    
private:    
    leveldb::Iterator* it;
    LevelDatabase* database;
    LevelDatabaseLayer* activeLayout;
    bool m_isValid;
};

#endif