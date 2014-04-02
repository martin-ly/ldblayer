#include "LevelDatabaseIterator.h"
#include "LevelDatabaseLayer.h"
#include "LevelDatabase.h"
#include <algorithm>

inline bool startsWith(const std::string& prefix, const std::string& toCheck) 
{
    return std::mismatch(prefix.begin(), prefix.end(), toCheck.begin()).first == prefix.end();    
}

LevelDatabaseIterator::LevelDatabaseIterator(leveldb::Iterator* iterator, LevelDatabaseLayer* layout)
: it(iterator),
  activeLayout(layout),
  m_isValid (false)
{
    /*leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        cout << it->key().ToString() << ": "  << it->value().ToString() << endl;
    }
    assert(it->status().ok());  // Check for any errors found during the scan
     delete it;*/
}

LevelDatabaseIterator::~LevelDatabaseIterator() 
{    
    if (it)
        delete it;
}

bool LevelDatabaseIterator::next() 
{   
    if (!m_isValid) {
        return false;
    }
    
    it->Next();
    
    if (it->Valid()) {
        if (activeLayout) {
            const std::string key = it->key().ToString();
            if (startsWith(activeLayout->getPrefix(), key)) {
                return m_isValid = true;
            }
        } else {
            return m_isValid = true;
        }
    }
    
    return m_isValid = false;
}

bool LevelDatabaseIterator::prev() 
{
    if (!m_isValid) {
        return false;
    }
    
    it->Prev();
    
    if (it->Valid()) {
        if (activeLayout) {
            const std::string key = it->key().ToString();
            if (startsWith(activeLayout->getPrefix(), key)) {
                return m_isValid = true;
            }
        } else {
            return m_isValid = true;
        }
    }
    
    return m_isValid = false;    
}

bool LevelDatabaseIterator::seekToFirst() 
{
    if (activeLayout) {
        it->Seek(activeLayout->getPrefix());
        const std::string key = it->key().ToString();
        if (activeLayout) {
            if (startsWith(activeLayout->getPrefix(), key)) {
                return m_isValid = true;
            } else {
                return m_isValid = false;
            }
        }
    } else {
        it->SeekToFirst();
    }
    
    return m_isValid = it->Valid();
}

bool LevelDatabaseIterator::seekToLast() 
{
    if (activeLayout) {
        LevelDatabase::layer_t layers = activeLayout->db()->getAllLayouts();   
        auto iter = layers.find(activeLayout->getPrefix());
        
        // database should contain current active layout.
        // if not -> than it is bug and this object was created outside of this database
        assert (iter != layers.end());
        
        // get next database prefix
        ++iter;
        
        // this is next prefix, so navigate to last element
        if (iter != layers.end()) {
            it->Seek(*iter);
        } else {
            it->SeekToLast();
        }
        
        if (it->Valid()) {                
            // check, if this key belongs to current layout.
            // SeekToLast can return key from prev. layout, if this layout doesn't contain any data
            const std::string& key = it->key().ToString();
            return m_isValid = startsWith(activeLayout->getPrefix(), key);
        }        
    } else {   
        it->SeekToLast();
        return m_isValid = it->Valid();
    }
    
    return m_isValid = false;
}

bool LevelDatabaseIterator::seek(const std::string& key) 
{
    if (activeLayout) {
        std::string seek_key = activeLayout->getPrefix();
        seek_key += key;
        
        it->Seek(seek_key);
        if (it->Valid()) {
            // if no such key, seek will return next lexicographical key
            const std::string& foundKey = it->key().ToString();
            
            if (startsWith(activeLayout->getPrefix(), foundKey)) {
                return m_isValid = true;
            }
        }
    } else {
        it->Seek(key);
        return m_isValid = it->Valid();
    }
    
    return m_isValid = false;
}