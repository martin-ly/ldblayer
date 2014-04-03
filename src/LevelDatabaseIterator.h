#ifndef LEVELDBITERATOR_H
#define LEVELDBITERATOR_H

#include <leveldb/iterator.h>

namespace ldblayer
{

class Database;
class LevelDatabaseLayer;

class LevelDatabaseIterator
{
public:
	LevelDatabaseIterator(Database* db, LevelDatabaseLayer* layout = nullptr);
	~LevelDatabaseIterator();
    
	bool seekToFirst();
	bool seekToLast();
	bool seek(const std::string& key);
	bool next();
	bool prev();
	bool isValid() const noexcept { return m_isValid; }

	std::string operator* () const { return value(); }
	std::string key() const;
	std::string value() const;
	void setLayout(LevelDatabaseLayer* layout) { activeLayout = layout; }    
    
	void reopen();
private:    
	Database* database;
	LevelDatabaseLayer* activeLayout;
	leveldb::Iterator* it;    
	bool m_isValid;
};

}	// end of namespace

#endif